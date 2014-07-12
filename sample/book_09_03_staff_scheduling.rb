# -*- encoding : utf-8 -*-
#
# 9.3 Staff Scheduling
#
# = SYNOPSIS
# ruby book_09_03_staff_scheduling.rb
#
# == SOURCE

require 'gurobi'

# for debug to sync with ext C++ printf output
$stdout.sync = true
$stderr.sync = true

class Fixnum
  def *(var)
    case var
    when Gurobi::Var
      exp = Gurobi::LinExpr.new
      exp.addTerm(self, var)
      exp
    else
      super
    end
  end
end

class Float
  def *(var)
    case var
    when Gurobi::Var
      exp = Gurobi::LinExpr.new
      exp.addTerm(self, var)
      exp
    else
      super
    end
  end
end

module Gurobi
  class Var
    def >=(rhs)
      lhs = Gurobi::LinExpr.new
      lhs.addTerm(1, self)

      [
        lhs,
        Gurobi::GREATER_EQUAL,
        case rhs
        when Gurobi::LinExpr
          rhs
        when Gurobi::Var
          result = Gurobi::LinExpr.new
          result.addTerm(1, rhs)
          result
        else
          raise "Unsupported type #{rhs.class.to_s} in Gurobi::Var::>="
        end,
        'Hello'
      ]
    end

    def +(rhs)
      result = Gurobi::LinExpr.new
      result.addTerm(1, self)
      case rhs
      when Gurobi::LinExpr
        result.add_to(rhs)
      when Gurobi::Var
        result.addTerm(1, rhs)
      else
        raise "Unsupported type #{rhs.class.to_s} in Gurobi::Var::>="
      end
      result
    end

    def to_lin_expr
      result = Gurobi::LinExpr.new
      result.addTerm(1, self)
      result
    end
  end

  class LinExpr
    def ==(rhs)
      [self, Gurobi::EQUAL, rhs]
    end

    def <=(rhs)
      [self, Gurobi::LESS_EQUAL, rhs]
    end

    def >=(rhs)
      [self, Gurobi::GREATER_EQUAL, rhs]
    end

    def +(rhs)
      result = Gurobi::LinExpr.new(self)
      case rhs
      when Gurobi::LinExpr
        result.add_to(rhs)
      when Gurobi::Var
        result.addTerm(1, rhs)
      else
        raise "Unsupported type #{rhs.class.to_s} in Gurobi::Var::+"
      end
      result
    end
  end
end

def dump_lin_exprs(lin_exprs)
  print "== lin_exprs ==\n{\n"
  print lin_exprs.inspect, "\n"
  for exp in lin_exprs do
    next if !exp

    case exp
    when Gurobi::LinExpr
      exp.size.times do |i|
        var = exp.getVar(i)
  
        # BUG? Following line:
        #
        #   var.get(Gurobi::StringAttr::VarName))
        #
        # causes the following exception:
        #   Java::Gurobi::Gurobi::Exception:
        #   from gurobi.Gurobi::Var.get(Gurobi::Var.java:82)
        #   from sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
        #   from sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
        #   ...
        #
        # so, use inspect rather than var name
        printf("  %8s %s\n", exp.getCoeff(i).to_s, var.inspect)
      end
    when Gurobi::Var
      printf("  %s\n", exp.inspect)
    else
      raise "Unsupported type #{exp.class.to_s} in dump_lin_exprs"
    end
  end
  print "}\n\n"
end

# ∑(coeff x Gurobi::Var)
#
# When lin_exprs element(LinExpr type) is nil, it is ignored.
#
# @param [Array]  array of lin_exprs
def quicksum(lin_exprs)
  Gurobi::LinExpr.new.tap do |result|
    for exp in lin_exprs.flatten do
      next if !exp
      case exp
      when Gurobi::LinExpr
        exp.size.times do |i|
          result.addTerm(exp.getCoeff(i), exp.getVar(i))
        end
      when Gurobi::Var
        result.addTerm(1.0, exp)
      else
        raise "Unsupported type #{exp.class.to_s} in quicksum"
      end
    end
  end
end

class StaffMo
  # create model
  #
  # @param [Array]  staffs          staffs
  # @param [Int]    term            term (e.g. 7, or 30)
  # @param [int]    work_days       working days <= term
  # @param [Array]  shifts          shift (0: holiday, 1:morning, 2:daytime, ...)
  # @param [Array]  special_shifts  special shift; subset of shifts
  # @param [Hash]   c               cost for [i,t,j]; where i=staff, t=day, j=shift
  # @param [Hash]   b               number of staffs for [t,j]; where t=day, j=shift
  #
  # == instance variables
  # \@model::       Gurobi model
  # \@x::           shift allocation table
  # \@y::           error (number of shortage of staffs)
  # \@total_cost::  total cost
  # \@total_y::     total staff shortage
  def staff_mo(staffs, term, work_days, shifts, special_shifts, c, b)
    @model = Gurobi::Model.new(Gurobi::Env.new)

    # Following code (set initial value for hash) doesn't work:
    #
    #   @x = Hash.new(Hash.new({}))
    #   @y = Hash.new({})
    #
    # because each element shares initial hash created by {}
    @x = {}
    for i in staffs
      @x[i] = {} if !@x[i]
      for t in 1..term
        @x[i][t] = {} if !@x[i][t]
      end
    end

    @y = {}
    for t in 1..term
      @y[t] = {} if !@y[t]

      for j in shifts
        @y[t][j] = @model.addVar(0, Gurobi::INFINITY, 0, Gurobi::CONTINUOUS, 'y[%d,%d]' % [t,j])
        for i in staffs
          # 1 when staff i is allocated to term t of shift j
          @x[i][t][j] = @model.addVar(0, 1, 0, Gurobi::BINARY, 'x[%d,%d,%d]' % [i,t,j])
        end
      end
    end

    @total_cost  = @model.addVar(0, Gurobi::INFINITY, 0, Gurobi::CONTINUOUS, 'cost')
    @total_y     = @model.addVar(0, Gurobi::INFINITY, 0, Gurobi::CONTINUOUS, 'uncovered')

    @model.update

    # minimize total_cost
    @model.addConstr(*(@total_cost >= quicksum(
        staffs.map{|i| (1..term).map{|t| shifts.map{|j|
          c[i][t][j] * @x[i][t][j] if j!=0 }}})))

    # minimize total staff shortage
    @model.addConstr(*(@total_y >= quicksum(
        (1..term).map{|t| shifts.map{|j| @y[t][j] if j!=0}})))

    for t in (1..term)
      for j in shifts
        next if j==0

        # it is explained as follows to keep staff in (b - y):
        #
        #   quicksum(x[i,t,j]) >= b[t,j] - y[t,j]
        @model.addConstr(
            *(quicksum(staffs.map{|i| @x[i][t][j]}) + @y[t][j] >= b[t][j]), 'gap(%d,%d)' % [t,j])
      end
    end

    for i in staffs
      # work_days = term - holiday
      @model.addConstr(*(quicksum(
          (1..term).map{|t| shifts.map{|j| @x[i][t][j] if j!=0}}) == work_days), 'bb')

      # each staff should have 1 shift (includes holiday) for each term
      for t in (1..term)
        @model.addConstr(*(quicksum(shifts.map{|j| @x[i][t][j]})==1), 'staff_term(%d,%d)' % [i, t])
      end

      for t in 1..(term-1)
        for j in shifts
          if j != 0
            # staff should not have different shift tomorrow
            # (staff should have holiday to change different shift)
            #
            # when x[i,t,j] == 1, different shift k
            # cannot be allocated at t+1.
            # k should not be holiday, nor j.
            @model.addConstr(*(@x[i][t][j] + quicksum(
                shifts.map{|k| @x[i][t+1][k] if k!=j and k!=0}) <= 1), 'cont_shift(%d,%d)' % [t, j])
          end
        end
      end

      for t in 2..(term-1)
        for j in special_shifts
          # special_shifts should continue 2 days
          #
          # * "special shifts" is specified by special_shifts argument,
          #   e.g. daytime shift or night shift.
          #   In other words, when some staff has daytime shift at a day,
          #   the staff should have the same daytime shift next day.
          # * This means that when x[i,t,j] == 1, x[i,t-1,j] == 1
          #   or x[i,t+1,j] == 1
          # * It doesn't matter the case of x[i,t,j]==0
          @model.addConstr(*(@x[i][t-1][j] + @x[i][t+1][j] >= @x[i][t][j]), 'cont_shift_2days(%d,%d)' % [t, j])
        end
      end
    end

    @model.update
  end

  # @param [Array]            cand  candidate
  # @param [Gurobi::LinExpr]  obj   objective
  #
  # == 戻り値
  # @return [Array] candidates of (total cost, or total staff shortage)
  def optimize(cand, obj)
    # @model.Params.OutputFlag = 0 # silent mode
    @model.setObjective(obj, Gurobi::MINIMIZE)
    @model.update
    @model.optimize

    status = @model.get_int(Gurobi::IntAttr::STATUS)
    if status == Gurobi::OPTIMAL or status == Gurobi::UBOPTIMAL
      @model.get_int(Gurobi::IntAttr::SOL_COUNT).times do |k|
        @model.getEnv.set(Gurobi::IntParam::SOLUTION_NUMBER, k)
        
        cand << [
          @total_y.get_double(Gurobi::DoubleAttr::X),
          @total_cost.get_double(Gurobi::DoubleAttr::X)]
      end
    end
    print "cand: ", cand.inspect, "\n"
    status
  end

  def solve(staffs,term,work_days,shifts,special_shifts,c,b)
    shiftslabel = {0=>'Holiday', 1=>'Morning', 2=>'Daytime', 3=>'Night'}

    staff_mo(staffs,term,work_days,shifts,special_shifts,c,b)
    cand = []

    # total_cost or total_y
    if false
      stat1   = optimize(cand, @total_cost.to_lin_expr)
      print "Total cost: ", @total_cost.get_double(Gurobi::DoubleAttr::X), "\n"
    else
      stat1   = optimize(cand, @total_y.to_lin_expr)
      print "Total staff shortage: ", @total_y.get_double(Gurobi::DoubleAttr::X), "\n"
    end

    for i in staffs
      print "Staff ", i, "\n"
      print "           Mo Tu We Th Fr Sa Su\n"
      for j in shifts
        printf("  %7s: ", shiftslabel[j])
        for t in 1..term
          if @x[i][t][j].get_double(Gurobi::DoubleAttr::X) == 1
            print 'o  '
          else
            print '   '
          end
        end
        print "\n"
      end
    end
  end

private
  def dump_x(staffs, term, shifts)
    for t in 1..term
      for j in shifts
        for i in staffs
          printf("  x[%d][%d][%d] = %g %s %s\n", i,t,j,
              @x[i][t][j].get_double(Gurobi::DoubleAttr::X),
              @x[i][t][j].get_string(Gurobi::StringAttr::VAR_NAME),
              @x[i][t][j].inspect)
        end
      end
    end
  end

  def dump_y(term, shifts)
    for t in 1..term
      for j in shifts
        printf("  y[%d][%d] = %g %s %s\n", t,j,
              @y[t][j].get_double(Gurobi::DoubleAttr::X),
              @y[t][j].get_string(Gurobi::StringAttr::VAR_NAME),
              @y[t][j].inspect)
      end
    end
  end
end


#---------------------------------------------------------------------
# Main
#---------------------------------------------------------------------

#staffs = [1,2]
staffs = [1,2,3,4,5,6,7,8]

# Shift(holiday=0、morning, daytime, night)
shifts = [0,1,2,3]

# term(Monday .. Sunday)
term = 7

# working days = 5
work_days = 5

# special shift (daytime, night)
special_shifts = [2,3]

# cost(c[i,t,j] staff i, term t, shift j)
c = {
  # Staff1
  1 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
  # Staff2
  2 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
  # Staff3
  3 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
  # Staff4
  4 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
  # Staff5
  5 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
  # Staff6
  6 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
  # Staff7
  7 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
  # Staff8
  8 => {
        #Morning Daytime Night
    1 => {1=>10, 2=>20, 3=>10},   # Mon
    2 => {1=>10, 2=>20, 3=>10},   # Tue
    3 => {1=>10, 2=>20, 3=>10},   # Wed
    4 => {1=>10, 2=>20, 3=>10},   # Thu
    5 => {1=>10, 2=>20, 3=>10},   # Fri
    6 => {1=>10, 2=>20, 3=>10},   # Sat
    7 => {1=>10, 2=>20, 3=>10},   # Sun
  },
}

# Required number of staffs(b[t,j] term t, shift j)
b = {
      #Morning Daytime Night
  1 => {1=>2, 2=>2, 3=>2},    # Mon
  2 => {1=>2, 2=>2, 3=>2},    # Tue
  3 => {1=>2, 2=>2, 3=>2},    # Wed
  4 => {1=>2, 2=>2, 3=>2},    # Thu
  5 => {1=>2, 2=>2, 3=>2},    # Fri
  6 => {1=>2, 2=>2, 3=>2},    # Sat
  7 => {1=>2, 2=>2, 3=>2},    # Sun
}

StaffMo.new.solve(staffs, term, work_days, shifts, special_shifts, c, b)

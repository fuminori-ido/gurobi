=begin
   Assign workers to shifts; each worker may or may not be available on a
   particular day. We use Pareto optimization to solve the model:
   first, we minimize the linear sum of the slacks. Then, we constrain
   the sum of the slacks, and we minimize a quadratic objective that
   tries to balance the workload among the workers.
=end

# @return [Int] status
def solveAndPrint(model, totSlack, workers, totShifts)
  model.optimize
  status = model.get_int(Gurobi::IntAttr::STATUS)

  if status == Gurobi::INF_OR_UNBD || status == Gurobi::INFEASIBLE ||
     status == Gurobi::UNBOUNDED
    print "The model cannot be solved ",
        "because it is infeasible or unbounded\n"
    return status
  end
  if status != Gurobi::OPTIMAL
    print "Optimization was stopped with status ", status, "\n"
    return status
  end

  # Print total slack and the number of shifts worked for each worker
  print "\nTotal slack required: ",
      totSlack.get_double(Gurobi::DoubleAttr::X), "\n"
  workers.size.times do |w|
    print workers[w], " worked ",
        totShifts[w].get_double(Gurobi::DoubleAttr::X), " shifts\n"
  end
  print "\n"
  return status
end

def verify_shift(v, w, worker, num_shifts)
  v[w[worker]].get_double(Gurobi::DoubleAttr::X).should == num_shifts
end

describe 'Workforce4' do
  it 'works' do
    # Sets of days and workers
    Shifts =
      [ "Mon1", "Tue2", "Wed3", "Thu4", "Fri5", "Sat6",
        "Sun7", "Mon8", "Tue9", "Wed10", "Thu11", "Fri12", "Sat13",
        "Sun14" ]
    Workers =
      [ "Amy", "Bob", "Cathy", "Dan", "Ed", "Fred", "Gu" ]

    # Number of workers required for each shift
    shiftRequirements =
      [ 3, 2, 4, 4, 5, 6, 5, 2, 2, 3, 4, 6, 7, 5 ]

    # Worker availability: 0 if the worker is unavailable for a shift
    availability =
      [ [ 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 ],
        [ 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0 ],
        [ 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 ],
        [ 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 ],
        [ 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1 ],
        [ 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1 ],
        [ 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ] ]

    # Model
    env = Gurobi::Env.new
    model = Gurobi::Model.new(env)
    model.set_string(Gurobi::StringAttr::MODEL_NAME, "assignment")

    # Assignment variables: x[w][s] == 1 if worker w is assigned
    # to shift s. This is no longer a pure assignment model, so we must
    # use binary variables.
    xCt = 0
    x = []
    Workers.size.times do |w|
      x[w] = model.addVars(Shifts.size)
      xCt += 1
      model.update
      Shifts.size.times do |s|
        x[w][s].set_double(Gurobi::DoubleAttr::UB, availability[w][s])
        x[w][s].set_char(Gurobi::CharAttr::V_TYPE, Gurobi::BINARY)
        x[w][s].set_string(Gurobi::StringAttr::VAR_NAME,
            Workers[w] + "." + Shifts[s])
      end
    end

    # Slack variables for each shift constraint so that the shifts can
    # be satisfied
    slacks = model.addVars(Shifts.size)
    model.update
    Shifts.size.times do |s|
      slacks[s].set_string(Gurobi::StringAttr::VAR_NAME, Shifts[s] + "Slack")
    end

    # Variable to represent the total slack
    totSlack = model.addVar(0, Gurobi::INFINITY, 0, Gurobi::CONTINUOUS,
        "totSlack")

    # Variables to count the total shifts worked by each worker
    totShifts = model.addVars(Workers.size)
    model.update
    Workers.size.times do |w|
      totShifts[w].set_string(Gurobi::StringAttr::VAR_NAME,
          Workers[w] + "TotShifts")
    end

    # Update model to integrate new variables
    model.update

    # Constraint: assign exactly shiftRequirements[s] workers
    # to each shift s
    Shifts.size.times do |s|
      lhs = Gurobi::LinExpr.new(0)
      lhs.add_to slacks[s]
      Workers.size.times do |w|
        lhs.add_to x[w][s]
      end
      model.addConstr(lhs == shiftRequirements[s], Shifts[s])
    end

    # Constraint: set totSlack equal to the total slack
    lhs = Gurobi::LinExpr.new(0)
    Shifts.size.times do |s|
      lhs.add_to slacks[s]
    end
    model.addConstr(lhs == totSlack, "totSlack")

    # Constraint: compute the total number of shifts for each worker
    Workers.size.times do |w|
      lhs = Gurobi::LinExpr.new(0)
      Shifts.size.times do |s|
        lhs.add_to x[w][s]
      end
      model.addConstr(lhs == totShifts[w], "totShifts" + Workers[w])
    end

    # Objective: minimize the total slack
    obj = Gurobi::LinExpr.new(0)
    obj += totSlack
    model.setObjective(obj)

    # Optimize
    status = solveAndPrint(model, totSlack, Workers, totShifts)
    if status != Gurobi::OPTIMAL
      return 1
    end

    # Constrain the slack by setting its upper and lower bounds
    totSlack.set_double(Gurobi::DoubleAttr::UB, totSlack.get_double(Gurobi::DoubleAttr::X))
    totSlack.set_double(Gurobi::DoubleAttr::LB, totSlack.get_double(Gurobi::DoubleAttr::X))

    # Variable to count the average number of shifts worked
    avgShifts = model.addVar(0, Gurobi::INFINITY, 0, Gurobi::CONTINUOUS, "avgShifts")

    # Variables to count the difference from average for each worker;
    # note that these variables can take negative values.
    diffShifts = model.addVars(Workers.size)
    model.update
    Workers.size.times do |w|
      diffShifts[w].set_string(Gurobi::StringAttr::VAR_NAME, Workers[w] + "Diff")
      diffShifts[w].set_double(Gurobi::DoubleAttr::LB, -Gurobi::INFINITY)
    end

    # Update model to integrate new variables
    model.update

    # Constraint: compute the average number of shifts worked
    lhs = Gurobi::LinExpr.new(0)
    Workers.size.times do |w|
      lhs.add_to totShifts[w]
    end
    model.addConstr(lhs == Workers.size * avgShifts, "avgShifts")

    # Constraint: compute the difference from the average number of shifts
    Workers.size.times do |w|
      lhs = Gurobi::LinExpr.new(0)
      lhs.add_to totShifts[w]
      lhs.sub_from avgShifts
      model.addConstr(lhs == diffShifts[w], Workers[w] + "Diff")
    end

    # Objective: minimize the sum of the square of the difference from the
    # average number of shifts worked
    qobj = Gurobi::QuadExpr.new
    Workers.size.times do |w|
      qobj += diffShifts[w] * diffShifts[w]
    end
    model.setObjective(qobj)

    # Optimize
    status = solveAndPrint(model, totSlack, Workers, totShifts)
    if status != Gurobi::OPTIMAL
      return 1;
    end

    # w: hash of name to index of Workers
    w = {}.tap{|h| Workers.each_with_index{|x, i| h[x]=i}}

    model.get_int(Gurobi::IntAttr::STATUS).should == Gurobi::OPTIMAL
    totSlack.get_double(Gurobi::DoubleAttr::X).should == 6.0
    verify_shift(totShifts, w, 'Amy',   7.0)
    verify_shift(totShifts, w, 'Bob',   7.0)
    verify_shift(totShifts, w, 'Cathy', 8.0)
    verify_shift(totShifts, w, 'Dan',   7.0)
    verify_shift(totShifts, w, 'Ed',    8.0)
    verify_shift(totShifts, w, 'Fred',  7.0)
    verify_shift(totShifts, w, 'Gu',    8.0)
  end
end

class Fixnum
  # '*' overload to generate LinExpr when rhs is Gurobi::Var
  #
  # when rhs is other than Gurobi::Var, it calculates normal multiplication
  # of self * rhs
  #
  # === Example
  #   3 * v
  #
  # will return LinExpr object
  #
  # @return [LinExpr] when rhs is Gurobi::Var
  def multiply_with_gurobi(rhs)
    case rhs
    when Gurobi::Var
      exp = Gurobi::LinExpr.new
      exp.addTerm(self, rhs)
      exp
    else
      multiply_without_gurobi(rhs)
    end
  end
  alias_method :multiply_without_gurobi, :'*'
  alias_method :'*', :multiply_with_gurobi
end

class Float
  # '*' overload to generate LinExpr when rhs is Gurobi::Var
  #
  # when rhs is other than Gurobi::Var, it calculates normal multiplication
  # of self * rhs
  #
  # === Example
  #   3.14 * v
  #
  # will return LinExpr object
  #
  # @return [LinExpr] when rhs is Gurobi::Var
  def multiply_with_gurobi(rhs)
    case rhs
    when Gurobi::Var
      exp = Gurobi::LinExpr.new
      exp.addTerm(self, rhs)
      exp
    else
      multiply_without_gurobi(rhs)
    end
  end
  alias_method :multiply_without_gurobi, :'*'
  alias_method :'*', :multiply_with_gurobi
end


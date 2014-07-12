=begin
   This example reads a MIP model from a file, adds artificial
   variables to each constraint, and then minimizes the sum of the
   artificial variables.  A solution with objective zero corresponds
   to a feasible solution to the input model.
   We can also use FeasRelax feature to do it. In this example, we
   use minrelax=1, i.e. optimizing the returned model finds a solution
   that minimizes the original objective, but only from among those
   solutions that minimize the sum of the artificial variables.
=end

describe 'Feasopt' do
  it 'works' do
    env = Gurobi::Env.new
    feasmodel = Gurobi::Model.new(env, 'spec/data/klein1.mps')

    # Create a copy to use FeasRelax feature later
    feasmodel1 = Gurobi::Model.new(feasmodel)

    # clear objective
    feasmodel.setObjective(Gurobi::LinExpr.new(0.0))

    # add slack variables
    c = feasmodel.getConstrs
    feasmodel.get_int(Gurobi::IntAttr::NUM_CONSTRS).times do |i|
      sense = c[i].get_char(Gurobi::CharAttr::SENSE)
      if sense != Gurobi::GREATER_EQUAL
        coef = -1.0
        feasmodel.addVar(0.0, Gurobi::INFINITY, 1.0, Gurobi::CONTINUOUS, 1,
            [c[i]], [coef],
            "ArtN_" + c[i].get_string(Gurobi::StringAttr::CONSTR_NAME))
      end
      if sense != Gurobi::LESS_EQUAL
        coef = 1.0
        feasmodel.addVar(0.0, Gurobi::INFINITY, 1.0, Gurobi::CONTINUOUS, 1,
            [c[i]], [coef],
            "ArtP_" + c[i].get_string(Gurobi::StringAttr::CONSTR_NAME))
      end
    end
    feasmodel.update

    # optimize modified model
    feasmodel.write("/tmp/feasopt.lp")
    feasmodel.optimize

    # use FeasRelax feature */
    feasmodel1.feasRelax(Gurobi::FEASRELAX_LINEAR, true, false, true)
    feasmodel1.write("/tmp/feasopt1.lp")
    feasmodel1.optimize
  end
end

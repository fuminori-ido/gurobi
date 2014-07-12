=begin
   Use parameters that are associated with a model.

   A MIP is solved for 5 seconds with different sets of parameters.
   The one with the smallest MIP gap is selected, and the optimization
   is resumed until the optimal solution is found.
=end

# Simple function to determine the MIP gap
def gap(model)
  if model.get_int(Gurobi::IntAttr::SOL_COUNT) == 0 ||
     model.get_double(Gurobi::DoubleAttr::OBJ_VAL).abs < 1e-6
    Gurobi::INFINITY
  else
    (model.get_double(Gurobi::DoubleAttr::OBJ_BOUND) -
     model.get_double(Gurobi::DoubleAttr::OBJ_VAL)).abs /
    model.get_double(Gurobi::DoubleAttr::OBJ_VAL).abs
  end
end

describe 'Params' do
  it 'works' do
    # Read model and verify that it is a MIP
    env = Gurobi::Env.new
    base = Gurobi::Model.new(env, 'spec/data/misc07.mps')
    base.get_int(Gurobi::IntAttr::IS_MIP).should_not == 0

    # Set a 5 second time limit
    base.getEnv.set_double(Gurobi::DoubleParam::TIME_LIMIT, 5)

    # Now solve the model with different values of MIPFocus,
    # using a pointer to save the best model
    bestGap = Gurobi::INFINITY
    bestModel = nil
    4.times do |i|
      m = Gurobi::Model.new(base)
      m.getEnv.set_int(Gurobi::IntParam::MIP_FOCUS, i)
      m.optimize
      if bestModel == nil || bestGap > gap(m)
        bestModel = m
        bestGap = gap(bestModel)
      end
    end

    # Finally, reset the time limit and continue to solve the
    # best model to optimality
    bestModel.getEnv.set_double(Gurobi::DoubleParam::TIME_LIMIT, Gurobi::INFINITY)
    bestModel.optimize
    check_range(bestModel.get_double(Gurobi::DoubleAttr::OBJ_VAL),
      2.810000000000e+03,
      2.820000000000e+03)
    bestModel.getEnv.get_int(Gurobi::IntParam::MIP_FOCUS).should == 1
    print "Solved with MIPFocus: ",
        bestModel.getEnv.get_int(Gurobi::IntParam::MIP_FOCUS), "\n"
  end
end

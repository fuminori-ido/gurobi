=begin
   Solve a model with different values of the Method parameter;
   show which value gives the shortest solve time.
=end

describe 'Lpmethod' do
  it 'works' do
    # Read model
    env = Gurobi::Env.new
    m = Gurobi::Model.new(env, 'spec/data/afiro.mps')
    menv = m.getEnv

    # Solve the model with different values of Method
    bestMethod = -1
    bestTime = menv.get_double(Gurobi::DoubleParam::TIME_LIMIT)
    2.times do |i|
      m.reset
      menv.set_int(Gurobi::IntParam::METHOD, i)
      m.optimize
      if m.get_int(Gurobi::IntAttr::STATUS) == Gurobi::OPTIMAL
        bestTime = m.get_double(Gurobi::DoubleAttr::RUNTIME)
        bestMethod = i
        # Reduce the TimeLimit parameter to save time
        # with other methods
        menv.set_double(Gurobi::DoubleParam::TIME_LIMIT, bestTime)
      end
    end

    # Report which method was fastest
    if bestMethod == -1
      print "Unable to solve this model\n"
    else
      print "Solved in ", bestTime,
          " seconds with Method: ", bestMethod, "\n"
    end
    bestMethod.should == 1
  end
end

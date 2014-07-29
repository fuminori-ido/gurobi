=begin
   This example reads a MIP model from a file, solves it and
   prints the objective values from all feasible solutions
   generated while solving the MIP. Then it creates the fixed
   model and solves that model.
=end

describe 'Mip2' do
  it 'works' do
    env = Gurobi::Env.new
    model = Gurobi::Model.new(env, model_file('stein9.mps'))

    if model.get_int(Gurobi::IntAttr::IS_MIP) == 0
      raise Gurobi::Exception("Model is not a MIP")
    end

    model.optimize

    optimstatus = model.get_int(Gurobi::IntAttr::STATUS)

    optimstatus.should == Gurobi::OPTIMAL
    model.get_double(Gurobi::DoubleAttr::OBJ_VAL).should == 5.0

    print "Optimization complete\n"
    objval = 0
    if optimstatus == Gurobi::OPTIMAL
      objval = model.get_double(Gurobi::DoubleAttr::OBJ_VAL)
      print "Optimal objective: ", objval, "\n"
    elsif optimstatus == Gurobi::INF_OR_UNBD
      print  "Model is infeasible or unbounded\n"
      return 0
    elsif optimstatus == Gurobi::INFEASIBLE
      print "Model is infeasible\n"
      return 0
    elsif optimstatus == Gurobi::UNBOUNDED
      print "Model is unbounded\n"
      return 0
    else
      print "Optimization was stopped with status = ",
          optimstatus, "\n"
      return 0;
    end

    # Iterate over the solutions and compute the objectives

    numvars = model.get_int(Gurobi::IntAttr::NUM_VARS)
    vars = model.getVars
    model.getEnv.set_int(Gurobi::IntParam::OUTPUT_FLAG, 0)

    print "\n"
    model.get_int(Gurobi::IntAttr::SOL_COUNT).times do |k|
      model.getEnv.set_int(Gurobi::IntParam::SOLUTION_NUMBER, k)
      objn = 0.0

      numvars.times do |j|
        v = vars[j]
        objn += v.get_double(Gurobi::DoubleAttr::OBJ) * v.get_double(Gurobi::DoubleAttr::XN)
      end

      print "Solution ", k, " has objective: ", objn, "\n"

      if k == 0
        objn.should == 5.0
      end
    end
    print "\n"
    model.getEnv.set_int(Gurobi::IntParam::OUTPUT_FLAG, 1)

    # Create a fixed model, turn off presolve and solve

    fixed = model.fixedModel

    fixed.getEnv.set_int(Gurobi::IntParam::PRESOLVE, 0)

    fixed.optimize

    foptimstatus = fixed.get_int(Gurobi::IntAttr::STATUS)

    if foptimstatus != Gurobi::OPTIMAL
      $stderr.print "Error: fixed model isn't optimal\n"
      return 0
    end

    fobjval = fixed.get_double(Gurobi::DoubleAttr::OBJ_VAL)

    if (fobjval - objval).abs > 1.0e-6 * (1.0 + objval.abs)
      $stderr.print "Error: objective values are different\n"
      return 0
    end

    # Print values of nonzero variables
    fvars = fixed.getVars
    
    numvars.times do |j|
      v = fvars[j]
      if v.get_double(Gurobi::DoubleAttr::X) != 0.0
        print v.get_string(Gurobi::StringAttr::VAR_NAME), " ",
            v.get_double(Gurobi::DoubleAttr::X), "\n"
      end
    end
  end
end

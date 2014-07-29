=begin
   Implement a simple MIP heuristic.  Relax the model,
   sort variables based on fractionality, and fix the 25% of
   the fractional variables that are closest to integer variables.
   Repeat until either the relaxation is integer feasible or
   linearly infeasible.
=end

def vcomp(v1, v2)
  sol1  = v1.get_double(Gurobi::DoubleAttr::X).abs
  sol2  = v2.get_double(Gurobi::DoubleAttr::X).abs
  frac1 = (sol1 - (sol1 + 0.5).floor).abs
  frac2 = (sol2 - (sol2 + 0.5).floor).abs
  frac1 <=> frac2
end

describe 'Fixanddive' do

  it 'works' do
    # Read model
    env = Gurobi::Env.new
    Gurobi::Model 
    model = Gurobi::Model.new(env, model_file('stein9.mps'))

    # Collect integer variables and relax them
    # Note that we use Gurobi::Var* to copy variables
    intvars = []
    x = model.getVars
    x.size.times do |j|
      if (x[j].get_char(Gurobi::CharAttr::V_TYPE) != Gurobi::CONTINUOUS)
        intvars.push(x[j])
        x[j].set_char(Gurobi::CharAttr::V_TYPE, Gurobi::CONTINUOUS)
      end
    end

    model.getEnv.set_int(Gurobi::IntParam::OUTPUT_FLAG, 0)
    model.optimize

    # Perform multiple iterations. In each iteration, identify the first
    # quartile of integer variables that are closest to an integer value
    # in the relaxation, fix them to the nearest integer, and repeat.
    for iter in 0..1000 do
      # create a list of fractional variables, sorted in order of
      # increasing distance from the relaxation solution to the nearest
      # integer value
      fractional = []
      intvars.size.times do |j|
        sol = intvars[j].get_double(Gurobi::DoubleAttr::X).abs
        if (sol - (sol + 0.5).floor).abs > 1e-5
          fractional.push(intvars[j])
        end
      end

      print "Iteration ", iter, ", obj ",
          model.get_double(Gurobi::DoubleAttr::OBJ_VAL), ", fractional ",
          fractional.size(), "\n"

      if fractional.size == 0
        print "Found feasible solution - objective ",
            model.get_double(Gurobi::DoubleAttr::OBJ_VAL), "\n"
        model.get_double(Gurobi::DoubleAttr::OBJ_VAL).should == 5.0
        break;
      end

      # Fix the first quartile to the nearest integer value
      f = fractional.sort{|a, b| vcomp(a, b)}
      nfix = f.size / 4
      nfix = (nfix > 1) ? nfix : 1
      nfix.times do |i|
        v = f[i]
        fixval = (v.get_double(Gurobi::DoubleAttr::X) + 0.5).floor
        v.set_double(Gurobi::DoubleAttr::LB, fixval)
        v.set_double(Gurobi::DoubleAttr::UB, fixval)
        print "  Fix ", v.get_string(Gurobi::StringAttr::VAR_NAME), " to ",
            fixval, " ( rel ", v.get_double(Gurobi::DoubleAttr::X), " )\n"
      end

      model.optimize

      # Check optimization result
      if model.get_int(Gurobi::IntAttr::STATUS) != Gurobi::OPTIMAL
        print "Relaxation is infeasible\n"
        break
      end
    end
  end
end

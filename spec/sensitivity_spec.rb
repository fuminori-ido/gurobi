=begin
   Simple MIP sensitivity analysis example.
   For each integer variable, fix it to its lower and upper bound
   and check the impact on the objective.
=end

describe 'Sensitivity' do
  it 'works' do
    # Read model
    env = Gurobi::Env.new
    a = Gurobi::Model.new(env, 'spec/data/p0033.lp')
    a.optimize
    a.getEnv.set_int(Gurobi::IntParam::OUTPUT_FLAG, 0)

    # Extract variables from model
    avars = a.getVars

    avars.size.times do |i|
      v = avars[i]
      if v.get_char(Gurobi::CharAttr::V_TYPE) == Gurobi::BINARY

        # Create clone and fix variable
        b = Gurobi::Model.new(a)
        bv = b.getVars
        if v.get_double(Gurobi::DoubleAttr::X) - v.get_double(Gurobi::DoubleAttr::LB) < 0.5
          bv[i].set_double(Gurobi::DoubleAttr::LB, bv[i].get_double(Gurobi::DoubleAttr::UB))
        else
          bv[i].set_double(Gurobi::DoubleAttr::UB, bv[i].get_double(Gurobi::DoubleAttr::LB))
        end

        b.optimize

        if b.get_int(Gurobi::IntAttr::STATUS) == Gurobi::OPTIMAL
          objchg = b.get_double(Gurobi::DoubleAttr::OBJ_VAL) - a.get_double(Gurobi::DoubleAttr::OBJ_VAL)
          if objchg < 0
            objchg = 0
          end
          print "Objective sensitivity for variable ",
              v.get_string(Gurobi::StringAttr::VAR_NAME), " is ", objchg, "\n"
        else
          print "Objective sensitivity for variable ",
              v.get_string(Gurobi::StringAttr::VAR_NAME), " is infinite\n"
        end
      end
    end
  end
end

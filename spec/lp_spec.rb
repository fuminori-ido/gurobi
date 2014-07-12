=begin
   This example reads an LP model from a file and solves it.
   If the model is infeasible or unbounded, the example turns off
   presolve and solves the model again. If the model is infeasible,
   the example computes an Irreducible Infeasible Subsystem (IIS),
   and writes it to a file 
=end

describe 'Lp' do
  it 'works' do
    env = Gurobi::Env.new
    model = Gurobi::Model.new(env, 'spec/data/klein1.mps')

    model.optimize

    optimstatus = model.get_int(Gurobi::IntAttr::STATUS)

    if optimstatus == Gurobi::INF_OR_UNBD
      model.getEnv.set_int(Gurobi::IntParam::PRESOLVE, 0)
      model.optimize
      optimstatus = model.get_int(Gurobi::IntAttr::STATUS)
    end

    if optimstatus == Gurobi::OPTIMAL
      objval = model.get_double(Gurobi::DoubleAttr::OBJ_VAL)
      print "Optimal objective: ", objval, "\n"
    elsif optimstatus == Gurobi::INFEASIBLE
      optimstatus.should == Gurobi::INFEASIBLE
      print "Model is infeasible\n"

      # compute and write out IIS

      model.computeIIS
      model.write("/tmp/model.ilp")
    elsif optimstatus == Gurobi::UNBOUNDED
      print "Model is unbounded\n"
    else
      print "Optimization was stopped with status = ",
          optimstatus, "\n"
    end
  end
end

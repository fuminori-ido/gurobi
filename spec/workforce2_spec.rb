=begin
   Assign workers to shifts; each worker may or may not be available on a
   particular day. If the problem cannot be solved, use IIS iteratively to
   find all conflicting constraints.
=end

describe 'Workforce2' do
  it 'works' do
    # Sets of days and workers
    Shifts =
      [ "Mon1", "Tue2", "Wed3", "Thu4", "Fri5", "Sat6",
        "Sun7", "Mon8", "Tue9", "Wed10", "Thu11", "Fri12", "Sat13",
        "Sun14"]
    Workers =
      [ "Amy", "Bob", "Cathy", "Dan", "Ed", "Fred", "Gu" ]

    # Number of workers required for each shift
    shiftRequirements =
      [ 3, 2, 4, 4, 5, 6, 5, 2, 2, 3, 4, 6, 7, 5 ]

    # Amount each worker is paid to work one shift
    pay = [ 10, 12, 10, 8, 8, 9, 11 ]

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
    # to shift s. Since an assignment model always produces integer
    # solutions, we use continuous variables and solve as an LP.
    xCt = 0
    x   = []
    Workers.size.times do |w|
      x[w] = model.addVars(Shifts.size)
      xCt += 1
      model.update
      Shifts.size.times do |s|
        x[w][s].set_double(Gurobi::DoubleAttr::UB, availability[w][s])
        x[w][s].set_double(Gurobi::DoubleAttr::OBJ, pay[w])
        x[w][s].set_string(Gurobi::StringAttr::VAR_NAME,
            Workers[w] + "." + Shifts[s])
      end
    end

    # The objective is to minimize the total pay costs
    model.set_int(Gurobi::IntAttr::MODEL_SENSE, 1)

    # Update model to integrate new variables
    model.update

    # Constraint: assign exactly shiftRequirements[s] workers
    # to each shift s
    Shifts.size.times do |s|
      lhs = Gurobi::LinExpr.new(0.0)
      Workers.size.times do |w|
        lhs += x[w][s];
      end
      model.addConstr(lhs == shiftRequirements[s], Shifts[s])
    end

    # Optimize
    model.optimize
    status = model.get_int(Gurobi::IntAttr::STATUS)
    if status == Gurobi::UNBOUNDED
      print "The model cannot be solved ",
          "because it is unbounded\n"
      return 1;
    end
    if status == Gurobi::OPTIMAL
      print "The optimal objective is ",
          model.get_double(Gurobi::DoubleAttr::OBJ_VAL), "\n"
      return 0;
    end
    if status != Gurobi::INF_OR_UNBD && status != Gurobi::INFEASIBLE
      print "Optimization was stopped with status ", status, "\n"
      return 1;
    end

    # do IIS
    status.should == Gurobi::INFEASIBLE
    print "The model is infeasible; computing IIS\n"
    removed = []

    # Loop until we reduce to a model that can be solved
    while true do
      model.computeIIS
      print "\nThe following constraint cannot be satisfied:\n"
      c = model.getConstrs()
      c.size.times do |i|
        if c[i].get_int(Gurobi::IntAttr::IIS_CONSTR) == 1
          print c[i].get_string(Gurobi::StringAttr::CONSTR_NAME), "\n"
          # Remove a single constraint from the model
          removed << c[i].get_string(Gurobi::StringAttr::CONSTR_NAME)
          model.remove(c[i])
          break
        end
      end
      c = []

      print "\n"
      model.optimize
      status = model.get_int(Gurobi::IntAttr::STATUS)

      if status == Gurobi::UNBOUNDED
        print "The model cannot be solved because it is unbounded\n"
        return 0;
      end
      if status == Gurobi::OPTIMAL
        break;
      end
      if status != Gurobi::INF_OR_UNBD && status != Gurobi::INFEASIBLE
        print "Optimization was stopped with status ", status, "\n"
        return 1;
      end
    end
    print "\nThe following constraints were removed ",
        "to get a feasible LP:\n"

    for r in removed do
      print r, " "
    end
    print "\n"
    removed.should == %w(Fri12 Sat6 Sun7 Thu4)
  end
end

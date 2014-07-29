=begin
   This example reads an LP model from a file and solves it.
   If the model can be solved, then it finds the smallest positive variable,
   sets its upper bound to zero, and resolves the model two ways:
   first with an advanced start, then without an advanced start
   (i.e. 'from scratch').
=end


describe 'Lpmod' do
  it 'works' do
    # Read model and determine whether it is an LP
    env = Gurobi::Env.new
    model = Gurobi::Model.new(env, model_file('afiro.mps'))
    if model.get_int(Gurobi::IntAttr::IS_MIP) != 0
      print "The model is not a linear program\n"
      return 1
    end
    model.optimize
    check_range(
        model.get_double(Gurobi::DoubleAttr::OBJ_VAL),
        -4.647531429e+02,
        -4.647531428e+02)

    status = model.get_int(Gurobi::IntAttr::STATUS)

    if status == Gurobi::INF_OR_UNBD || status == Gurobi::INFEASIBLE ||
       status == Gurobi::UNBOUNDED
      print "The model cannot be solved because it is ",
          "infeasible or unbounded\n"
      return 1
    end

    if status != Gurobi::OPTIMAL
      print "Optimization was stopped with status ", status, "\n"
      return 0
    end

    # Find the smallest variable value
    minVal = Gurobi::INFINITY
    minVar = 0
    v = model.getVars
    v.size.times do |j|
      sol = v[j].get_double(Gurobi::DoubleAttr::X)
      if sol > 0.0001 && sol < minVal &&
         v[j].get_double(Gurobi::DoubleAttr::LB) == 0.0
        minVal = sol
        minVar = j
      end
    end

    check_range(minVal, 18.2142, 18.2143)
    print "\n*** Setting ", v[minVar].get_string(Gurobi::StringAttr::VAR_NAME),
        " from ", minVal, " to zero ***\n\n"
    v[minVar].set_double(Gurobi::DoubleAttr::UB, 0.0)

    # Solve from this starting point
    model.optimize
    check_range(
        model.get_double(Gurobi::DoubleAttr::OBJ_VAL),
        -4.589245715e+02,
        -4.589245714e+02)

    # Save iteration & time info
    warmCount = model.get_double(Gurobi::DoubleAttr::ITER_COUNT)
    warmTime  = model.get_double(Gurobi::DoubleAttr::RUNTIME)

    # Reset the model and resolve
    print "\n*** Resetting and solving ",
        "without an advanced start ***\n\n"
    model.reset
    model.optimize
    check_range(
        model.get_double(Gurobi::DoubleAttr::OBJ_VAL),
        -4.589245715e+02,
        -4.589245714e+02)

    # Save iteration & time info
    coldCount = model.get_double(Gurobi::DoubleAttr::ITER_COUNT)
    coldTime  = model.get_double(Gurobi::DoubleAttr::RUNTIME)

    print "\n*** Warm start: ", warmCount, " iterations, ",
        warmTime, " seconds\n"
    print "*** Cold start: ", coldCount, " iterations, ",
        coldTime, " seconds\n"
  end
end

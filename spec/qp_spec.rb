=begin
   This example formulates and solves the following simple QP model:

     minimize    x^2 + x*y + y^2 + y*z + z^2 + 2 x
     subject to  x + 2 y + 3 z >= 4
                 x +   y       >= 1

   It solves it once as a continuous model, and once as an integer model.
=end

describe 'Qp' do
  it 'works' do
    env = Gurobi::Env.new
    model = Gurobi::Model.new(env)

    # Create variables

    x = model.addVar(0.0, 1.0, 0.0, Gurobi::CONTINUOUS, "x")
    y = model.addVar(0.0, 1.0, 0.0, Gurobi::CONTINUOUS, "y")
    z = model.addVar(0.0, 1.0, 0.0, Gurobi::CONTINUOUS, "z")

    # Integrate new variables

    model.update

    # Set objective

    obj = x*x + x*y + y*y + y*z + z*z + 2*x
    model.setObjective(obj)

    # Add constraint: x + 2 y + 3 z >= 4

    model.addConstr(x + 2 * y + 3 * z >= 4, "c0")

    # Add constraint: x + y >= 1

    model.addConstr(x + y >= 1, "c1")

    # Optimize model

    model.optimize

    print x.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          x.get_double(Gurobi::DoubleAttr::X), "\n"
    print y.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          y.get_double(Gurobi::DoubleAttr::X), "\n"
    print z.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          z.get_double(Gurobi::DoubleAttr::X), "\n"

    check_range(x.get_double(Gurobi::DoubleAttr::X), 0.00, 0.01)
    check_range(y.get_double(Gurobi::DoubleAttr::X), 0.99, 1.00)
    check_range(z.get_double(Gurobi::DoubleAttr::X), 0.66, 0.67)

    print "Obj: ", model.get_double(Gurobi::DoubleAttr::OBJ_VAL), "\n"
    check_range(model.get_double(Gurobi::DoubleAttr::OBJ_VAL), 2.11, 2.12)

    # Change variable types to integer

    x.set_char(Gurobi::CharAttr::V_TYPE, Gurobi::INTEGER)
    y.set_char(Gurobi::CharAttr::V_TYPE, Gurobi::INTEGER)
    z.set_char(Gurobi::CharAttr::V_TYPE, Gurobi::INTEGER)

    # Optimize model

    model.optimize

    print x.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          x.get_double(Gurobi::DoubleAttr::X), "\n"
    print y.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          y.get_double(Gurobi::DoubleAttr::X), "\n"
    print z.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          z.get_double(Gurobi::DoubleAttr::X), "\n"

    x.get_double(Gurobi::DoubleAttr::X).should  == 0
    y.get_double(Gurobi::DoubleAttr::X).should  == 1
    z.get_double(Gurobi::DoubleAttr::X).should  == 1

    print "Obj: ", model.get_double(Gurobi::DoubleAttr::OBJ_VAL), "\n"
    model.get_double(Gurobi::DoubleAttr::OBJ_VAL).should  == 3
  end
end

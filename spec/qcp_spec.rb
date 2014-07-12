=begin
   This example formulates and solves the following simple QCP model:

     maximize    x
     subject to  x + y + z = 1
                 x^2 + y^2 <= z^2 (second-order cone)
                 x^2 <= yz        (rotated second-order cone)
=end

describe 'Qcp' do
  it 'works' do
    env = Gurobi::Env.new
    model = Gurobi::Model.new(env)

    # Create variables

    x = model.addVar(0.0, Gurobi::INFINITY, 0.0, Gurobi::CONTINUOUS, "x")
    y = model.addVar(0.0, Gurobi::INFINITY, 0.0, Gurobi::CONTINUOUS, "y")
    z = model.addVar(0.0, Gurobi::INFINITY, 0.0, Gurobi::CONTINUOUS, "z")

    # Integrate new variables

    model.update

    # Set objective

    obj = Gurobi::LinExpr.new(x)
    model.setObjective(obj, Gurobi::MAXIMIZE)

    # Add linear constraint: x + y + z = 1

    model.addConstr(x + y + z == 1, "c0")

    # Add second-order cone: x^2 + y^2 <= z^2

    model.addQConstr(x*x + y*y <= z*z, "qc0")

    # Add rotated cone: x^2 <= yz

    model.addQConstr(x*x <= y*z, "qc1")

    # Optimize model

    model.optimize

    print x.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          x.get_double(Gurobi::DoubleAttr::X), "\n"
    print y.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          y.get_double(Gurobi::DoubleAttr::X), "\n"
    print z.get_string(Gurobi::StringAttr::VAR_NAME), " ",
          z.get_double(Gurobi::DoubleAttr::X), "\n"

    print "Obj: ", model.get_double(Gurobi::DoubleAttr::OBJ_VAL), "\n"

    check_range(x.get_double(Gurobi::DoubleAttr::X), 0.326, 0.327)
    check_range(y.get_double(Gurobi::DoubleAttr::X), 0.257, 0.258)
    check_range(z.get_double(Gurobi::DoubleAttr::X), 0.415, 0.416)
    check_range(model.get_double(Gurobi::DoubleAttr::OBJ_VAL), 0.326, 0.327)
  end
end

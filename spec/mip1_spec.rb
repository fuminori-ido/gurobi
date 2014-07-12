=begin

test by Gurobi example mip1

     maximize    x +   y + 2 z
     subject to  x + 2 y + 3 z <= 4
                 x +   y       >= 1
     x, y, z binary
=end

describe 'Mip1' do
  it 'works' do
    model = Gurobi::Model.new(Gurobi::Env.new)

    x = model.addVar(0.0, 1.0, 1.0, Gurobi::BINARY, "x")
    y = model.addVar(0.0, 1.0, 1.0, Gurobi::BINARY, "y")
    z = model.addVar(0.0, 1.0, 2.0, Gurobi::BINARY, "z")

    model.set_int(Gurobi::IntAttr::MODEL_SENSE, Gurobi::MAXIMIZE)
    model.update

    expr = Gurobi::LinExpr.new
    expr.addTerm(1.0, x); expr.addTerm(2.0, y); expr.addTerm(3.0, z);
    model.addConstr(expr, Gurobi::LESS_EQUAL, 4.0, "c0");

    expr = Gurobi::LinExpr.new
    expr.addTerm(1.0, x); expr.addTerm(1.0, y);
    model.addConstr(expr, Gurobi::GREATER_EQUAL, 1.0, "c1");

    model.optimize

    x.get_double(Gurobi::DoubleAttr::X).should  == 1.0
    y.get_double(Gurobi::DoubleAttr::X).should  == 0.0
    z.get_double(Gurobi::DoubleAttr::X).should  == 1.0

    model.get_double(Gurobi::DoubleAttr::OBJ_VAL).should  == 3.0
  end
end

require 'spec_helper'

describe 'Book 01-03 Turukametako' do
  it 'works' do
    grb_env = Gurobi::Env.new
    model   = Gurobi::Model.new(grb_env)
    
    x       = model.addVar(0, Gurobi::INFINITY,  0, Gurobi::INTEGER, 'x')
    y       = model.addVar(0, Gurobi::INFINITY,  1, Gurobi::INTEGER, 'y')
    z       = model.addVar(0, Gurobi::INFINITY,  1, Gurobi::INTEGER, 'z')
    model.update
    
    # x + y + z == 32
    l = Gurobi::LinExpr.new
    l.addTerm(1, x)
    l.addTerm(1, y)
    l.addTerm(1, z)
    model.addConstr(l, Gurobi::EQUAL, 32, 'c0')
    
    # 2*x + 4*y + 8*z == 80
    l = Gurobi::LinExpr.new
    l.addTerm(2, x)
    l.addTerm(4, y)
    l.addTerm(8, z)
    model.addConstr(l, Gurobi::EQUAL, 80, 'c1')
    
    model.optimize
    
    model.get_double(Gurobi::DoubleAttr::OBJ_VAL).should  == 4.0
    x.get_double(Gurobi::DoubleAttr::X).should  == 28.0
    y.get_double(Gurobi::DoubleAttr::X).should  == 2.0
    z.get_double(Gurobi::DoubleAttr::X).should  == 2.0
  end
end

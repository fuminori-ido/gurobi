describe Gurobi::LinExpr do
  before do
    @model  = Gurobi::Model.new(Gurobi::Env.new)
  end

  it 'new' do
    # new without arg
    exp = Gurobi::LinExpr.new

    # new from Var
    v = @model.addVar(0.0, 10.0, 3.14, Gurobi::CONTINUOUS, "v")
    exp.size.should == 0
    exp = Gurobi::LinExpr.new(v)
    exp.size.should == 1

    # copy
    exp2 = Gurobi::LinExpr.new(exp)
    exp2.size.should == 1

    # new with constant
    exp3 = Gurobi::LinExpr.new(3.14)

    # new with var & coeff
    exp4 = Gurobi::LinExpr.new(v, 1.414)
    exp4.getCoeff(0).should == 1.414
  end

  it 'getConstant' do
    exp = Gurobi::LinExpr.new
    exp.getConstant.should == 0

    exp = Gurobi::LinExpr.new(3.14)
    exp.getConstant.should == 3.14
  end

  it '+' do
    v   = @model.addVar(0.0, 10.0, 3.14, Gurobi::CONTINUOUS, "v")
    exp = Gurobi::LinExpr.new(v)
    (exp + v).class.should == Gurobi::LinExpr
  end

  it '<=' do
    v1   = @model.addVar(0.0, 10.0, 3.14, Gurobi::CONTINUOUS, "var1")
    exp1 = Gurobi::LinExpr.new
    exp1.addTerm(1, v1)

    v2  = @model.addVar(0.0, 10.0, 1.41, Gurobi::CONTINUOUS, "var2")
    exp2 = Gurobi::LinExpr.new
    exp2.addTerm(1, v2)

    (exp1 <= exp2).class.should == Gurobi::TempConstr
  end
end

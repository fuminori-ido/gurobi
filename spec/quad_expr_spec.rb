describe Gurobi::QuadExpr do
  before do
    @model  = Gurobi::Model.new(Gurobi::Env.new)
  end

  it 'new' do
    # new without arg
    exp = Gurobi::QuadExpr.new

    # new from Var
    v = @model.addVar(0.0, 10.0, 3.14, Gurobi::CONTINUOUS, "v")
    exp = Gurobi::QuadExpr.new(v)

    # new with constant
    exp3 = Gurobi::QuadExpr.new(3.14)

    # new with var & coeff
    v2 = @model.addVar(0.0, 10.0, 1.41, Gurobi::CONTINUOUS, "v2")
    exp4 = Gurobi::QuadExpr.new
    exp4.addTerm(2.718, v, v2)
    exp4.getCoeff(0).should == 2.718
  end

  it '<=' do
    v1   = @model.addVar(0.0, 10.0, 3.14, Gurobi::CONTINUOUS, "var1")
    exp1 = Gurobi::QuadExpr.new
    exp1.addTerm(1, v1)

    v2  = @model.addVar(0.0, 10.0, 1.41, Gurobi::CONTINUOUS, "var2")
    exp2 = Gurobi::QuadExpr.new
    exp2.addTerm(1, v2)

    (exp1 <= exp2).class.should == Gurobi::TempConstr
  end
end

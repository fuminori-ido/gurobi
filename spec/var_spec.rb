describe Gurobi::Var do
  before do
    @model  = Gurobi::Model.new(Gurobi::Env.new)
    @var    = @model.addVar(0.0, 10.0, 3.14, Gurobi::CONTINUOUS, "var")
    @model.update
  end

  it 'get_double' do
    @var.get_double(Gurobi::DoubleAttr::OBJ).should == 3.14
  end

  it 'get_string' do
    @var.get_string(Gurobi::StringAttr::VAR_NAME).should == "var"
  end

  it 'get_char' do
    @var.get_char(Gurobi::CharAttr::V_TYPE).should == Gurobi::CONTINUOUS
  end

  it '*' do
    @var2   = @model.addVar(0.0, 10.0, 1.414, Gurobi::CONTINUOUS, "var2")
    (@var * @var2).class.should == Gurobi::QuadExpr
    (@var * 3).class.should == Gurobi::LinExpr
    (@var * 4.3).class.should == Gurobi::LinExpr
  end

  it '+' do
    @var2   = @model.addVar(0.0, 10.0, 1.414, Gurobi::CONTINUOUS, "var2")
    (@var + @var2).class.should == Gurobi::LinExpr
    (@var + 3).class.should == Gurobi::LinExpr
    (@var + 4.3).class.should == Gurobi::LinExpr
  end
end

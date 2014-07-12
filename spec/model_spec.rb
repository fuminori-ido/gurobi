describe Gurobi::Model do
  before do
    @model = Gurobi::Model.new(Gurobi::Env.new)
  end

  it 'set_string' do
    @model.set_string(Gurobi::StringAttr::MODEL_NAME, "facility")
    @model.update
    @model.get_string(Gurobi::StringAttr::MODEL_NAME).should == "facility"
  end

  it 'addVars without type' do
    vars = @model.addVars(3)
    @model.update
    vars.size.should  == 3
    for v in vars do
      v.class.should == Gurobi::Var
      v.get_char(Gurobi::CharAttr::V_TYPE).should  == Gurobi::CONTINUOUS
    end
  end

  it 'addVars with type' do
    vars = @model.addVars(3, Gurobi::BINARY)
    @model.update
    vars.size.should  == 3
    for v in vars do
      v.class.should == Gurobi::Var
      v.get_char(Gurobi::CharAttr::V_TYPE).should  == Gurobi::BINARY
    end
  end
end

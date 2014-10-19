# issue reported to https://github.com/fuminori-ido/gurobi/issues/2
describe 'FixnumFloat' do
  before do
    @model  = Gurobi::Model.new(Gurobi::Env.new)
  end

  it 'var * Bignum' do
    var = @model.addVar(0, 1, 0, Gurobi::BINARY, "a")
    @model.update
    @model.setObjective(var * 9999999999999999999, Gurobi::MAXIMIZE)
    @model.update
    @model.optimize
    puts var.get_double(Gurobi::DoubleAttr::X)
  end

  it 'Bignum * var' do
    var = @model.addVar(0, 1, 0, Gurobi::BINARY, "a")
    @model.update
    @model.setObjective(9999999999999999999 * var, Gurobi::MAXIMIZE)
    @model.update
    @model.optimize
    puts var.get_double(Gurobi::DoubleAttr::X)
  end
end

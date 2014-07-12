require 'spec_helper'

describe Gurobi do
  it 'has a version number' do
    expect(Gurobi::VERSION).not_to be nil
  end

  it 'has C++ defined const' do
    Gurobi::OPTIMAL.should == 2
  end

  describe 'Env' do
    it 'new' do
      Gurobi::Env.new.should_not be_nil
    end
  end

  describe 'Model' do
    it 'new' do
      Gurobi::Model.new(Gurobi::Env.new).should_not be_nil
    end
  end

  describe 'Var' do
    it 'new' do
      m = Gurobi::Model.new(Gurobi::Env.new)
      m.addVar(0, Gurobi::INFINITY, 0, Gurobi::CONTINUOUS, 'x').class.
          should == Gurobi::Var
    end
  end
end

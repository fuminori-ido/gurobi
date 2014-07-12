module Gurobi
  class Model
    def addSOS(vars, weights, type)
      _addSOS(vars, weights, vars.size, type)
    end
  end
end

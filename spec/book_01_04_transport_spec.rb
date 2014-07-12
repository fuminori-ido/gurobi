# 1.4 Transport
#
# transport product from 3 factories to 5 customers
#
# mass[j]::   Maximum mass for each factory j
# cost[i,j]:: transport cost from factory j to customer i
# demand[i]:: demand of customer i
#
#      \ Cust.i | 1   2   3   4   5   |
#       \Demand | 80  270 250 160 180 |
# Fact.j \      | cost[i,j]           | mass[i]
# --------------+---------------------+-------------
# 1             | 4   5   6   8   10  | 500
# 2             | 6   4   3   5   8   | 500
# 3             | 9   7   4   3   4   | 500
#
# Objective: minimize total transport cost

describe 'Gurobi 01-04 Transport' do
  it 'works' do
    model   = Gurobi::Model.new(Gurobi::Env.new)
    
    I       = [1,2,3,4,5]               # customer
    J       = [1,2,3]                   # factory
    demand  = {1=>80,  2=>270, 3=>250, 4=>160, 5=>180}
    mass    = {1=>500, 2=>500, 3=>500}
    cost    = {
      [1,1]=>4,  [2,1]=>5,  [3,1]=>6,  [4,1]=>8,  [5,1]=>10,
      [1,2]=>6,  [2,2]=>4,  [3,2]=>3,  [4,2]=>5,  [5,2]=>8,
      [1,3]=>9,  [2,3]=>7,  [3,3]=>4,  [4,3]=>3,  [5,3]=>4,
    }
    
    x = {}
    for i in I
      for j in J
        x[i] = {} if !x[i]
        x[i][j] = model.addVar(0.0, Gurobi::INFINITY, cost[[i,j]], Gurobi::CONTINUOUS, "x(%d,%d)" % [i,j])
      end
    end
    model.update
    
    # customer demand constraint
    for i in I
      lhs = Gurobi::LinExpr.new
      for j in J
        lhs.addTerm(1.0, x[i][j])
      end

      model.addConstr(lhs, Gurobi::EQUAL, demand[i].to_f, "demand(%d)" % i)
    end
    
    # factory mass constraint
    for j in J
      lhs = Gurobi::LinExpr.new
      for i, in I
        lhs.addTerm(1.0, x[i][j])
      end
      model.addConstr(lhs, Gurobi::LESS_EQUAL, mass[j].to_f, name="capacity(%s)" % j)
    end
    
    # NOTE: #set_objective is not supported now (v0.01.00) so that
    # it is set at #addVar
    model.optimize

    model.get_int(Gurobi::IntAttr::STATUS).should  == Gurobi::OPTIMAL
    x[1][1].get_double(Gurobi::DoubleAttr::X).should  ==  80.0
    x[2][2].get_double(Gurobi::DoubleAttr::X).should  == 270.0
    x[3][2].get_double(Gurobi::DoubleAttr::X).should  == 230.0
    x[3][3].get_double(Gurobi::DoubleAttr::X).should  ==  20.0
    x[4][3].get_double(Gurobi::DoubleAttr::X).should  == 160.0
    x[5][3].get_double(Gurobi::DoubleAttr::X).should  == 180.0
  end
end

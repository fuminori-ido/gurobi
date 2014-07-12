=begin
   Facility location: a company currently ships its product from 5 plants
   to 4 warehouses. It is considering closing some plants to reduce
   costs. What plant(s) should the company close, in order to minimize
   transportation and fixed costs?

   Based on an example from Frontline Systems:
   http://www.solver.com/disfacility.htm
   Used with permission.
=end

describe 'Facility' do
  it 'works' do
    # Number of plants and warehouses
    n_plants      = 5
    n_warehouses  = 4

    # Warehouse demand in thousands of units
    demand = [15, 18, 14, 20]

    # Plant capacity in thousands of units
    capacity = [20, 22, 17, 19, 18]

    # Fixed costs for each plant
    fixed_costs = [12000, 15000, 17000, 13000, 16000]

    # Transportation costs per thousand units
    trans_costs = [
      [4000, 2000, 3000, 2500, 4500],
      [2500, 2600, 3400, 3000, 4000],
      [1200, 1800, 2600, 4100, 3000],
      [2200, 2600, 3100, 3700, 3200],
    ]

    # Model
    env   = Gurobi::Env.new
    model = Gurobi::Model.new(env)
    model.set_string(Gurobi::StringAttr::MODEL_NAME, "facility")

    # Plant open decision variables: open[p] == 1 if plant p is open.
    open = model.addVars(n_plants, Gurobi::BINARY)
    model.update
    n_plants.times do |i|
      open[i].set_double(Gurobi::DoubleAttr::OBJ, fixed_costs[i])
      open[i].set_string(Gurobi::StringAttr::VAR_NAME, "Open#{i}")
    end

    # Transportation decision variables: how much to transport from
    # a plant p to a warehouse w
    transport = []
    n_warehouses.times do |w|
      transport[w] = model.addVars(n_plants)
      model.update
      n_plants.times do |p|
        transport[w][p].set_double(Gurobi::DoubleAttr::OBJ, trans_costs[w][p])
        transport[w][p].set_string(Gurobi::StringAttr::VAR_NAME, "Trans#{p}.#{w}")
      end
    end

    # The objective is to minimize the total fixed and variable costs
    model.set_int(Gurobi::IntAttr::MODEL_SENSE, Gurobi::MINIMIZE)

    # Update model to integrate new variables
    model.update

    # Production constraints
    # Note that the right-hand limit sets the production to zero if
    # the plant is closed
    n_plants.times do |p|
      ptot = Gurobi::LinExpr.new
      n_warehouses.times do |w|
        ptot.add_to(transport[w][p])
      end
      model.addConstr(ptot <= capacity[p] * open[p], "Capacity#{p}")
    end

    # Demand constraints
    n_warehouses.times do |w|
      dtot = Gurobi::LinExpr.new
      n_plants.times do |p|
        dtot.add_to(transport[w][p])
      end
      model.addConstr(dtot == demand[w], "Demant#{w}")
    end

    # Guess at the starting point: close the plant with the highest
    # fixed costs; open all others

    # First, open all plants
    n_plants.times do |p|
      open[p].set_double(Gurobi::DoubleAttr::START, 1.0);
    end

    # Now close the plant with the highest fixed cost
    print "Initial guess:\n"
    maxFixed = -Gurobi::INFINITY
    n_plants.times do |p|
      if fixed_costs[p] > maxFixed
        maxFixed = fixed_costs[p]
      end
    end
    n_plants.times do |p|
      if fixed_costs[p] == maxFixed
        open[p].set_double(Gurobi::DoubleAttr::START, 0.0)
        print "Closing plant #{p}\n\n"
        p.should == 2
        break
      end
    end

    # Use barrier to solve root relaxation
    model.getEnv.set_int(Gurobi::IntParam::METHOD, Gurobi::METHOD_BARRIER)

    # Solve
    model.optimize

    # Print solution
    obj_val = model.get_double(Gurobi::DoubleAttr::OBJ_VAL)
    print "\nTOTAL COSTS: #{model.get_double(Gurobi::DoubleAttr::OBJ_VAL)}\n"
    obj_val.should == 210500
    print "SOLUTION:\n"
    n_plants.times do |p|
      if open[p].get_double(Gurobi::DoubleAttr::X) == 1.0
        print "Plant #{p} open:\n"
        n_warehouses.times do |w|
          if transport[w][p].get_double(Gurobi::DoubleAttr::X) > 0.0001
            print "  Transport ",
                transport[w][p].get_double(Gurobi::DoubleAttr::X),
                " units to warehouse #{w}\n"
          end
        end
      else
        print "Plant #{p} closed!\n"
      end
    end
    transport[2][0].get_double(Gurobi::DoubleAttr::X).should  == 14
    transport[3][0].get_double(Gurobi::DoubleAttr::X).should  == 6

    transport[0][1].get_double(Gurobi::DoubleAttr::X).should  == 14
    transport[3][1].get_double(Gurobi::DoubleAttr::X).should  == 8

    transport[0][3].get_double(Gurobi::DoubleAttr::X).should  == 1
    transport[1][3].get_double(Gurobi::DoubleAttr::X).should  == 18

    transport[3][4].get_double(Gurobi::DoubleAttr::X).should  == 6
  end
end

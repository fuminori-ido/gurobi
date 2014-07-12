=begin

Solve the classic diet model, showing how to add constraints
to an existing model.
=end

describe 'Diet' do
  it 'works' do
    categories    = ["calories",  "protein",      "fat",  "sodium"]
    min_nutrition = [1800,        91,             0,      0]
    max_nutrition = [2200,        Gurobi::INFINITY,  65,     1779]
    foods         = ["hamburger", "chicken", "hot dog", "fries",
                     "macaroni", "pizza", "salad", "milk", "ice cream"]
    cost          = [2.49, 2.89, 1.50, 1.89, 2.09, 1.99, 2.49, 0.89, 1.59]

    # Nutrition values for the foods
    nutrition_values = {
      "hamburger" => [410, 24, 26, 730],
      "chicken"   => [420, 32, 10, 1190],
      "hot dog"   => [560, 20, 32, 1800],
      "fries"     => [380, 4, 19, 270],
      "macaroni"  => [320, 12, 10, 930],
      "pizza"     => [320, 15, 12, 820],
      "salad"     => [320, 31, 12, 1230],
      "milk"      => [100, 8, 2.5, 125],
      "ice cream" => [330, 8, 10, 180]}

    model = Gurobi::Model.new(Gurobi::Env.new)
   #model.set(Gurobi::StringAttr::MODEL_NAME, "diet")

    nutrition = {}
    categories.each_with_index do |category, i|
      nutrition[category] = model.addVar(
          min_nutrition[i],
          max_nutrition[i],
          0,
          Gurobi::CONTINUOUS,
          category)
    end

    buy = {}
    foods.each_with_index do |food, j|
      buy[food] = model.addVar(
          0,
          Gurobi::INFINITY,
          cost[j],
          Gurobi::CONTINUOUS,
          food)
    end

    model.set_int(Gurobi::IntAttr::MODEL_SENSE, Gurobi::MINIMIZE)
    model.update

    # Nutrition constraints
    categories.each_with_index do |category, i|
      ntot = Gurobi::LinExpr.new
      foods.each_with_index do |food, j|
        ntot.addTerm(nutrition_values[food][i], buy[food])
      end
      model.addConstr(ntot, Gurobi::EQUAL, nutrition[category], category)
    end

    model.optimize
    model.get_int(Gurobi::IntAttr::MODEL_SENSE).should == Gurobi::MINIMIZE
    model.get_int(Gurobi::IntAttr::STATUS).should == Gurobi::OPTIMAL

    cost = model.get_double(Gurobi::DoubleAttr::OBJ_VAL)
    check_range(cost, 11.828, 11.829)
    check_range(buy['hamburger'].get_double(Gurobi::DoubleAttr::X),       0.604,    0.605)
    check_range(buy['milk'].get_double(Gurobi::DoubleAttr::X),            6.970,    6.971)
    check_range(buy['ice cream'].get_double(Gurobi::DoubleAttr::X),       2.59,     2.60)
    check_range(nutrition['calories'].get_double(Gurobi::DoubleAttr::X),  1800.0,   1800.0)
    check_range(nutrition['protein'].get_double(Gurobi::DoubleAttr::X),   91.0,     91.0)
    check_range(nutrition['fat'].get_double(Gurobi::DoubleAttr::X),       59.0,     59.1)
    check_range(nutrition['sodium'].get_double(Gurobi::DoubleAttr::X),    1779.0,   1779.0)

    lhs = Gurobi::LinExpr.new
    lhs.addTerm(1.0, buy['milk']);
    lhs.addTerm(1.0, buy['ice cream']);
    model.addConstr(lhs, Gurobi::LESS_EQUAL, 6.0, "limit_dairy")
    model.optimize
    model.get_int(Gurobi::IntAttr::STATUS).should == Gurobi::INFEASIBLE
  end
end

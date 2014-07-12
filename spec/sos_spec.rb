=begin
   This example creates a very simple Special Ordered Set (SOS) model.
   The model consists of 3 continuous variables, no linear constraints,
   and a pair of SOS constraints of type 1.
=end

describe 'SOS' do
  it 'works' do
    env = Gurobi::Env.new

    model = Gurobi::Model.new(env)

    # Create variables

    ub    = [1, 1, 2]
    obj   = [-2, -1, -1]
    names = ["x0", "x1", "x2"]

    x = model.addVars(nil, ub, obj, nil, names, 3)

    # Integrate new variables

    model.update

    # Add first SOS1: x0=0 or x1=0

    sosv1   = [x[0], x[1]]
    soswt1  = [1, 2]

    model.addSOS(sosv1, soswt1, Gurobi::SOS_TYPE1)

    # Add second SOS1: x0=0 or x2=0

    sosv2   = [x[0], x[2]]
    soswt2  = [1, 2]

    model.addSOS(sosv2, soswt2, Gurobi::SOS_TYPE1)

    # Optimize model

    model.optimize

    3.times do |i|
      print x[i].get_string(Gurobi::StringAttr::VAR_NAME), " ",
            x[i].get_double(Gurobi::DoubleAttr::X), "\n"
    end
    x[0].get_double(Gurobi::DoubleAttr::X).should == 0.0
    x[1].get_double(Gurobi::DoubleAttr::X).should == 1.0
    x[2].get_double(Gurobi::DoubleAttr::X).should == 2.0

    print "Obj: ", model.get_double(Gurobi::DoubleAttr::OBJ_VAL), "\n"
    model.get_double(Gurobi::DoubleAttr::OBJ_VAL).should  == -3.0
  end
end

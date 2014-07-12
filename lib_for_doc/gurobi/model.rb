module Gurobi
  # wrapper of C++ GRBModel class.
  class Model
    # model constructor
    #
    # @overload initialize(env)
    #   initialize from env
    # @overload initialize(env, filename)
    #   Read a model from a file.
    #   Note that the type of the file is encoded in the file name suffix.
    #   Valid suffixes are .mps, .rew, .lp, .rlp, or .ilp.
    #   The files can be compressed, so additional suffixes of .zip, .gz,
    #   .bz2 or .7z are accepted.
    # @overload initialize(model)
    #   initialize by copying Model
    #
    # @param env [Env] Gurobi environment
    # @param filename [String] gurobi model file
    # @param model [Model] source model
    def initialize; end

    # Add a single linear constraint to a model. Multiple signatures are available.
    #
    # @overload addConstr(tc, name="")
    # @overload addConstr(lhs, sence, rhs_lin_expr, name="")
    # @overload addConstr(lhs, sence, rhs_var, name="")
    # @overload addConstr(lhs, sence, rhs_val, name="")
    # 
    # @param tc [TempConstr] Temporary constraint object, created using an overloaded comparison operator.
    # @param name [String] Name for new constraint (optional).
    # @param lhs [LinExpr] Left-hand side expression for new linear constraint.
    # @param sense [Char] Sense for new linear constraint (Gurobi::LESS_EQUAL, Gurobi::EQUAL, or Gurobi::GREATER_EQUAL).
    # @param rhs_lin_expr [LinExpr] Right-hand side expression for new linear constraint.
    # @param rhs_lin_var [Var] Right-hand side variable for new linear constraint.
    # @param rhs_lin_val [Float or Fixnum] Right-hand side value for new linear constraint.
    # @return [Constr] New constraint object.
    def addConstr; end

    # Add a quadratic constraint to a model. Multiple signatures are available.
    # @overload addQConstr(tc, name ="")
    # @overload addQConstr(lhs, sense, rhs_quad_expr, name ="")
    # @overload addQConstr(lhs, sense, rhs_var, name ="")
    #
    # @param tc [TempConstr] Temporary constraint object, created using an overloaded comparison operator.
    # @param name [String] Name for new constraint (optional).
    # @param lhs [QuadExpr] Left-hand side expression for new quadratic constraint.
    # @param sense [Char] Sense for new linear constraint (Gurobi::LESS_EQUAL, Gurobi::EQUAL, or Gurobi::GREATER_EQUAL).
    # @param rhs_quad_expr [QuadExpr] Right-hand side expression for new quadratic constraint.
    # @param rhs_var [Var] Right-hand side variable for new quadratic constraint.
    # @return [QConstr] New quadratic constraint object.
    def addQConstr; end

    # Modifies the GRBModel object to create a feasibility relaxation. Note that you need to call optimize on the result to compute the actual relaxed solution.
    #
    # @param relaxobjtype [int] The cost function used when finding the minimum cost relaxation (Gurobi::FEASRELAX_LINEAR, Gurobi::FEASRELAX_QUADRATIC, or Gurobi::FEASRELAX_CARDINALITY).
    # @param minrelax [bool]    The type of feasibility relaxation to perform.
    # @param vrelax [bool]      Indicates whether variable bounds can be relaxed (with a cost of 1.0 for any violations.
    # @param crelax [bool]      Indicates whether linear constraints can be relaxed (with a cost of 1.0 for any violations.
    # @return [Float] Zero if minrelax is false. If minrelax is true, the return value is the objective value for the relaxation performed. If the value is less than 0, it indicates that the method failed to create the feasibility relaxation.
    def feasRelax(relaxobjtype, minrelax, vrelax, crelax); end

    # Add an Sos constraint to the model.
    #
    # @param vars     [Array[Var]] Array of variables that participate in the Sos constraint.
    # @param weights  [Array[Float]] Weights for the variables in the Sos constraint.
    # @param type     [int]   Sos type (can be Gurobi::SOS_TYPE1 or Gurobi::SOS_TYPE2)
    # @return [Sos] New Sos constraint.
    #
    # === BUG
    # memory leak exists in vars and weights
    def addSOS(vars, weights, type); end

    # Add a single decision variable to a model.
    #
    # @overload addVar(lb, ub, obj, type, name)
    # @overload addVar(lb, ub, obj, type, col, name)
    # @overload addVar(lb, ub, obj, type, numnz, constrs, coeffs, name)
    #
    # @param lb       [Float]         Lower bound for new variable.
    # @param ub       [Float]         Upper bound for new variable.
    # @param obj      [Float]         Objective coefficient for new variable.
    # @param type     [Char]          Variable type for new variable Gurobi::CONTINUOUS, Gurobi::BINARY, Gurobi::INTEGER, Gurobi::SEMICONT, or Gurobi::SEMIINT)
    # @param name     [String]        Name for new variable.  Unlike C++ name parameter, it is not optional.
    # @param col      [Column]        Column object for specifying a set of constraints to which new variable belongs.
    # @param numnz    [int]           Number of constraints in which this new variable participates.
    # @param constrs  [Array[Constr]] Array of constraints in which the variable participates.
    # @param coeffs   [Array[Float]]  Array of coefficients for each constraint in which the variable participates.
    # @return [Var] New variable object.
    def addVar; end

    # Add new decision variables to a model.
    #
    # @overload addVars(count, type=Gurobi::CONTINUOUS)
    # @overload addVars(lb, ub, obj, type, names, count)
    # @param lb [Array[Float]]      Lower bounds for new variables. Can be nil, in which case the variables get lower bounds of 0.0.
    # @param ub [Array[Float]]      Upper bounds for new variables. Can be nil, in which case the variables get infinite upper bounds.
    # @param obj [Array[Float]]     Objective coefficients for new variables. Can be nil, in which case the variables get objective coefficients of 0.0
    # @param type [Array[Char]]     Variable types for new variables.  Can be nil, in which case the variables are assumed to be continuous.
    # @param names [Array[String]]  Names for new variables. Can be nil, in which case all variables are given default names.
    # @param count [int]            The number of variables to add.
    # @return [Array[Var]] Array of new variable objects.
    #
    # === BUG
    # memory leak exists.
    def addVars; end

    # Query the value of a double-valued model attribute.
    #
    # @param attr [DoubleAttr] The attribute being queried.
    # @return [Float] The current value of the requested attribute.
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::get().  Since C++ GRB_DoubleAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'get'
    # to 'get_double'
    def get_double(attr); end

    # Query the value of a int-valued model attribute.
    #
    # @param attr [IntAttr] The attribute being queried.
    # @return [int] The current value of the requested attribute.
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::get().  Since C++ GRB_IntAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'get'
    # to 'get_int'
    def get_int(attr); end

    # Query the value of a string-valued model attribute.
    #
    # @param attr [StringAttr] The attribute being queried.
    # @return [string] The current value of the requested attribute.
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::get().  Since C++ GRB_StringAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'get'
    # to 'get_string'
    def get_string(attr); end

    # Retrieve an array of all constraints in the model.
    #
    # @return [Array[Constr]] An array of all constraints in the model.
    #
    # === BUG
    # freeing GRBConstr C array is not considered.
    def getConstrs; end

    # Query the environment associated with the model.
    #
    # Note that each model makes its own copy of the environment when it is created. To change parameters for a model, for example, you should use this method to obtain the appropriate environment object.
    #
    # @return [Env] The environment for the model.
    def getEnv; end

    # Use this method to retrieve the results of a previous tune call.
    #
    # @param n [int] The index of the tuning result to retrieve. The best result is available as index 0. The number of stored results is available in attribute TuneResultCount.
    # @return [nil]
    def getTuneResult(n); end

    # Retrieve an array of all variables in the model.
    #
    # @return [Array[Var]] An array of all variables in the model.
    #
    # === BUG
    # freeing GRBVar C array is not considered.
    def getVars; end

    # Create the fixed model associated with a MIP model.
    #
    # The MIP model must have a solution loaded
    # (e.g., after a call to the optimize method).
    # In the fixed model, each integer variable is fixed to the value that
    # variable takes in the MIP solution.
    #
    # @return [Model] Fixed model associated with calling object.
    def fixedModel; end

    # Optimize the model.
    #
    # @return [nil]
    def optimize; end

    # Compute an Irreducible Inconsistent Subsystem (IIS).
    #
    # @return [nil]
    def computeIIS; end

    # Remove a variable, constraint, or SOS from a model.
    #
    # @overload remove(constr)
    # @overload remove(qconstr)
    # @overload remove(sos)
    # @overload remove(var)
    #
    # @param constr [Constr] The linear constraint to remove.
    # @param qconstr [QConstr] The quadratic constraint to remove.
    # @param sos [Sos] The SOS constraint to remove.
    # @param var [Var] The variable to remove.
    # @return [nil]
    def remove; end

    # Reset the model to an unsolved state, discarding any previously computed solution information.
    # @return [nil]
    def reset; end

    # Set the value of a int-valued model attribute.
    #
    # @param attr [IntAttr] The attribute being modified.
    # @param val [int] The desired new value for the attribute.
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::set().  Since C++ GRB_IntAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'set'
    # to 'set_int'
    def set_int(attr, val); end

    # Set the value of a string-valued model attribute.
    #
    # @param attr [IntAttr] The attribute being modified.
    # @param val [string] The desired new value for the attribute.
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::set().  Since C++ GRB_StringAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'set'
    # to 'set_string'
    def set_string(attr, val); end

    # Set the model objective equal to a linear or quadratic expression.
    #
    # @overload setObjective(linexpr, sense=0)
    # @overload setObjective(quadexpr, sense=0)
    #
    # @param linexpr [LinExpr] New linear model objective.
    # @param quadexpr [QuadExpr] New quadratic model objective.
    # @param sense [int]  Optimization sense (Gurobi::MINIMIZE for minimization, Gurobi::MAXIMIZE for maximization). Omit this argument to use the ModelSense attribute value.
    # @return [nil]
    def setObjective; end

    # Perform an automated search for parameter settings that improve performance.
    #
    # See C++ explanation for more detail.
    def tune; end

    # Process any pending model modifications.
    #
    # @return [nil]
    def update; end

    # This method is the general entry point for writing model data to a file.
    #
    # @param filename [string] Name of the file to write. The file type is encoded in the file name suffix.  See C++ explanation for more detail.
    # @return [nil]
    def write(filename); end
  end
end

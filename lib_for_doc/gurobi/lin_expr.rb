module Gurobi
  # wrapper of C++ GRBLinExpr class.
  class LinExpr < Expr
    # Linear expression constructor.
    #
    # Create a constant expression or an expression with one term.
    #
    # @overload initialize(const = 0.0)
    #   @param const [Float] Constant value for expression.
    # @overload initialize(lin_expr)
    #   @param lin_expr [LinExpr] initial lin_expr
    # @overload initialize(var, coeff = 1.0)
    #   Create an expression with one term.
    #   @param var [Var] Variable for expression term.
    #   @param coeff [Float] Coefficient for expression term.
    def initialize; end

    # Add a new term into a linear expression.
    #
    # @param coeff [Fixnum/Float] Coefficient for new term.
    # @param var [Var] Variable for new term.
    #
    # @return [nil]
    def addTerm(coeff, var); end

    # Add an expression/var into the invoking expression.
    #
    # @overload add_to(lin_expr)
    #   Add an expression into the invoking expression.
    #   @param lin_exp [LinExpr] Expression to add.
    # @overload add_to(var)
    #   Add an var into the invoking expression.
    #   @param var [Var] Var to add.
    #
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBLinExpr::operator+=()
    def add_to; end

    # Subtract an expression/var from the invoking expression.
    #
    # @overload sub_from(lin_expr)
    #   Subtract an expression from the invoking expression.
    #   @param lin_exp [LinExpr] Expression to subtract.
    # @overload sub_from(var)
    #   Subtract an var from the invoking expression.
    #   @param var [Var] Var to subtract.
    #
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBLinExpr::operator-=()
    def sub_from; end

    # Retrieve the constant term from a linear expression.
    #
    # @return [Float] Constant from expression.
    def getConstant; end

    # Retrieve the coefficient from a single term of the expression.
    #
    # @param i [Fixnum] Index for coefficient of interest.
    # @return [Float] Coefficient for the term at index _i_ in the expression.
    def getCoeff(i); end

    # Retrieve the variable object from a single term of the expression.
    #
    # @param i [Fixnum] Index for term of interest.
    # @return [Var] Variable for the term at index _i_ in the expression.
    def getVar(i); end

    # Retrieve the number of terms in the linear expression (not including the constant).
    #
    # @return [Fixnum] Number of terms in the expression.
    def size; end

    # Add one var/expression into another, producing a result expression.
    #
    # @overload +(var)
    #   Add one var into another, producing a result expression.
    #   @param var [Var] Expression to add.
    #
    # @overload +(lin_expr)
    #   Add one expression into another, producing a result expression.
    #   @param lin_exprar [LinExpr] Expression to add.
    #
    # @return [LinExpr] Expression object which is equal the sum of the invoking expression and the argument expression.
    def +(rhs); end

    # Create an inequality constraint
    #
    # @overload <=(val)
    #   @param val [Fixnum/Float]
    # @overload <=(var)
    #   (not yet implemented)
    #   @param var [Var]
    # @overload <=(lin_expr)
    #   @param lin_expr [LinExpr]
    #
    # @return [TempConstr] A constraint. The result is typically immediately passed to Model::addConstr or Model::addQConstr.
    def <=(rhs); end

    # Create an inequality constraint
    #
    # @overload >=(val)
    #   @param val [Fixnum/Float]
    # @overload >=(var)
    #   (not yet implemented)
    #   @param var [Var]
    # @overload >=(lin_expr)
    #   @param lin_expr [LinExpr]
    #
    # @return [TempConstr] A constraint. The result is typically immediately passed to Model::addConstr or Model::addQConstr.
    def >=(rhs); end

    # Create an equality constraint
    #
    # @overload ==(val)
    #   @param val [Fixnum/Float]
    # @overload ==(var)
    #   @param var [Var]
    # @overload ==(lin_expr)
    #   @param lin_expr [LinExpr]
    #
    # @return [TempConstr] A constraint. The result is typically immediately passed to Model::addConstr or Model::addQConstr.
    def ==(rhs); end
  end
end
  
module Gurobi
  # wrapper of C++ GRBQuadExpr class.
  class QuadExpr < Expr
    # Quadratic expression constructor.
    #
    # Create a constant expression or an expression with one term.
    #
    # @overload initialize(const = 0.0)
    #   @param const [Float] Constant value for expression.
    # @overload initialize(quad_expr)
    #   @param quad_expr [QuadExpr] initial quad_expr
    # @overload initialize(var, coeff = 1.0)
    #   Create an expression with one term.
    #   @param var [Var] Variable for expression term.
    #   @param coeff [Float] Coefficient for expression term.
    def initialize; end

    # Add a new term into a quadratic expression.
    #
    # @overload addTerm(coeff, var)
    #   Add a new linear term into a quadratic expression.
    #   @param coeff [Float] Coefficient for new linear term.
    #   @param var [Var] Variable for new linear term.
    # @overload addTerm(coeff, var1, var2)
    #   Add a new quadratic term into a quadratic expression.
    #   @param coeff [Float] Coefficient for new quadratic term.
    #   @param var1 [Var] Variable for new quadratic term.
    #   @param var2 [Var] Variable for new quadratic term.
    #
    # @return [nil]
    def addTerm(coeff, var); end

    # Add an expression/var into the invoking expression.
    #
    # @overload add_to(quad_expr)
    #   Add an expression into the invoking expression.
    #   @param quad_exp [QuadExpr] Expression to add.
    # @overload add_to(var)
    #   Add an var into the invoking expression.
    #   @param var [Var] Var to add.
    #
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBQuadExpr::operator+=()
    def add_to; end

    # Retrieve the coefficient from a single quadratic term of the
    # quadratic expression.
    #
    # @param i [Fixnum] Index for coefficient of interest.
    # @return [Float] Coefficient for the term at index _i_ in the expression.
    def getCoeff(i); end

    # Retrieve the first variable object associated with a single quadratic
    # term from the expression.
    #
    # @param i [Fixnum] Index for term of interest.
    # @return [Var] First variable for the quadratic term at index _i_ in the quadratic expression.
    def getVar1(i); end

    # Retrieve the second variable object associated with a single quadratic
    # term from the expression.
    #
    # @param i [Fixnum] Index for term of interest.
    # @return [Var] Second variable for the quadratic term at index _i_ in the quadratic expression.
    def getVar2(i); end

    # Retrieve the number of terms in the quadratic expression.
    #
    # @return [Fixnum] Number of terms in the expression.
    def size; end

    # Add one var/expression into another, producing a result expression.
    #
    # @overload +(lin_expr)
    # @overload +(quad_expr)
    # @param lin_expr [LinExpr] Expression to add.
    # @param quad_exprar [QuadExpr] Expression to add.
    # @return [QuadExpr] Expression object which is equal the sum of the invoking expression and the argument expression.
    def +; end

    # Create an inequality constraint
    #
    # @param quad_expr [QuadExpr]
    # @return [TempConstr] A constraint. The result is typically immediately passed to Model::addConstr or Model::addQConstr.
    def <=(quad_expr); end

    # Create an equality constraint
    #
    # @overload ==(val)
    #   @param val [Fixnum/Float]
    # @overload ==(quad_expr)
    #   @param quad_expr [QuadExpr]
    #
    # @return [TempConstr] A constraint. The result is typically immediately passed to Model::addConstr or Model::addQConstr.
    def ==; end
    end
end

module Gurobi
  # wrapper of C++ GRBVar class.
  class Var
    # Query the value of a double-valued variable attribute.
    #
    # @param attr [DoubleAttr] The attribute being queried.
    # @return [Float] The current value of the requested attribute.
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBVar::get().  Since C++ GRB_DoubleAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'get'
    # to 'get_double'
    def get_double(attr); end

    # Query the value of a string-valued variable attribute.
    #
    # @param attr [StringAttr] The attribute being queried.
    # @return [string] The current value of the requested attribute.
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBVar::get().  Since C++ GRB_StringAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'get'
    # to 'get_string'
    def get_string(attr); end

    # Query the value of a char-valued variable attribute.
    #
    # @param attr [CharAttr] The attribute being queried.
    # @return [Fixnum] The current value of the requested attribute.
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBVar::get().  Since C++ GRB_CharAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'get'
    # to 'get_char'
    def get_char(attr); end

    # Set the value of a double-valued model attribute.
    #
    # @param attr [DoubleAttr] The attribute being modified.
    # @param val [Float] The desired new value for the attribute.
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::set().  Since C++ GRB_DoubleAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'set'
    # to 'set_double'
    def set_double(attr, val); end

    # Set the value of a int-valued model attribute.
    #
    # @param attr [IntAttr] The attribute being modified.
    # @param val [Float] The desired new value for the attribute.
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::set().  Since C++ GRB_IntAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'set'
    # to 'set_int'
    def set_int(attr, val); end

    # Set the value of a char-valued model attribute.
    #
    # @param attr [CharAttr] The attribute being modified.
    # @param val [Float] The desired new value for the attribute.
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::set().  Since C++ GRB_CharAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'set'
    # to 'set_char'
    def set_char(attr, val); end

    # multiply var and const/var/lin_expr
    #
    # @overload *(val)
    #   @param val [Float] Constant multiplier.
    #   @return [LinExpr] Expression that represents the result of multiplying the variable by a constant.
    # @overload *(var)
    #   @param var [Var] Second variable.
    #   @return [QuadExpr] Expression that represents the result of multiplying the argument variables.
    # @overload *(lin_expr)
    #   @param lin_expr [LinExpr] Expression.
    #   @return [QuadExpr] Expression that represents the result of multiplying the expression by a variable.
    def *(x); end

    # add var and const/var/lin_expr
    #
    # @overload +(val)
    #   @param val [Float/Fixnum] Constant to add.
    #   @return [LinExpr] Sum expression.
    # @overload +(var)
    #   @param var [Var] Second variable to be added.
    #   @return [LinExpr] Sum expression.
    # @overload +(lin_expr)
    #   @param lin_expr [LinExpr] Linear expression to add.
    #   @return [LinExpr] Sum expression.
    def +(x); end
  end
end

module Gurobi
  class Env
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

    # Query the value of a int-valued variable attribute.
    #
    # @param attr [IntAttr] The attribute being queried.
    # @return [Fixnum] The current value of the requested attribute.
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBVar::get().  Since C++ GRB_IntAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'get'
    # to 'get_int'
    def get_int(attr); end

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

    # Set the value of a double-valued model attribute.
    #
    # @param attr [DoubleAttr] The attribute being modified.
    # @param val [double] The desired new value for the attribute.
    # @return [nil]
    #
    # === Ruby Implementation Note
    # This method wraps C++ GRBModel::set().  Since C++ GRB_DoubleAttr
    # and other enum is converted to integer constant in ruby, 
    # there is no way to identify type of attr at runtime,
    # so that method name is changed from 'set'
    # to 'set_double'
    def set_double(attr, val); end
  end
end

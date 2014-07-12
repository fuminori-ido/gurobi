module Gurobi
  # wrapper of C++ GRBConstr class.
  class Constr
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
  end
end

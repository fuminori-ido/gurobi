module Gurobi
  # wrapper of C++ GRBColumn class.
  class Column
    # Create an empty column.
    def initialize; end

    # Add a single term into a column.
    #
    # @param coeff [Fixnum/Float] Coefficient for new term.
    # @param constr [Constr] Constraint for new term.
    #
    # @return [nil]
    def addTerm(coeff, constr); end
  end
end

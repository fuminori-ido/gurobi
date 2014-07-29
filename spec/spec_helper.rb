$LOAD_PATH.unshift File.expand_path('../../ext', __FILE__)
$LOAD_PATH.unshift File.expand_path('../../lib', __FILE__)
require 'gurobi'

def check_range(val, lower, upper)
  val.should >= lower
  val.should <= upper
end

def model_file(name)
  File.join(ENV['GUROBI_HOME'], 'examples/data', name)
end

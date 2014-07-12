require "mkmf"

# set Gurobi header & lib
dir_config 'grb',
    ENV['GUROBI_HOME'] + '/include',
    ENV['GUROBI_HOME'] + '/lib'
abort 'Can not find Gurobi 5.5 C library'   if !have_library('gurobi55')
abort 'Can not find Gurobi 5.5 C++ library' if !have_library('gurobi_c++')

create_makefile("gurobi/gurobi")

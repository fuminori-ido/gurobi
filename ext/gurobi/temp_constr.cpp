#include "ruby_gurobi.h"

VALUE rb_cGurobiTempConstr;

GRBTempConstr*
gurobi_temp_constr_get(VALUE self)
{
  GRBTempConstr* p;
  Data_Get_Struct(self, GRBTempConstr, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_temp_constr_free(GRBTempConstr* temp_constr)
{
  temp_constr->~GRBTempConstr();
  ruby_xfree(temp_constr);
}

/*!
copy C++ GRBTempConstr to heap and wrap with Ruby Gurobi::TempConstr
*/
VALUE
gurobi_wrap_temp_constr(const GRBTempConstr& x)
{
  GRBTempConstr* duped = new GRBTempConstr(x);
  return Data_Wrap_Struct(rb_cGurobiTempConstr, NULL, gurobi_temp_constr_free, duped);
}

void
init_gurobi_temp_constr()
{
  rb_cGurobiTempConstr = rb_define_class_under(rb_mGurobi, "TempConstr", rb_cObject);
}

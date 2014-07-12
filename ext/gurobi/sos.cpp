#include "ruby_gurobi.h"

VALUE rb_cGurobiSos;

GRBSOS*
gurobi_sos_get(VALUE self)
{
  GRBSOS* p;
  Data_Get_Struct(self, GRBSOS, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_sos_free(GRBSOS* sos)
{
  sos->~GRBSOS();
  ruby_xfree(sos);
}

/*!
copy C++ GRBSOS to heap and wrap with Ruby Gurobi::SOS
*/
VALUE
gurobi_wrap_sos(const GRBSOS& x)
{
  GRBSOS* duped = new GRBSOS(x);
  return Data_Wrap_Struct(rb_cGurobiSos, NULL, gurobi_sos_free, duped);
}

void
init_gurobi_sos()
{
  rb_cGurobiSos = rb_define_class_under(rb_mGurobi, "Sos", rb_cObject);
}

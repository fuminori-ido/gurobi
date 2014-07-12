/*!
\file Gurobi Var class binding for Ruby 'Gurobi::Var'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiQConstr;

GRBQConstr*
gurobi_qconstr_get(VALUE self)
{
  GRBQConstr* p;
  Data_Get_Struct(self, GRBQConstr, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_qconstr_free(GRBQConstr* qconstr)
{
  qconstr->~GRBQConstr();
  ruby_xfree(qconstr);
}

/*!
\brief  used for Model::getQConstrs()
*/
void
gurobi_qconstr_free2(GRBQConstr* qconstr)
{
  qconstr->~GRBQConstr();
}

/*!
copy C++ GRBQConstr to heap and wrap with Ruby Gurobi::QConstr
*/
VALUE
gurobi_wrap_qconstr(const GRBQConstr& x)
{
  GRBQConstr* duped = new GRBQConstr(x);
  return Data_Wrap_Struct(rb_cGurobiQConstr, NULL, gurobi_qconstr_free, duped);
}

static VALUE
qconstr_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_qconstr_free, ALLOC(GRBQConstr));
}

static VALUE
qconstr_init(VALUE self)
{
  GRBQConstr* p     = gurobi_qconstr_get(self);
  new(p) GRBQConstr();
  return Qnil;
}

static VALUE
qconstr_get_string(VALUE self, VALUE attr)
{
  GRBQConstr*      p     = gurobi_qconstr_get(self);
  GRB_StringAttr  _attr = static_cast<GRB_StringAttr>(NUM2INT(attr));
  return rb_str_new2(p->get(_attr).c_str());
}

/*
NOTE:

1. actually it doesn't return char, but integer converted from char.
*/
static VALUE
qconstr_get_char(VALUE self, VALUE attr)
{
  GRBQConstr*     p      = gurobi_qconstr_get(self);
  GRB_CharAttr  _attr   = static_cast<GRB_CharAttr>(NUM2INT(attr));
  return INT2FIX(static_cast<char>(p->get(_attr)));
}

void
init_gurobi_qconstr()
{
  rb_cGurobiQConstr = rb_define_class_under(rb_mGurobi, "QConstr", rb_cObject);
/*
  rb_define_method(rb_cGurobiQConstr, "get_string",   method_cast(qconstr_get_string), 1);
  rb_define_method(rb_cGurobiQConstr, "get_char",     method_cast(qconstr_get_char), 1);
*/
}

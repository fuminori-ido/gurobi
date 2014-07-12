/*!
\file Gurobi Var class binding for Ruby 'Gurobi::Var'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiConstr;

GRBConstr*
gurobi_constr_get(VALUE self)
{
  GRBConstr* p;
  Data_Get_Struct(self, GRBConstr, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_constr_free(GRBConstr* constr)
{
  constr->~GRBConstr();
  ruby_xfree(constr);
}

/*!
\brief  used for Model::getConstrs()
*/
void
gurobi_constr_free2(GRBConstr* constr)
{
  constr->~GRBConstr();
}

/*!
copy C++ GRBConstr to heap and wrap with Ruby Gurobi::Constr
*/
VALUE
gurobi_wrap_constr(const GRBConstr& x)
{
  GRBConstr* duped = new GRBConstr(x);
  return Data_Wrap_Struct(rb_cGurobiConstr, NULL, gurobi_constr_free, duped);
}

static VALUE
constr_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_constr_free, ALLOC(GRBConstr));
}

static VALUE
constr_init(VALUE self)
{
  GRBConstr* p     = gurobi_constr_get(self);
  new(p) GRBConstr();
  return Qnil;
}

static VALUE
constr_get_string(VALUE self, VALUE attr)
{
  GRBConstr*      p     = gurobi_constr_get(self);
  GRB_StringAttr  _attr = static_cast<GRB_StringAttr>(NUM2INT(attr));
  return rb_str_new2(p->get(_attr).c_str());
}

/*
*/
static VALUE
constr_get_int(VALUE self, VALUE attr)
{
  GRBConstr*    p       = gurobi_constr_get(self);
  GRB_IntAttr   _attr   = static_cast<GRB_IntAttr>(NUM2INT(attr));
  return INT2FIX(static_cast<int>(p->get(_attr)));
}

/*
NOTE:

1. actually it doesn't return char, but integer converted from char.
*/
static VALUE
constr_get_char(VALUE self, VALUE attr)
{
  GRBConstr*     p      = gurobi_constr_get(self);
  GRB_CharAttr  _attr   = static_cast<GRB_CharAttr>(NUM2INT(attr));
  return INT2FIX(static_cast<char>(p->get(_attr)));
}

void
init_gurobi_constr()
{
  rb_cGurobiConstr = rb_define_class_under(rb_mGurobi, "Constr", rb_cObject);
  rb_define_method(rb_cGurobiConstr, "get_string",  method_cast(constr_get_string), 1);
  rb_define_method(rb_cGurobiConstr, "get_int",     method_cast(constr_get_int), 1);
  rb_define_method(rb_cGurobiConstr, "get_char",    method_cast(constr_get_char), 1);
}

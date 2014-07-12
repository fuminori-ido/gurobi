/*!
\file Gurobi Column class binding for Ruby 'Gurobi::Column'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiColumn;

GRBColumn*
gurobi_column_get(VALUE self)
{
  GRBColumn* p;
  Data_Get_Struct(self, GRBColumn, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_column_free(GRBColumn* column)
{
  column->~GRBColumn();
  ruby_xfree(column);
}

/*!
copy C++ GRBColumn to heap and wrap with Ruby Gurobi::Column
*/
VALUE
gurobi_wrap_column(const GRBColumn& x)
{
  GRBColumn* duped = new GRBColumn(x);
  return Data_Wrap_Struct(rb_cGurobiColumn, NULL, gurobi_column_free, duped);
}

static VALUE
column_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_column_free, ALLOC(GRBColumn));
}

/*!
Column()
*/
static VALUE
column_init(VALUE self)
{
  GRBColumn* p     = gurobi_column_get(self);
  new(p) GRBColumn;
  return Qnil;
}

static VALUE
column_add_term(VALUE self, VALUE coeff, VALUE constr)
{
  GRBColumn*  p       = gurobi_column_get(self);
  GRBConstr*  _constr = gurobi_constr_get(constr);
  double      _coeff;
  switch( TYPE(coeff) ){
  case T_FIXNUM:
    _coeff = FIX2INT(coeff);
    break;
  case T_FLOAT:
    _coeff = NUM2DBL(coeff);
    break;
  default:
    rb_raise(rb_eStandardError, "unsupported coeff type(%x)", TYPE(coeff));
  }
  p->addTerm(_coeff, *_constr);
  return Qnil;
}

void
init_gurobi_column()
{
  rb_cGurobiColumn = rb_define_class_under(rb_mGurobi, "Column", rb_cObject);
  rb_define_alloc_func(rb_cGurobiColumn, column_alloc);
  rb_define_private_method(rb_cGurobiColumn, "initialize",  method_cast(column_init), 0);
  rb_define_method(rb_cGurobiColumn, "addTerm",      method_cast(column_add_term), 2);
}

/*!
\file Gurobi Expr class binding for Ruby 'Gurobi::Expr'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiExpr;

static GRBExpr*
get_expr(VALUE self)
{
  GRBExpr* p;
  Data_Get_Struct(self, GRBExpr, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_expr_free(GRBExpr* expr)
{
  expr->~GRBExpr();
  ruby_xfree(expr);
}

static VALUE
expr_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_expr_free, ALLOC(GRBExpr));
}

static VALUE
expr_init(VALUE self)
{
  GRBExpr* p     = get_expr(self);
  new(p) GRBExpr();
  return Qnil;
}

void
init_gurobi_expr()
{
  rb_cGurobiExpr = rb_define_class_under(rb_mGurobi, "Expr", rb_cObject);
}

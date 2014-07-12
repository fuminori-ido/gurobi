/*!
\file Gurobi LinExpr class binding for Ruby 'Gurobi::LinExpr'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiLinExpr;

GRBLinExpr*
gurobi_lin_expr_get(VALUE self)
{
  GRBLinExpr* p;
  Data_Get_Struct(self, GRBLinExpr, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_lin_expr_free(GRBLinExpr* lin_expr)
{
  lin_expr->~GRBLinExpr();
  ruby_xfree(lin_expr);
}

/*!
copy C++ GRBLinExpr to heap and wrap with Ruby Gurobi::LinExpr
*/
VALUE
gurobi_wrap_lin_expr(const GRBLinExpr& x)
{
  GRBLinExpr* duped = new GRBLinExpr(x);
  return Data_Wrap_Struct(rb_cGurobiLinExpr, NULL, gurobi_lin_expr_free, duped);
}

static VALUE
lin_expr_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_lin_expr_free, ALLOC(GRBLinExpr));
}

/*!

* LinExpr()
* LinExpr(Fixnum)
* LinExpr(Float)
* LinExpr(LinExpr)
* LinExpr(Var, double = 1.0)
*/
static VALUE
lin_expr_init(int argc, VALUE* argv, VALUE self)
{
  GRBLinExpr* p     = gurobi_lin_expr_get(self);
  switch( argc ){
  case 0:
    new(p) GRBLinExpr;
    break;
  case 1:
    if( CLASS_OF(argv[0]) == rb_cGurobiLinExpr ){
      new(p) GRBLinExpr();
      *p = *gurobi_lin_expr_get(argv[0]);
    }else if( TYPE(argv[0]) == T_FIXNUM ){
      new(p) GRBLinExpr(NUM2INT(argv[0]));
    }else if( TYPE(argv[0]) == T_FLOAT ){
      new(p) GRBLinExpr(NUM2DBL(argv[0]));
    }else if( CLASS_OF(argv[0]) == rb_cGurobiVar ){
      new(p) GRBLinExpr(*gurobi_var_get(argv[0]));
    }else{
      rb_raise(rb_eArgError, "unsupported argument type");
    }
    break;
  case 2:
    {
    new(p) GRBLinExpr(*gurobi_var_get(argv[0]), NUM2DBL(argv[1]));
    }
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments");
  }
  return Qnil;
}

static VALUE
lin_expr_add_term(VALUE self, VALUE coeff, VALUE var)
{
  GRBLinExpr* p       = gurobi_lin_expr_get(self);
  GRBVar*     _var    = gurobi_var_get(var);

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

  p->addTerms(&_coeff, _var, 1);
  return Qnil;
}

/*!
operator+=

NOTE: Ruby doesn't support "+=" operator overload and it interprets
as '+' and assignment.

* add_to(LinExpr)
* add_to(Var)
*/
static VALUE
lin_expr_add_to(VALUE self, VALUE rhs)
{
  GRBLinExpr* p         = gurobi_lin_expr_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiLinExpr ){
    GRBLinExpr* _rhs    = gurobi_lin_expr_get(rhs);
    *p += *_rhs;
  }else if( rhs_class == rb_cGurobiVar ){
    GRBVar* _rhs    = gurobi_var_get(rhs);
    *p += *_rhs;
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type");
  }
  return Qnil;
}

/*!
operator-=

NOTE: Ruby doesn't support "-=" operator overload and it interprets
as '-' and assignment.

* sub_from(LinExpr)
* sub_from(Var)
*/
static VALUE
lin_expr_sub_from(VALUE self, VALUE rhs)
{
  GRBLinExpr* p         = gurobi_lin_expr_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiLinExpr ){
    GRBLinExpr* _rhs    = gurobi_lin_expr_get(rhs);
    *p -= *_rhs;
  }else if( rhs_class == rb_cGurobiVar ){
    GRBVar* _rhs    = gurobi_var_get(rhs);
    *p -= *_rhs;
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type");
  }
  return Qnil;
}

static VALUE
lin_expr_get_constant(VALUE self)
{
  GRBLinExpr* p       = gurobi_lin_expr_get(self);
  return rb_float_new(p->getConstant());
}

static VALUE
lin_expr_get_coeff(VALUE self, VALUE i)
{
  GRBLinExpr* p       = gurobi_lin_expr_get(self);
  return rb_float_new(p->getCoeff(FIX2INT(i)));
}

static VALUE
lin_expr_get_var(VALUE self, VALUE i)
{
  GRBLinExpr* p       = gurobi_lin_expr_get(self);
  GRBVar*     duped   = new GRBVar(p->getVar(FIX2INT(i)));
  return Data_Wrap_Struct(rb_cGurobiVar, NULL, gurobi_var_free, duped);
}

static VALUE
lin_expr_size(VALUE self)
{
  GRBLinExpr* p       = gurobi_lin_expr_get(self);
  return INT2FIX(p->size());
}

/*!
LinExpr opeartor+(Var)
LinExpr opeartor+(LinExpr)
*/
static VALUE
lin_expr_add(VALUE self, VALUE rhs)
{
  GRBLinExpr* p         = gurobi_lin_expr_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiVar ){
    return gurobi_wrap_lin_expr(*p + *gurobi_var_get(rhs));
  }else if( rhs_class == rb_cGurobiLinExpr ){
    return gurobi_wrap_lin_expr(*p + *gurobi_lin_expr_get(rhs));
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type in Gurobi::LinExpr#+");
  }
}

/*!
TempConstr opeartor<=(const LinExpr& rhs)
*/
static VALUE
lin_expr_le(VALUE self, VALUE rhs)
{
  GRBLinExpr* p         = gurobi_lin_expr_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiLinExpr ){
    return gurobi_wrap_temp_constr(*p <= *gurobi_lin_expr_get(rhs));
  }else if( TYPE(rhs) == T_FIXNUM ){
    return gurobi_wrap_temp_constr(*p <= FIX2INT(rhs));
  }else if( TYPE(rhs) == T_FLOAT ){
    return gurobi_wrap_temp_constr(*p <= NUM2DBL(rhs));
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type in Gurobi::LinExpr#<=");
  }
}

/*!
TempConstr opeartor>=(const LinExpr& rhs)
*/
static VALUE
lin_expr_ge(VALUE self, VALUE rhs)
{
  GRBLinExpr* p         = gurobi_lin_expr_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiLinExpr ){
    return gurobi_wrap_temp_constr(*p >= *gurobi_lin_expr_get(rhs));
  }else if( TYPE(rhs) == T_FIXNUM ){
    return gurobi_wrap_temp_constr(*p >= FIX2INT(rhs));
  }else if( TYPE(rhs) == T_FLOAT ){
    return gurobi_wrap_temp_constr(*p >= NUM2DBL(rhs));
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type in Gurobi::LinExpr#>=");
  }
}

/*!
TempConstr opeartor==(LinExpr)
TempConstr opeartor==(Fixnum)
TempConstr opeartor==(Float)
TempConstr opeartor==(Var)
*/
static VALUE
lin_expr_eq(VALUE self, VALUE rhs)
{
  GRBLinExpr*   p         = gurobi_lin_expr_get(self);
  VALUE         rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiLinExpr ){
    return gurobi_wrap_temp_constr(*p == *gurobi_lin_expr_get(rhs));
  }else if( TYPE(rhs) == T_FIXNUM ){
    return gurobi_wrap_temp_constr(*p == FIX2INT(rhs));
  }else if( TYPE(rhs) == T_FLOAT ){
    return gurobi_wrap_temp_constr(*p == NUM2DBL(rhs));
  }else if( rhs_class == rb_cGurobiVar ){
    return gurobi_wrap_temp_constr(*p == *gurobi_var_get(rhs));
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type in Gurobi::LinExpr#==");
  }
}

void
init_gurobi_lin_expr()
{
  rb_cGurobiLinExpr = rb_define_class_under(rb_mGurobi, "LinExpr", rb_cGurobiExpr);
  rb_define_alloc_func(rb_cGurobiLinExpr, lin_expr_alloc);
  rb_define_private_method(rb_cGurobiLinExpr, "initialize",  method_cast(lin_expr_init), -1);
  rb_define_method(rb_cGurobiLinExpr, "addTerm",      method_cast(lin_expr_add_term), 2);
  rb_define_method(rb_cGurobiLinExpr, "add_to",       method_cast(lin_expr_add_to), 1);
  rb_define_method(rb_cGurobiLinExpr, "sub_from",     method_cast(lin_expr_sub_from), 1);
  rb_define_method(rb_cGurobiLinExpr, "getConstant",  method_cast(lin_expr_get_constant), 0);
  rb_define_method(rb_cGurobiLinExpr, "getCoeff",     method_cast(lin_expr_get_coeff), 1);
  rb_define_method(rb_cGurobiLinExpr, "getVar",       method_cast(lin_expr_get_var), 1);
  rb_define_method(rb_cGurobiLinExpr, "size",         method_cast(lin_expr_size), 0);
  rb_define_method(rb_cGurobiLinExpr, "+",            method_cast(lin_expr_add), 1);
  rb_define_method(rb_cGurobiLinExpr, "<=",           method_cast(lin_expr_le), 1);
  rb_define_method(rb_cGurobiLinExpr, ">=",           method_cast(lin_expr_ge), 1);
  rb_define_method(rb_cGurobiLinExpr, "==",           method_cast(lin_expr_eq), 1);
}

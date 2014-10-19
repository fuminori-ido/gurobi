/*!
\file Gurobi QuadExpr class binding for Ruby 'Gurobi::QuadExpr'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiQuadExpr;

GRBQuadExpr*
gurobi_quad_expr_get(VALUE self)
{
  GRBQuadExpr* p;
  Data_Get_Struct(self, GRBQuadExpr, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_quad_expr_free(GRBQuadExpr* quad_expr)
{
  quad_expr->~GRBQuadExpr();
  ruby_xfree(quad_expr);
}

/*!
copy C++ GRBQuadExpr to heap and wrap with Ruby Gurobi::QuadExpr
*/
VALUE
gurobi_wrap_quad_expr(const GRBQuadExpr& x)
{
  GRBQuadExpr* duped = new GRBQuadExpr(x);
  return Data_Wrap_Struct(rb_cGurobiQuadExpr, NULL, gurobi_quad_expr_free, duped);
}

static VALUE
quad_expr_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_quad_expr_free, ALLOC(GRBQuadExpr));
}

/*!

* QuadExpr()
* QuadExpr(QuadExpr)
* QuadExpr(Float)
* QuadExpr(Var, double = 1.0)
*/
static VALUE
quad_expr_init(int argc, VALUE* argv, VALUE self)
{
  GRBQuadExpr* p     = gurobi_quad_expr_get(self);
  switch( argc ){
  case 0:
    new(p) GRBQuadExpr;
    break;
  case 1:
    if( CLASS_OF(argv[0]) == rb_cGurobiQuadExpr ){
      new(p) GRBQuadExpr();
      *p = *gurobi_quad_expr_get(argv[0]);
    }else if( TYPE(argv[0]) == T_FLOAT ){
      new(p) GRBQuadExpr(NUM2DBL(argv[0]));
    }else if( CLASS_OF(argv[0]) == rb_cGurobiVar ){
      new(p) GRBQuadExpr(*gurobi_var_get(argv[0]));
    }else{
      rb_raise(rb_eArgError, "unsupported argument type");
    }
    break;
  case 2:
    {
    new(p) GRBQuadExpr(*gurobi_var_get(argv[0]), NUM2DBL(argv[1]));
    }
    break;
  default:
    rb_raise(rb_eArgError, "wrong number of arguments");
  }
  return Qnil;
}

/*!
addTerm(double, Var)
addTerm(double, Var, Var)
*/
static VALUE
quad_expr_add_term(int argc, VALUE* argv, VALUE self)
{
  GRBQuadExpr* p      = gurobi_quad_expr_get(self);
  switch( argc ){
  case 2:
    {
      p->addTerm(NUM2DBL(argv[0]), *gurobi_var_get(argv[1]));
      break;
    }
  case 3:
    {
      p->addTerm(NUM2DBL(argv[0]),
          *gurobi_var_get(argv[1]),
          *gurobi_var_get(argv[2]));
      break;
    }
  default:
    rb_raise(rb_eArgError, "wrong number of arguments");
  }
  return Qnil;
}

/*!
operator+=

NOTE: Ruby doesn't support "+=" operator overload and it interprets
as '+' and assignment.

* add_to(QuadExpr)
* add_to(Var)
*/
static VALUE
quad_expr_add_to(VALUE self, VALUE rhs)
{
  GRBQuadExpr* p         = gurobi_quad_expr_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiQuadExpr ){
    GRBQuadExpr* _rhs    = gurobi_quad_expr_get(rhs);
    *p += *_rhs;
  }else if( rhs_class == rb_cGurobiVar ){
    GRBVar* _rhs    = gurobi_var_get(rhs);
    *p += *_rhs;
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type");
  }
  return Qnil;
}

/*
static VALUE
quad_expr_get_constant(VALUE self)
{
  GRBQuadExpr* p       = gurobi_quad_expr_get(self);
  return rb_float_new(p->getConstant());
}
*/

static VALUE
quad_expr_get_coeff(VALUE self, VALUE i)
{
  GRBQuadExpr* p       = gurobi_quad_expr_get(self);
  return rb_float_new(p->getCoeff(FIX2INT(i)));
}

static VALUE
quad_expr_get_var1(VALUE self, VALUE i)
{
  GRBQuadExpr* p       = gurobi_quad_expr_get(self);
  GRBVar*     duped   = new GRBVar(p->getVar1(FIX2INT(i)));
  return Data_Wrap_Struct(rb_cGurobiVar, NULL, gurobi_var_free, duped);
}

static VALUE
quad_expr_get_var2(VALUE self, VALUE i)
{
  GRBQuadExpr* p       = gurobi_quad_expr_get(self);
  GRBVar*     duped   = new GRBVar(p->getVar2(FIX2INT(i)));
  return Data_Wrap_Struct(rb_cGurobiVar, NULL, gurobi_var_free, duped);
}

static VALUE
quad_expr_size(VALUE self)
{
  GRBQuadExpr* p       = gurobi_quad_expr_get(self);
  return INT2FIX(p->size());
}

/*!
QuadExpr opeartor+(QuadExpr)
QuadExpr opeartor+(LinExpr)
*/
static VALUE
quad_expr_add(VALUE self, VALUE rhs)
{
  GRBQuadExpr*  p         = gurobi_quad_expr_get(self);
  VALUE         rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiQuadExpr ){
    return gurobi_wrap_quad_expr(*p + *gurobi_quad_expr_get(rhs));
  }else if( rhs_class == rb_cGurobiLinExpr ){
    return gurobi_wrap_quad_expr(*p + *gurobi_lin_expr_get(rhs));
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type");
  }
}

/*!
TempConstr opeartor<=(const QuadExpr& rhs)
*/
static VALUE
quad_expr_le(VALUE self, VALUE rhs)
{
  GRBQuadExpr* p         = gurobi_quad_expr_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( rhs_class == rb_cGurobiQuadExpr ){
    GRBQuadExpr*     _rhs    = gurobi_quad_expr_get(rhs);
    GRBTempConstr   result  = *p <= *_rhs;
    GRBTempConstr*  duped   = new GRBTempConstr(result);
    return Data_Wrap_Struct(rb_cGurobiTempConstr, NULL, gurobi_temp_constr_free, duped);
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type");
  }
}

/*!
TempConstr opeartor==(QuadExpr)
TempConstr opeartor==(Float)
TempConstr opeartor==(Fixnum)
*/
static VALUE
quad_expr_eq(VALUE self, VALUE rhs)
{
  GRBQuadExpr*   p         = gurobi_quad_expr_get(self);
  VALUE         rhs_class = CLASS_OF(rhs);
  GRBTempConstr result;

  if( rhs_class == rb_cGurobiQuadExpr ){
    GRBQuadExpr*     _rhs    = gurobi_quad_expr_get(rhs);
    result  = *p == *_rhs;
  }else if( TYPE(rhs) == T_FIXNUM ){
    double          _rhs    = FIX2INT(rhs);
    result  = *p == _rhs;
  }else if( float_or_bignum(rhs) ){
    double          _rhs    = NUM2DBL(rhs);
    result  = *p == _rhs;
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type");
  }
  GRBTempConstr*  duped   = new GRBTempConstr(result);
  return Data_Wrap_Struct(rb_cGurobiTempConstr, NULL, gurobi_temp_constr_free, duped);
}

void
init_gurobi_quad_expr()
{
  rb_cGurobiQuadExpr = rb_define_class_under(rb_mGurobi, "QuadExpr", rb_cGurobiExpr);
  rb_define_alloc_func(rb_cGurobiQuadExpr, quad_expr_alloc);
  rb_define_private_method(rb_cGurobiQuadExpr, "initialize",  method_cast(quad_expr_init), -1);
  rb_define_method(rb_cGurobiQuadExpr, "addTerm",      method_cast(quad_expr_add_term), -1);
  rb_define_method(rb_cGurobiQuadExpr, "add_to",       method_cast(quad_expr_add_to), 1);
//rb_define_method(rb_cGurobiQuadExpr, "getConstant",  method_cast(quad_expr_get_constant), 0);
  rb_define_method(rb_cGurobiQuadExpr, "getCoeff",     method_cast(quad_expr_get_coeff), 1);
  rb_define_method(rb_cGurobiQuadExpr, "getVar1",      method_cast(quad_expr_get_var1), 1);
  rb_define_method(rb_cGurobiQuadExpr, "getVar2",      method_cast(quad_expr_get_var2), 1);
  rb_define_method(rb_cGurobiQuadExpr, "size",         method_cast(quad_expr_size), 0);
  rb_define_method(rb_cGurobiQuadExpr, "+",            method_cast(quad_expr_add), 1);
  rb_define_method(rb_cGurobiQuadExpr, "<=",           method_cast(quad_expr_le), 1);
  rb_define_method(rb_cGurobiQuadExpr, "==",           method_cast(quad_expr_eq), 1);
}

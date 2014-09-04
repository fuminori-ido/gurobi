/*!
\file Gurobi Var class binding for Ruby 'Gurobi::Var'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiVar;

GRBVar*
gurobi_var_get(VALUE self)
{
  GRBVar* p;
  Data_Get_Struct(self, GRBVar, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_var_free(GRBVar* var)
{
  var->~GRBVar();
  ruby_xfree(var);
}

/*!
\brief  used for Model::addVars()
*/
void
gurobi_var_free2(GRBVar* var)
{
  var->~GRBVar();
}

static VALUE
var_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_var_free, ALLOC(GRBVar));
}

static VALUE
var_init(VALUE self)
{
  GRBVar* p     = gurobi_var_get(self);
  new(p) GRBVar();
  return Qnil;
}

/*
NOTE: model.update should be executed before calling this.
*/
static VALUE
var_get_double(VALUE self, VALUE attr)
{
  GRBVar*         p     = gurobi_var_get(self);
  GRB_DoubleAttr  _attr = static_cast<GRB_DoubleAttr>(NUM2INT(attr));
  return rb_float_new(p->get(_attr));
}

/*
NOTE: model.update should be executed before calling this.
*/
static VALUE
var_get_string(VALUE self, VALUE attr)
{
  GRBVar*         p     = gurobi_var_get(self);
  GRB_StringAttr  _attr = static_cast<GRB_StringAttr>(NUM2INT(attr));
  return rb_str_new2(p->get(_attr).c_str());
}

/*
NOTE:

1. model.update should be executed before calling this.
1. actually it doesn't return char, but integer converted from char.
*/
static VALUE
var_get_char(VALUE self, VALUE attr)
{
  GRBVar*         p     = gurobi_var_get(self);
  GRB_CharAttr  _attr   = static_cast<GRB_CharAttr>(NUM2INT(attr));
  return INT2FIX(static_cast<char>(p->get(_attr)));
}

static VALUE
var_set_double(VALUE self, VALUE attr, VALUE val)
{
  GRBVar*         p     = gurobi_var_get(self);
  GRB_DoubleAttr  _attr = static_cast<GRB_DoubleAttr>(NUM2INT(attr));
  p->set(_attr, NUM2DBL(val));
  return Qnil;
}

static VALUE
var_set_string(VALUE self, VALUE attr, VALUE val)
{
  GRBVar*         p     = gurobi_var_get(self);
  GRB_StringAttr  _attr = static_cast<GRB_StringAttr>(NUM2INT(attr));
  p->set(_attr, string(RSTRING_PTR(val)));
  return Qnil;
}

static VALUE
var_set_char(VALUE self, VALUE attr, VALUE val)
{
  GRBVar*       p       = gurobi_var_get(self);
  GRB_CharAttr  _attr   = static_cast<GRB_CharAttr>(NUM2INT(attr));
  p->set(_attr, NUM2INT(val));
  return Qnil;
}

/*!
copy C++ GRBLinExpr to heap and wrap with Ruby Gurobi::LinExpr
*/
VALUE
gurobi_wrap_var(const GRBVar& x)
{
  GRBVar* duped = new GRBVar(x);
  return Data_Wrap_Struct(rb_cGurobiVar, NULL, gurobi_var_free, duped);
}

/*!
LinExpr   opeartor*(Float)
QuadExpr  opeartor*(Var)
QuadExpr  opeartor*(LinExpr)
*/
static VALUE
var_mul(VALUE self, VALUE rhs)
{
  GRBVar*     p         = gurobi_var_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( TYPE(rhs) == T_FIXNUM ){
    return gurobi_wrap_lin_expr(*p * FIX2INT(rhs));
  }else if( float_or_bignum(rhs) ){
    return gurobi_wrap_lin_expr(*p * NUM2DBL(rhs));
  }else if( rhs_class == rb_cGurobiVar ){
    return gurobi_wrap_quad_expr(*p * *gurobi_var_get(rhs));
  }else if( rhs_class == rb_cGurobiLinExpr ){
    return gurobi_wrap_quad_expr(*p * *gurobi_lin_expr_get(rhs));
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type");
  }
}

/*!
LinExpr opeartor+(Var)
LinExpr opeartor+(Fixnum)
LinExpr opeartor+(Float)
LinExpr opeartor+(LinExpr)
*/
static VALUE
var_add(VALUE self, VALUE rhs)
{
  GRBVar*     p         = gurobi_var_get(self);
  VALUE       rhs_class = CLASS_OF(rhs);

  if( TYPE(rhs) == T_FIXNUM ){
    return gurobi_wrap_lin_expr(*p + FIX2INT(rhs));
  }else if( float_or_bignum(rhs) ){
    return gurobi_wrap_lin_expr(*p + NUM2DBL(rhs));
  }else if( rhs_class == rb_cGurobiVar ){
    return gurobi_wrap_lin_expr(*p + *gurobi_var_get(rhs));
  }else if( rhs_class == rb_cGurobiLinExpr ){
    return gurobi_wrap_lin_expr(*p + *gurobi_lin_expr_get(rhs));
  }else{
    rb_raise(rb_eStandardError, "unknown rhs type in Gurobi::Var");
  }
}

#ifdef commentout
/*!
accessor to return model
*/
static VALUE
var_model(VALUE self)
{
  return ((gVar *)DATA_PTR(self))->model;
}

/*!
accessor to return ix
*/
static VALUE
var_ix(VALUE self)
{
  return INT2FIX(((gVar *)DATA_PTR(self))->ix);
}

static VALUE
var_name(VALUE self);

static VALUE
var_set(VALUE self, VALUE attr, VALUE val)
{
  int         result;
  double      val_d;
  int         val_i;
  VALUE       model     = ((gVar *)DATA_PTR(self))->model;
  gModel      *_model;
  int         ix        = ((gVar *)DATA_PTR(self))->ix;
  ID          attr_id   = SYM2ID(attr);
  const char  *attrname = rb_id2name(attr_id);

  Data_Get_Struct(model, gModel, _model);
  if( attr_id == idGRB_DBL_ATTR_START ){
    val_d = NUM2DBL(val);
    result = GRBsetdblattrelement(_model->model, attrname, ix, val_d);
    if( result ){
      rb_raise(rb_eStandardError, "GRBsetdblattrelement() error: %d", result);
    }
  }else{
    rb_raise(rb_eStandardError, "unsupportd attr(%s)", attrname);
  }
  return Qnil;
}


/*!
accessor to return name
*/
static VALUE
var_name(VALUE self)
{
  return var_get(self, ID2SYM(idGRB_STR_ATTR_VARNAME));
}
#endif /* commentout */

void
init_gurobi_var()
{
  rb_cGurobiVar = rb_define_class_under(rb_mGurobi, "Var", rb_cObject);
  rb_define_method(rb_cGurobiVar, "get_double",   method_cast(var_get_double), 1);
  rb_define_method(rb_cGurobiVar, "get_string",   method_cast(var_get_string), 1);
  rb_define_method(rb_cGurobiVar, "get_char",     method_cast(var_get_char), 1);
  rb_define_method(rb_cGurobiVar, "set_double",   method_cast(var_set_double), 2);
  rb_define_method(rb_cGurobiVar, "set_string",   method_cast(var_set_string), 2);
  rb_define_method(rb_cGurobiVar, "set_char",     method_cast(var_set_char), 2);
  rb_define_method(rb_cGurobiVar, "*",            method_cast(var_mul), 1);
  rb_define_method(rb_cGurobiVar, "+",            method_cast(var_add), 1);

#ifdef commentout
  /* primary methods */
  rb_define_method(rb_cGurobiVar, "model",       var_model, 0);
  rb_define_method(rb_cGurobiVar, "ix",          var_ix, 0);
  rb_define_method(rb_cGurobiVar, "set",         var_set, 2);

  /* secondary methods (depends on primary methods) */
  rb_define_method(rb_cGurobiVar, "name",        var_name, 0);
#endif
}

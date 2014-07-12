/*!
\file Gurobi Env class binding for Ruby 'Gurobi::Env'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiEnv;

GRBEnv*
gurobi_env_get(VALUE self)
{
  GRBEnv* p;
  Data_Get_Struct(self, GRBEnv, p);
  return p;
}

/*!
\brief  called every GC
*/
void
gurobi_env_free(GRBEnv* env)
{
/*
BUG: Gurobi::Model.new(Gurobi::Env.new) failed at exit ruby process
with the following Gurobi message:

  Warning: invalid Gurobi environment. Was it freed too early?

So that avoid freeing for now:

  env->~GRBEnv();
  ruby_xfree(env);
*/
}

static VALUE
env_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, gurobi_env_free, ALLOC(GRBEnv));
}

static VALUE
env_init(VALUE self)
{
  GRBEnv* p = gurobi_env_get(self);
  new (p) GRBEnv();
  return Qnil;
}

static VALUE
env_get_double(VALUE self, VALUE attr)
{
  GRBEnv*         p     = gurobi_env_get(self);
  GRB_DoubleParam _attr = static_cast<GRB_DoubleParam>(NUM2INT(attr));
  return rb_float_new(p->get(_attr));
}

static VALUE
env_get_int(VALUE self, VALUE attr)
{
  GRBEnv*         p     = gurobi_env_get(self);
  GRB_IntParam    _attr = static_cast<GRB_IntParam>(NUM2INT(attr));
  return INT2NUM(p->get(_attr));
}

static VALUE
env_set_double(VALUE self, VALUE attr, VALUE val)
{
  GRBEnv*         p     = gurobi_env_get(self);
  GRB_DoubleParam _attr = static_cast<GRB_DoubleParam>(NUM2INT(attr));
  p->set(_attr, NUM2DBL(val));
  return Qnil;
}

/*!

*/
static VALUE
env_set_int(VALUE self, VALUE attr, VALUE val)
{
  GRBEnv*       p     = gurobi_env_get(self);
  GRB_IntParam  _attr = static_cast<GRB_IntParam>(NUM2INT(attr));
  p->set(_attr, NUM2INT(val));
  return Qnil;
}

void
init_gurobi_env()
{
  rb_cGurobiEnv = rb_define_class_under(rb_mGurobi, "Env", rb_cObject);
  rb_define_alloc_func(rb_cGurobiEnv, env_alloc);
  rb_define_private_method(rb_cGurobiEnv, "initialize", method_cast(env_init), 0);
  rb_define_method(rb_cGurobiEnv, "get_double", method_cast(env_get_double), 1);
  rb_define_method(rb_cGurobiEnv, "get_int",    method_cast(env_get_int), 1);
  rb_define_method(rb_cGurobiEnv, "set_double", method_cast(env_set_double), 2);
  rb_define_method(rb_cGurobiEnv, "set_int",    method_cast(env_set_int), 2);
}

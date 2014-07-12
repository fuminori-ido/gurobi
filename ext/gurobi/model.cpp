/*!
\file Gurobi Model class binding for Ruby 'Gurobi::Model'
*/

#include "ruby_gurobi.h"

VALUE rb_cGurobiModel;

#ifdef commentout
static char *space(int depth){          // for debug
  static char buf[200];

  assert(depth < 200);
  char *p = buf;
  for(int i=0; i < depth; i++){
    *p++ = ' ';
  }
  p = '\0';
  return buf;
}

static void debug_vars(GRBModel *model){
  GRBVar* vars      = model->getVars();
  int     num_vars  = model->get(GRB_IntAttr_NumVars);
  int     i         = 0;
  for(GRBVar *p = vars; i < num_vars; i++, p++){
    printf("var: %s\n", p->get(GRB_StringAttr_VarName).c_str());
  }
  free(vars);
}
static void debug_lin_exp(GRBModel* model, GRBLinExpr* lin_exp, const char *name, int depth){
  int     size      = lin_exp->size();
  printf("%s%s lin_exp:\n", space(depth), name);
  for(int i=0; i < size; i++){
    GRBVar  var = lin_exp->getVar(i);
    printf("%s  %10g %s\n", space(depth),
        lin_exp->getCoeff(i),
        var.get(GRB_StringAttr_VarName).c_str());
  }
}

static const char *int2op(int op){
  const char *result;

  switch(op){
  case GRB_LESS_EQUAL:    result = "<="; break;
  case GRB_GREATER_EQUAL: result = ">="; break;
  case GRB_EQUAL:         result = "=="; break;
  default:
    fprintf(stderr, "unknown op(%d)\n", op);
  }
  return result;
}

static void debug_constrs(GRBModel *model){
  GRBConstr*  constrs     = model->getConstrs();
  int         num_constrs = model->get(GRB_IntAttr_NumConstrs);
  int     i         = 0;
  for(GRBConstr *p = constrs; i < num_constrs; i++, p++){
    printf("constr: %s\n", p->get(GRB_StringAttr_ConstrName).c_str());
  }
  free(constrs);
}
#endif /* commentout */


GRBModel*
gurobi_model_get(VALUE self)
{
  GRBModel* p;
  Data_Get_Struct(self, GRBModel, p);
  return p;
}

/*!
\brief  called every GC
*/
static void
model_free(GRBModel* model)
{
  model->~GRBModel();
  ruby_xfree(model);
}

static VALUE
model_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, model_free, ALLOC(GRBModel));
}

/*!
new(Env)
new(Env, String)
new(Model)
*/
static VALUE
model_init(int argc, VALUE* argv, VALUE  self)
{
  GRBModel* p     = gurobi_model_get(self);

  if( argc == 1 ){
    if( CLASS_OF(argv[0]) == rb_cGurobiEnv ){
      GRBEnv*   _env  = gurobi_env_get(argv[0]);
      new(p) GRBModel(*_env);
    }else if( CLASS_OF(argv[0]) == rb_cGurobiModel ){
      GRBModel*   _model  = gurobi_model_get(argv[0]);
      new(p) GRBModel(*_model);
    }else{
      rb_raise(rb_eArgError, "unsupported argument type");
    }
  }else if( argc == 2 ){
    if( CLASS_OF(argv[0]) == rb_cGurobiEnv ){
      GRBEnv*   _env  = gurobi_env_get(argv[0]);
      new(p) GRBModel(*_env, StringValuePtr(argv[1]));
    }else{
      rb_raise(rb_eArgError, "unsupported argument type");
    }
  }else{
    rb_raise(rb_eArgError, "wrong number of arguments");
  }
  return Qnil;
}

/*!
BUG:  not free _vars and _weights
*/
static VALUE
model_add_sos(VALUE self, VALUE vars, VALUE weights, VALUE len, VALUE type)
{
  GRBModel* p         = gurobi_model_get(self);
  int       _len      = FIX2INT(len);
  GRBVar*   _vars     = ALLOC_N(GRBVar, _len);
  double*   _weights  = ALLOC_N(double, _len);
  for(int i=0; i < _len; i++){
    _vars[i]    = *gurobi_var_get(rb_ary_entry(vars, i));
    _weights[i] = NUM2DBL(rb_ary_entry(weights, i));
  }
  return gurobi_wrap_sos(p->addSOS(_vars, _weights, _len, FIX2INT(type)));
}

static VALUE
model_add_var5(VALUE self, VALUE lb, VALUE ub, VALUE obj,
    VALUE vtype, VALUE varname)
{
  GRBModel* p     = gurobi_model_get(self);
  return gurobi_wrap_var(p->addVar(
      NUM2DBL(lb),
      NUM2DBL(ub),
      NUM2DBL(obj),
      FIX2INT(vtype),
      StringValuePtr(varname)));
}

static VALUE
model_add_var6(VALUE self, VALUE lb, VALUE ub, VALUE obj,
    VALUE vtype, VALUE col, VALUE varname)
{
  GRBModel* p     = gurobi_model_get(self);
  return gurobi_wrap_var(p->addVar(
      NUM2DBL(lb),
      NUM2DBL(ub),
      NUM2DBL(obj),
      FIX2INT(vtype),
      *gurobi_column_get(col),
      StringValuePtr(varname)));
}

static VALUE
model_add_var8(VALUE self, VALUE lb, VALUE ub, VALUE obj,
    VALUE vtype, VALUE numnz, VALUE constrs, VALUE coeffs, VALUE varname)
{
  GRBModel*   p         = gurobi_model_get(self);
  int         _numnz    = FIX2INT(numnz);
  GRBConstr*  _constrs  = ALLOC_N(GRBConstr, _numnz);
  double*     _coeffs   = ALLOC_N(double, _numnz);
  for(int i=0; i < _numnz; i++){
    _constrs[i] = *gurobi_constr_get(rb_ary_entry(constrs, i));
    _coeffs[i]  = NUM2DBL(rb_ary_entry(coeffs, i));
    }
  return gurobi_wrap_var(p->addVar(
      NUM2DBL(lb),
      NUM2DBL(ub),
      NUM2DBL(obj),
      FIX2INT(vtype),
      _numnz,
      _constrs,
      _coeffs,
      StringValuePtr(varname)));
}

/*!
addVar(lb, ub, obj, type, name)
addVar(lb, ub, obj, type, col, name)
addVar(lb, ub, obj, type, numnz, constrs, coeffs, name)
*/
static VALUE
model_add_var(int argc, VALUE* argv, VALUE  self)
{
  if( argc == 5 ){
    return model_add_var5(self, argv[0], argv[1], argv[2], argv[3], argv[4]);
  }else if( argc == 6 ){
    return model_add_var6(self, argv[0], argv[1], argv[2], argv[3], argv[4],
        argv[5]);
  }else if( argc == 8 ){
    return model_add_var8(self, argv[0], argv[1], argv[2], argv[3], argv[4],
        argv[5], argv[6], argv[7]);
  }else{
    rb_raise(rb_eArgError, "wrong number of arguments");
  }
}

/*
* addVars(count, type=GRB_CONTINUOUS)
* addVars(lb, ub, obj, type, names, count)
  lb    Lower bounds for new variables. Can be nil, in which case the variables get lower bounds of 0.0.
  ub    Upper bounds for new variables. Can be nil, in which case the variables get infinite upper bounds.
  obj   Objective coefficients for new variables. Can be nil, in which case the variables get objective coefficients of 0.0
  type  Variable types for new variables.  Can be nil, in which case the variables are assumed to be continuous.
  names Names for new variables. Can be nil, in which case all variables are given default names.
  count The number of variables to add.

BUG: freeing vars is not considered.
*/
static VALUE
model_add_vars(int argc, VALUE* argv, VALUE  self)
{
  GRBModel* p     = gurobi_model_get(self);
  int       _count;
  GRBVar*   vars;

  if( argc == 1 || argc == 2 ){
    _count = NUM2INT(argv[0]);

    if( argc == 2 ){
      vars  = p->addVars(_count, NUM2INT(argv[1]));
    }else{
      vars  = p->addVars(_count);
    }
  }else  if( argc == 6 ){
    // BUG: alloced memory are not freed:
    _count = FIX2INT(argv[5]);

    double* _lb     = argv[0] == Qnil ? NULL : ALLOC_N(double, _count);
    double* _ub     = argv[1] == Qnil ? NULL : ALLOC_N(double, _count);
    double* _obj    = argv[2] == Qnil ? NULL : ALLOC_N(double, _count);
    char*   _type   = argv[3] == Qnil ? NULL : ALLOC_N(char, _count);
    string* _names  = argv[4] == Qnil ? NULL : new string[_count];
    for(int i=0; i < _count; i++){
      if( argv[0] != Qnil ){ _lb[i]    = NUM2DBL(rb_ary_entry(argv[0], i)); }
      if( argv[1] != Qnil ){ _ub[i]    = NUM2DBL(rb_ary_entry(argv[1], i)); }
      if( argv[2] != Qnil ){ _obj[i]   = NUM2DBL(rb_ary_entry(argv[2], i)); }
      if( argv[3] != Qnil ){ _type[i]  = FIX2INT(rb_ary_entry(argv[3], i)); }
      if( argv[4] != Qnil ){
        VALUE n   = rb_ary_entry(argv[4], i);
        _names[i] = StringValuePtr(n);
      }
    }
    vars  = p->addVars(_lb, _ub, _obj, _type, _names, _count);
  }else{
    rb_raise(rb_eArgError, "wrong number of arguments");
  }

  // convert C++ array to Ruby array
  VALUE _vars = rb_ary_new();
  for(int i=0; i < _count; i++){
    VALUE _var  = Data_Wrap_Struct(rb_cGurobiVar, NULL,
        gurobi_var_free2, &vars[i]);
    rb_ary_push(_vars, _var);
  }
  return _vars;
}

static VALUE
model_update(VALUE self)
{
  GRBModel* p     = gurobi_model_get(self);
  p->update();
  return Qnil;
}

/*!
lhs         - Left-hand-side LinExpr
sence       - LESS_EQUAL, EQUAL, or GREATER_EQUAL
rhs         - Right-hand-side LinExpr, Var, double, or int
constrname  - Name for the new constraint
*/
static VALUE
model_add_constr_4(GRBModel* p, VALUE lhs, VALUE sense, VALUE rhs, VALUE constrname)
{
  int         _sense    = FIX2INT(sense);
  string      _strname  = StringValuePtr(constrname);

  if( CLASS_OF(lhs) == rb_cGurobiLinExpr ){
    GRBLinExpr* _lhs    = gurobi_lin_expr_get(lhs);
    double      rhs_val;

    switch( TYPE(rhs) ){
    case T_FIXNUM:
      rhs_val = FIX2INT(rhs);
      return gurobi_wrap_constr(p->addConstr(*_lhs, _sense, rhs_val, _strname));
    case T_FLOAT:
      rhs_val = NUM2DBL(rhs);
      return gurobi_wrap_constr(p->addConstr(*_lhs, _sense, rhs_val, _strname));
      break;
    default:
      if( CLASS_OF(rhs) == rb_cGurobiLinExpr ){
        return gurobi_wrap_constr(p->addConstr(*_lhs, _sense,
            *gurobi_lin_expr_get(rhs), _strname));
      }else if( CLASS_OF(rhs) == rb_cGurobiVar ){
        return gurobi_wrap_constr(p->addConstr(*_lhs, _sense,
            *gurobi_var_get(rhs), _strname));
      }
    }
  }
  rb_raise(rb_eStandardError, "unsupported rhs type(%x)", TYPE(rhs));
}

/*!
Constr addConstr(TempConstr, string name="")
Constr addConstr(LinExpr lhs, int sence, LinExpr rhs, string name="")
  lhs         - Left-hand-side LinExpr
  sence       - LESS_EQUAL, EQUAL, or GREATER_EQUAL
  rhs         - Right-hand-side LinExpr, Var, double, or int
  constrname  - Name for the new constraint
*/
static VALUE
model_add_constr(int argc, VALUE* argv, VALUE self)
{
  GRBModel*   p         = gurobi_model_get(self);

  if( argc == 1 ){        // addConstr(TempConstr)
    return gurobi_wrap_constr(p->addConstr(*gurobi_temp_constr_get(argv[0])));
  }else if( argc == 2 ){  // addConstr(TempConstr, name)
    return gurobi_wrap_constr(p->addConstr(
        *gurobi_temp_constr_get(argv[0]),
        StringValuePtr(argv[1])));
  }else if( argc == 4 ){  // addConstr(lhs, sence, rhs, constrname)
    return model_add_constr_4(p, argv[0], argv[1], argv[2], argv[3]);
  }
  rb_raise(rb_eArgError, "wrong number of arguments");
}

/*!
lhs         - Left-hand-side QuadExpr
sence       - LESS_EQUAL, EQUAL, or GREATER_EQUAL
rhs         - Right-hand-side QuadExpr, Var
constrname  - Name for the new constraint
*/
static VALUE
model_add_qconstr_4(GRBModel* p, VALUE lhs, VALUE sense, VALUE rhs, VALUE constrname)
{
  int         _sense    = FIX2INT(sense);
  string      _strname  = StringValuePtr(constrname);

  if( CLASS_OF(lhs) == rb_cGurobiLinExpr ){
    GRBQuadExpr* _lhs   = gurobi_quad_expr_get(lhs);

    if( CLASS_OF(rhs) == rb_cGurobiQuadExpr ){
      return gurobi_wrap_qconstr(p->addQConstr(*_lhs, _sense,
          *gurobi_quad_expr_get(rhs), _strname));
    }else if( CLASS_OF(rhs) == rb_cGurobiVar ){
      return gurobi_wrap_qconstr(p->addQConstr(*_lhs, _sense,
          *gurobi_var_get(rhs), _strname));
    }
  }
  rb_raise(rb_eStandardError, "unsupported lhs type(%x)", TYPE(lhs));
}

/*!
QConstr addQConstr(TempConstr, string name ="")
QConstr addQConstr(QuadExpr lhs, int sence, Var rhs, string name="")
  lhs         - Left-hand-side QuadExpr
  sence       - LESS_EQUAL, EQUAL, or GREATER_EQUAL
  rhs         - Right-hand-side LinExpr, Var, double, or int
  constrname  - Name for the new constraint
*/
static VALUE
model_add_qconstr(int argc, VALUE* argv, VALUE self)
{
  GRBModel*   p         = gurobi_model_get(self);

  if( argc == 1 ){        // addQConstr(TempConstr)
    return gurobi_wrap_qconstr(p->addQConstr(*gurobi_temp_constr_get(argv[0])));
  }else if( argc == 2 ){  // addQConstr(TempConstr, name)
    return gurobi_wrap_qconstr(p->addQConstr(
        *gurobi_temp_constr_get(argv[0]),
        StringValuePtr(argv[1])));
  }else if( argc == 4 ){  // addQConstr(lhs, sence, rhs, constrname)
    return model_add_qconstr_4(p, argv[0], argv[1], argv[2], argv[3]);
  }else{
    rb_raise(rb_eArgError, "wrong number of arguments");
  }
}

static VALUE
model_feas_relax(VALUE self, VALUE relaxobjtype, VALUE minrelax,
    VALUE vrelax, VALUE crelax)
{
  GRBModel* p             = gurobi_model_get(self);
  int       _relaxobjtype = NUM2INT(relaxobjtype);
  return rb_float_new(p->feasRelax(_relaxobjtype,
      RTEST(minrelax),
      RTEST(vrelax),
      RTEST(crelax)));
}

/*!
Array[GRBConstr] getConstrs()

BUG: freeing GRBConstr C++ array is not considered.
*/
static VALUE
model_get_constrs(VALUE self)
{
  GRBModel*   p         = gurobi_model_get(self);
  GRBConstr*  constrs   = p->getConstrs();
  VALUE       _constrs  = rb_ary_new();
  int         size      = p->get(GRB_IntAttr_NumConstrs);
  for(int i=0; i < size; i++){
    VALUE _constr  = Data_Wrap_Struct(rb_cGurobiConstr, NULL,
        gurobi_constr_free2, &constrs[i]);
    rb_ary_push(_constrs, _constr);
  }
  return _constrs;
}

/*!
void getTuneResult(int n)
*/
static VALUE
model_get_tune_result(VALUE self, VALUE n)
{
  GRBModel*   p         = gurobi_model_get(self);
  p->getTuneResult(NUM2INT(n));
  return Qnil;
}

/*!
Array[GRBVar] getVars()

BUG: freeing GRBVar C++ array is not considered.
*/
static VALUE
model_get_vars(VALUE self)
{
  GRBModel*   p         = gurobi_model_get(self);
  GRBVar*     vars      = p->getVars();
  VALUE       _vars     = rb_ary_new();
  int         size      = p->get(GRB_IntAttr_NumVars);
  for(int i=0; i < size; i++){
    VALUE _var  = Data_Wrap_Struct(rb_cGurobiVar, NULL,
        gurobi_var_free2, &vars[i]);
    rb_ary_push(_vars, _var);
  }
  return _vars;
}

static VALUE
model_fixed_model(VALUE self)
{
  GRBModel* p     = gurobi_model_get(self);
  GRBModel  m     = p->fixedModel();
  GRBModel* duped = new GRBModel(m);
  return Data_Wrap_Struct(rb_cGurobiModel, NULL, model_free, duped);
}

static VALUE
model_optimize(VALUE self)
{
  GRBModel* p     = gurobi_model_get(self);
  p->optimize();
  return Qnil;
}

static VALUE
model_compute_iis(VALUE self)
{
  GRBModel* p     = gurobi_model_get(self);
  p->computeIIS();
  return Qnil;
}

static VALUE
model_remove(VALUE self, VALUE obj)
{
  GRBModel*       p     = gurobi_model_get(self);
  if( CLASS_OF(obj) == rb_cGurobiConstr ){
    p->remove(*gurobi_constr_get(obj));
  }else if( CLASS_OF(obj) == rb_cGurobiQConstr ){
    p->remove(*gurobi_qconstr_get(obj));
  }else if( CLASS_OF(obj) == rb_cGurobiSos ){
    p->remove(*gurobi_sos_get(obj));
  }else if( CLASS_OF(obj) == rb_cGurobiVar ){
    p->remove(*gurobi_var_get(obj));
  }else{
    rb_raise(rb_eArgError, "unsupported argument type");
  }
  return Qnil;
}

static VALUE
model_reset(VALUE self)
{
  GRBModel* p     = gurobi_model_get(self);
  p->reset();
  return Qnil;
}

static VALUE
model_tune(VALUE self)
{
  GRBModel* p     = gurobi_model_get(self);
  p->tune();
  return Qnil;
}

static VALUE
model_get_double(VALUE self, VALUE attr)
{
  GRBModel*       p     = gurobi_model_get(self);
  GRB_DoubleAttr  _attr = static_cast<GRB_DoubleAttr>(NUM2INT(attr));
  return rb_float_new(p->get(_attr));
}

static VALUE
model_get_int(VALUE self, VALUE attr)
{
  GRBModel*       p     = gurobi_model_get(self);
  GRB_IntAttr  _attr    = static_cast<GRB_IntAttr>(NUM2DBL(attr));
  return INT2FIX(p->get(_attr));
}

static VALUE
model_get_string(VALUE self, VALUE attr)
{
  GRBModel*       p     = gurobi_model_get(self);
  GRB_StringAttr  _attr = static_cast<GRB_StringAttr>(NUM2INT(attr));
  return rb_str_new2(p->get(_attr).c_str());
}

static VALUE
model_set_int(VALUE self, VALUE attr, VALUE val)
{
  GRBModel*   p     = gurobi_model_get(self);
  GRB_IntAttr _attr = static_cast<GRB_IntAttr>(NUM2INT(attr));
  p->set(_attr, NUM2INT(val));
  return Qnil;
}

static VALUE
model_set_string(VALUE self, VALUE attr, VALUE val)
{
  GRBModel*       p     = gurobi_model_get(self);
  GRB_StringAttr  _attr = static_cast<GRB_StringAttr>(NUM2INT(attr));
  p->set(_attr, string(RSTRING_PTR(val)));
  return Qnil;
}

static VALUE
model_set_objective(int argc, VALUE* argv, VALUE  self)
{
  GRBModel*   p         = gurobi_model_get(self);
  if( argc == 2 ){
  }

  switch( argc ){
  case 1:
    if( CLASS_OF(argv[0]) == rb_cGurobiLinExpr ){
      p->setObjective(*gurobi_lin_expr_get(argv[0]));
    }else if( CLASS_OF(argv[0]) == rb_cGurobiQuadExpr ){
      p->setObjective(*gurobi_quad_expr_get(argv[0]));
    }else{
      rb_raise(rb_eArgError, "unsupported argument type");
    }
    break;
  case 2: {
    int _sense = NUM2INT(argv[1]);

    if( CLASS_OF(argv[0]) == rb_cGurobiLinExpr ){
      p->setObjective(*gurobi_lin_expr_get(argv[0]), _sense);
    }else if( CLASS_OF(argv[0]) == rb_cGurobiQuadExpr ){
      p->setObjective(*gurobi_quad_expr_get(argv[0]), _sense);
    }else{
      rb_raise(rb_eArgError, "unsupported argument type");
    }
    break;
  }
  default:
    rb_raise(rb_eArgError, "wrong number of arguments");
  }
  return Qnil;
}

/*
NOTE: Followings are different:

* env passed to Grb::Model.new initially created
* getEnv() returns

In order to change env parameter after setting, need to use *latter*.
Otherwise, following warning happens:

  Warning: parameter changes on this environment will not affect
  existing models.
*/
static VALUE
model_get_env(VALUE self)
{
  GRBModel*   p         = gurobi_model_get(self);
  GRBEnv      env       = p->getEnv();
  GRBEnv*     duped     = new GRBEnv(env);
  return Data_Wrap_Struct(rb_cGurobiEnv, 0, gurobi_env_free, duped);
}

static VALUE
model_write(VALUE self, VALUE filename)
{
  GRBModel*   p         = gurobi_model_get(self);
  p->write(StringValuePtr(filename));
  return Qnil;
}

void
init_gurobi_model()
{
  rb_cGurobiModel = rb_define_class_under(rb_mGurobi, "Model", rb_cObject);
  rb_define_alloc_func(rb_cGurobiModel, model_alloc);
  rb_define_private_method(rb_cGurobiModel, "initialize",  method_cast(model_init), -1);
  rb_define_private_method(rb_cGurobiModel, "_addSOS",     method_cast(model_add_sos), 4);
  rb_define_method(rb_cGurobiModel, "addConstr",    method_cast(model_add_constr), -1);
  rb_define_method(rb_cGurobiModel, "addQConstr",   method_cast(model_add_qconstr), -1);
  rb_define_method(rb_cGurobiModel, "addVar",       method_cast(model_add_var), -1);
  rb_define_method(rb_cGurobiModel, "addVars",      method_cast(model_add_vars), -1);
  rb_define_method(rb_cGurobiModel, "computeIIS",   method_cast(model_compute_iis), 0);
  rb_define_method(rb_cGurobiModel, "feasRelax",    method_cast(model_feas_relax), 4);
  rb_define_method(rb_cGurobiModel, "fixedModel",   method_cast(model_fixed_model), 0);
  rb_define_method(rb_cGurobiModel, "getConstrs",   method_cast(model_get_constrs), 0);
  rb_define_method(rb_cGurobiModel, "getTuneResult",method_cast(model_get_tune_result), 1);
  rb_define_method(rb_cGurobiModel, "getVars",      method_cast(model_get_vars), 0);
  rb_define_method(rb_cGurobiModel, "get_double",   method_cast(model_get_double), 1);
  rb_define_method(rb_cGurobiModel, "get_int",      method_cast(model_get_int), 1);
  rb_define_method(rb_cGurobiModel, "get_string",   method_cast(model_get_string), 1);
  rb_define_method(rb_cGurobiModel, "optimize",     method_cast(model_optimize), 0);
  rb_define_method(rb_cGurobiModel, "remove",       method_cast(model_remove), 1);
  rb_define_method(rb_cGurobiModel, "reset",        method_cast(model_reset), 0);
  rb_define_method(rb_cGurobiModel, "set_int",      method_cast(model_set_int), 2);
  rb_define_method(rb_cGurobiModel, "set_string",   method_cast(model_set_string), 2);
  rb_define_method(rb_cGurobiModel, "setObjective", method_cast(model_set_objective), -1);
  rb_define_method(rb_cGurobiModel, "getEnv",       method_cast(model_get_env), 0);
  rb_define_method(rb_cGurobiModel, "tune",         method_cast(model_tune), 0);
  rb_define_method(rb_cGurobiModel, "update",       method_cast(model_update), 0);
  rb_define_method(rb_cGurobiModel, "write",        method_cast(model_write), 1);
/*
  rb_define_method(rb_cGurobiModel, "set",         model_set, 2);
*/
}

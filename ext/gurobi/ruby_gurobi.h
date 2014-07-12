#ifndef __ruby_gurobi_h
#define __ruby_gurobi_h

#include <ruby.h>
#include <gurobi_c++.h>

extern VALUE rb_mGurobi;
extern VALUE rb_cGurobiEnv;
extern VALUE rb_cGurobiModel;
extern VALUE rb_cGurobiVar;
extern VALUE rb_cGurobiExpr;
extern VALUE rb_cGurobiLinExpr;
extern VALUE rb_cGurobiQConstr;
extern VALUE rb_cGurobiQuadExpr;
extern VALUE rb_cGurobiConstr;
extern VALUE rb_cGurobiColumn;
extern VALUE rb_cGurobiSos;
extern VALUE rb_cGurobiTempConstr;

extern VALUE rb_mGurobiIntParam;
extern VALUE rb_mGurobiDoubleParam;
extern VALUE rb_mGurobiStringParam;

extern VALUE rb_mGurobiIntAttr;
extern VALUE rb_mGurobiCharAttr;
extern VALUE rb_mGurobiDoubleAttr;
extern VALUE rb_mGurobiStringAttr;

extern ID idGRB_INT_ATTR_MODELSENSE;
extern ID idGRB_INT_ATTR_STATUS;
extern ID idGRB_INT_PAR_METHOD;
extern ID idGRB_DBL_ATTR_START;
extern ID idGRB_DBL_ATTR_X;
extern ID idGRB_DBL_ATTR_OBJVAL;
extern ID idGRB_STR_ATTR_VARNAME;
extern ID idGRB_STR_ATTR_CONSTRNAME;

extern "C" void Init_gurobi(void);

void          init_gurobi_env();
void          init_gurobi_model();
void          init_gurobi_var();
void          init_gurobi_column();
void          init_gurobi_constr();
void          init_gurobi_qconstr();
void          init_gurobi_expr();
void          init_gurobi_lin_expr();
void          init_gurobi_quad_expr();
void          init_gurobi_sos();
void          init_gurobi_temp_constr();

/* self -> this */
GRBEnv*       gurobi_env_get(VALUE);
GRBModel*     gurobi_model_get(VALUE);
GRBVar*       gurobi_var_get(VALUE);
GRBLinExpr*   gurobi_lin_expr_get(VALUE);
GRBQConstr*   gurobi_qconstr_get(VALUE);
GRBQuadExpr*  gurobi_quad_expr_get(VALUE);
GRBColumn*    gurobi_column_get(VALUE);
GRBConstr*    gurobi_constr_get(VALUE);
GRBSOS*       gurobi_sos_get(VALUE);
GRBTempConstr*gurobi_temp_constr_get(VALUE);

/* free function to bind C++ dtor & ruby GC */
void          gurobi_env_free(GRBEnv*);
void          gurobi_var_free(GRBVar*);
void          gurobi_var_free2(GRBVar*);
void          gurobi_column_free(GRBColumn*);
void          gurobi_constr_free(GRBConstr*);
void          gurobi_constr_free2(GRBConstr*);
void          gurobi_temp_constr_free(GRBTempConstr*);
void          gurobi_lin_expr_free(GRBLinExpr*);
void          gurobi_quad_expr_free(GRBQuadExpr*);
void          gurobi_sos_free(GRBSOS*);

/*
copy C++ object (may be a local var) to heap and wrap it with corresponding
Ruby object
*/
VALUE         gurobi_wrap_var(const GRBVar&);
VALUE         gurobi_wrap_lin_expr(const GRBLinExpr&);
VALUE         gurobi_wrap_quad_expr(const GRBQuadExpr&);
VALUE         gurobi_wrap_constr(const GRBConstr&);
VALUE         gurobi_wrap_qconstr(const GRBQConstr&);
VALUE         gurobi_wrap_temp_constr(const GRBTempConstr&);
VALUE         gurobi_wrap_sos(const GRBSOS&);

/* Utility */
VALUE     gurobi_sym(const char *);
VALUE     gurobi_sym2(ID);

#define method_cast(method)   reinterpret_cast<VALUE(*)(...)>(method)

#endif /* __ruby_gurobi_h */

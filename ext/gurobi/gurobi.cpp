#include "ruby_gurobi.h"

VALUE rb_mGurobi;

VALUE rb_mGurobiIntParam;
VALUE rb_mGurobiDoubleParam;
VALUE rb_mGurobiStringParam;

VALUE rb_mGurobiIntAttr;
VALUE rb_mGurobiCharAttr;
VALUE rb_mGurobiDoubleAttr;
VALUE rb_mGurobiStringAttr;

static void
init_gurobi_const()
{
  /* Constraint senses */
  rb_define_const(rb_mGurobi, "LESS_EQUAL",    INT2FIX(GRB_LESS_EQUAL));
  rb_define_const(rb_mGurobi, "GREATER_EQUAL", INT2FIX(GRB_GREATER_EQUAL));
  rb_define_const(rb_mGurobi, "EQUAL",         INT2FIX(GRB_EQUAL));

  /* Variable types */
  rb_define_const(rb_mGurobi, "CONTINUOUS",  INT2FIX(GRB_CONTINUOUS));
  rb_define_const(rb_mGurobi, "BINARY",      INT2FIX(GRB_BINARY));
  rb_define_const(rb_mGurobi, "INTEGER",     INT2FIX(GRB_INTEGER));

  /* Objective sense */
  rb_define_const(rb_mGurobi, "MINIMIZE",    INT2FIX(GRB_MINIMIZE));
  rb_define_const(rb_mGurobi, "MAXIMIZE",    INT2FIX(GRB_MAXIMIZE));

  /* SOS types */
  rb_define_const(rb_mGurobi, "SOS_TYPE1",    INT2FIX(GRB_SOS_TYPE1));
  rb_define_const(rb_mGurobi, "SOS_TYPE2",    INT2FIX(GRB_SOS_TYPE2));
  
  /* Numeric constants */
  rb_define_const(rb_mGurobi, "INFINITY",    rb_float_new(GRB_INFINITY));

  /* Model status codes (after call to GRBoptimize()) */
  rb_define_const(rb_mGurobi, "LOADED",          INT2FIX(GRB_LOADED));
  rb_define_const(rb_mGurobi, "OPTIMAL",         INT2FIX(GRB_OPTIMAL));
  rb_define_const(rb_mGurobi, "INFEASIBLE",      INT2FIX(GRB_INFEASIBLE));
  rb_define_const(rb_mGurobi, "INF_OR_UNBD",     INT2FIX(GRB_INF_OR_UNBD));
  rb_define_const(rb_mGurobi, "UNBOUNDED",       INT2FIX(GRB_UNBOUNDED));
  rb_define_const(rb_mGurobi, "CUTOFF",          INT2FIX(GRB_CUTOFF));
  rb_define_const(rb_mGurobi, "ITERATION_LIMIT", INT2FIX(GRB_ITERATION_LIMIT));
  rb_define_const(rb_mGurobi, "NODE_LIMIT",      INT2FIX(GRB_NODE_LIMIT));
  rb_define_const(rb_mGurobi, "TIME_LIMIT",      INT2FIX(GRB_TIME_LIMIT));
  rb_define_const(rb_mGurobi, "SOLUTION_LIMIT",  INT2FIX(GRB_SOLUTION_LIMIT));
  rb_define_const(rb_mGurobi, "INTERRUPTED",     INT2FIX(GRB_INTERRUPTED));
  rb_define_const(rb_mGurobi, "NUMERIC",         INT2FIX(GRB_NUMERIC));
  rb_define_const(rb_mGurobi, "SUBOPTIMAL",      INT2FIX(GRB_SUBOPTIMAL));
  rb_define_const(rb_mGurobi, "INPROGRESS",      INT2FIX(GRB_INPROGRESS));

  /* Parameter enumerations */
  /* All *CUTS parameters */
  rb_define_const(rb_mGurobi, "METHOD_AUTO",       INT2FIX(GRB_METHOD_AUTO));
  rb_define_const(rb_mGurobi, "METHOD_PRIMAL",     INT2FIX(GRB_METHOD_PRIMAL));
  rb_define_const(rb_mGurobi, "METHOD_DUAL",       INT2FIX(GRB_METHOD_DUAL));
  rb_define_const(rb_mGurobi, "METHOD_BARRIER",    INT2FIX(GRB_METHOD_BARRIER));
  rb_define_const(rb_mGurobi, "METHOD_CONCURRENT", INT2FIX(GRB_METHOD_CONCURRENT));
  rb_define_const(rb_mGurobi, "METHOD_DETERMINISTIC_CONCURRENT", INT2FIX(GRB_METHOD_DETERMINISTIC_CONCURRENT));

  /* FEASRELAX */
  rb_define_const(rb_mGurobi, "FEASRELAX_LINEAR",       INT2FIX(GRB_FEASRELAX_LINEAR));
  rb_define_const(rb_mGurobi, "FEASRELAX_QUADRATIC",    INT2FIX(GRB_FEASRELAX_QUADRATIC));
  rb_define_const(rb_mGurobi, "FEASRELAX_CARDINALITY",  INT2FIX(GRB_FEASRELAX_CARDINALITY));
}

static void
init_gurobi_int_param()
{
  rb_mGurobiIntParam = rb_define_module_under(rb_mGurobi, "IntParam");
  rb_define_const(rb_mGurobiIntParam, "SOLUTION_LIMIT", INT2FIX(GRB_IntParam_SolutionLimit));
  rb_define_const(rb_mGurobiIntParam, "METHOD", INT2FIX(GRB_IntParam_Method));
  rb_define_const(rb_mGurobiIntParam, "LP_METHOD", INT2FIX(GRB_IntParam_LPMethod));
  rb_define_const(rb_mGurobiIntParam, "SCALE_FLAG", INT2FIX(GRB_IntParam_ScaleFlag));
  rb_define_const(rb_mGurobiIntParam, "SIMPLEX_PRICING", INT2FIX(GRB_IntParam_SimplexPricing));
  rb_define_const(rb_mGurobiIntParam, "QUAD", INT2FIX(GRB_IntParam_Quad));
  rb_define_const(rb_mGurobiIntParam, "NORM_ADJUST", INT2FIX(GRB_IntParam_NormAdjust));
  rb_define_const(rb_mGurobiIntParam, "SIFTING", INT2FIX(GRB_IntParam_Sifting));
  rb_define_const(rb_mGurobiIntParam, "SIFT_METHOD", INT2FIX(GRB_IntParam_SiftMethod));
  rb_define_const(rb_mGurobiIntParam, "SUB_MIP_NODES", INT2FIX(GRB_IntParam_SubMIPNodes));
  rb_define_const(rb_mGurobiIntParam, "VAR_BRANCH", INT2FIX(GRB_IntParam_VarBranch));
  rb_define_const(rb_mGurobiIntParam, "CUTS", INT2FIX(GRB_IntParam_Cuts));
  rb_define_const(rb_mGurobiIntParam, "CLIQUE_CUTS", INT2FIX(GRB_IntParam_CliqueCuts));
  rb_define_const(rb_mGurobiIntParam, "COVER_CUTS", INT2FIX(GRB_IntParam_CoverCuts));
  rb_define_const(rb_mGurobiIntParam, "FLOW_COVER_CUTS", INT2FIX(GRB_IntParam_FlowCoverCuts));
  rb_define_const(rb_mGurobiIntParam, "FLOW_PATH_CUTS", INT2FIX(GRB_IntParam_FlowPathCuts));
  rb_define_const(rb_mGurobiIntParam, "GUB_COVER_CUTS", INT2FIX(GRB_IntParam_GUBCoverCuts));
  rb_define_const(rb_mGurobiIntParam, "IMPLIED_CUTS", INT2FIX(GRB_IntParam_ImpliedCuts));
  rb_define_const(rb_mGurobiIntParam, "MIP_SEP_CUTS", INT2FIX(GRB_IntParam_MIPSepCuts));
  rb_define_const(rb_mGurobiIntParam, "MIR_CUTS", INT2FIX(GRB_IntParam_MIRCuts));
  rb_define_const(rb_mGurobiIntParam, "MOD_K_CUTS", INT2FIX(GRB_IntParam_ModKCuts));
  rb_define_const(rb_mGurobiIntParam, "ZERO_HALF_CUTS", INT2FIX(GRB_IntParam_ZeroHalfCuts));
  rb_define_const(rb_mGurobiIntParam, "NETWORK_CUTS", INT2FIX(GRB_IntParam_NetworkCuts));
  rb_define_const(rb_mGurobiIntParam, "SUB_MIP_CUTS", INT2FIX(GRB_IntParam_SubMIPCuts));
  rb_define_const(rb_mGurobiIntParam, "CUT_AGG_PASSES", INT2FIX(GRB_IntParam_CutAggPasses));
  rb_define_const(rb_mGurobiIntParam, "CUT_PASSES", INT2FIX(GRB_IntParam_CutPasses));
  rb_define_const(rb_mGurobiIntParam, "GOMORY_PASSES", INT2FIX(GRB_IntParam_GomoryPasses));
  rb_define_const(rb_mGurobiIntParam, "NODE_METHOD", INT2FIX(GRB_IntParam_NodeMethod));
  rb_define_const(rb_mGurobiIntParam, "PRESOLVE", INT2FIX(GRB_IntParam_Presolve));
  rb_define_const(rb_mGurobiIntParam, "AGGREGATE", INT2FIX(GRB_IntParam_Aggregate));
  rb_define_const(rb_mGurobiIntParam, "IIS_METHOD", INT2FIX(GRB_IntParam_IISMethod));
  rb_define_const(rb_mGurobiIntParam, "PRE_CRUSH", INT2FIX(GRB_IntParam_PreCrush));
  rb_define_const(rb_mGurobiIntParam, "PRE_DEP_ROW", INT2FIX(GRB_IntParam_PreDepRow));
  rb_define_const(rb_mGurobiIntParam, "PRE_MIQP_METHOD", INT2FIX(GRB_IntParam_PreMIQPMethod));
  rb_define_const(rb_mGurobiIntParam, "PRE_PASSES", INT2FIX(GRB_IntParam_PrePasses));
  rb_define_const(rb_mGurobiIntParam, "DISPLAY_INTERVAL", INT2FIX(GRB_IntParam_DisplayInterval));
  rb_define_const(rb_mGurobiIntParam, "OUTPUT_FLAG", INT2FIX(GRB_IntParam_OutputFlag));
  rb_define_const(rb_mGurobiIntParam, "THREADS", INT2FIX(GRB_IntParam_Threads));
  rb_define_const(rb_mGurobiIntParam, "BAR_ITER_LIMIT", INT2FIX(GRB_IntParam_BarIterLimit));
  rb_define_const(rb_mGurobiIntParam, "CROSSOVER", INT2FIX(GRB_IntParam_Crossover));
  rb_define_const(rb_mGurobiIntParam, "CROSSOVER_BASIS", INT2FIX(GRB_IntParam_CrossoverBasis));
  rb_define_const(rb_mGurobiIntParam, "BAR_CORRECTORS", INT2FIX(GRB_IntParam_BarCorrectors));
  rb_define_const(rb_mGurobiIntParam, "BAR_ORDER", INT2FIX(GRB_IntParam_BarOrder));
  rb_define_const(rb_mGurobiIntParam, "PUMP_PASSES", INT2FIX(GRB_IntParam_PumpPasses));
  rb_define_const(rb_mGurobiIntParam, "RINS", INT2FIX(GRB_IntParam_RINS));
  rb_define_const(rb_mGurobiIntParam, "SYMMETRY", INT2FIX(GRB_IntParam_Symmetry));
  rb_define_const(rb_mGurobiIntParam, "MIP_FOCUS", INT2FIX(GRB_IntParam_MIPFocus));
  rb_define_const(rb_mGurobiIntParam, "NUMERIC_FOCUS", INT2FIX(GRB_IntParam_NumericFocus));
  rb_define_const(rb_mGurobiIntParam, "AGG_FILL", INT2FIX(GRB_IntParam_AggFill));
  rb_define_const(rb_mGurobiIntParam, "PRE_DUAL", INT2FIX(GRB_IntParam_PreDual));
  rb_define_const(rb_mGurobiIntParam, "SOLUTION_NUMBER", INT2FIX(GRB_IntParam_SolutionNumber));
  rb_define_const(rb_mGurobiIntParam, "MIN_REL_NODES", INT2FIX(GRB_IntParam_MinRelNodes));
  rb_define_const(rb_mGurobiIntParam, "ZERO_OBJ_NODES", INT2FIX(GRB_IntParam_ZeroObjNodes));
  rb_define_const(rb_mGurobiIntParam, "BRANCH_DIR", INT2FIX(GRB_IntParam_BranchDir));
  rb_define_const(rb_mGurobiIntParam, "INF_UNBD_INFO", INT2FIX(GRB_IntParam_InfUnbdInfo));
  rb_define_const(rb_mGurobiIntParam, "DUAL_REDUCTIONS", INT2FIX(GRB_IntParam_DualReductions));
  rb_define_const(rb_mGurobiIntParam, "BAR_HOMOGENEOUS", INT2FIX(GRB_IntParam_BarHomogeneous));
  rb_define_const(rb_mGurobiIntParam, "PRE_Q_LINEARIZE", INT2FIX(GRB_IntParam_PreQLinearize));
  rb_define_const(rb_mGurobiIntParam, "MIQCP_METHOD", INT2FIX(GRB_IntParam_MIQCPMethod));
  rb_define_const(rb_mGurobiIntParam, "QCP_DUAL", INT2FIX(GRB_IntParam_QCPDual));
  rb_define_const(rb_mGurobiIntParam, "LOG_TO_CONSOLE", INT2FIX(GRB_IntParam_LogToConsole));
  rb_define_const(rb_mGurobiIntParam, "PRE_SPARSIFY", INT2FIX(GRB_IntParam_PreSparsify));
  rb_define_const(rb_mGurobiIntParam, "SEED", INT2FIX(GRB_IntParam_Seed));
  rb_define_const(rb_mGurobiIntParam, "NON_BLOCKING", INT2FIX(GRB_IntParam_NonBlocking));
  rb_define_const(rb_mGurobiIntParam, "CONCURRENT_MIP", INT2FIX(GRB_IntParam_ConcurrentMIP));
  rb_define_const(rb_mGurobiIntParam, "LAZY_CONSTRAINTS", INT2FIX(GRB_IntParam_LazyConstraints));
  rb_define_const(rb_mGurobiIntParam, "TUNE_RESULTS", INT2FIX(GRB_IntParam_TuneResults));
  rb_define_const(rb_mGurobiIntParam, "TUNE_TRIALS", INT2FIX(GRB_IntParam_TuneTrials));
  rb_define_const(rb_mGurobiIntParam, "TUNE_OUTPUT", INT2FIX(GRB_IntParam_TuneOutput));
}

static void
init_gurobi_double_param()
{
  rb_mGurobiDoubleParam = rb_define_module_under(rb_mGurobi, "DoubleParam");
  rb_define_const(rb_mGurobiDoubleParam, "CUTOFF", INT2FIX(GRB_DoubleParam_Cutoff));
  rb_define_const(rb_mGurobiDoubleParam, "ITERATION_LIMIT", INT2FIX(GRB_DoubleParam_IterationLimit));
  rb_define_const(rb_mGurobiDoubleParam, "NODE_LIMIT", INT2FIX(GRB_DoubleParam_NodeLimit));
  rb_define_const(rb_mGurobiDoubleParam, "TIME_LIMIT", INT2FIX(GRB_DoubleParam_TimeLimit));
  rb_define_const(rb_mGurobiDoubleParam, "FEASIBILITY_TOL", INT2FIX(GRB_DoubleParam_FeasibilityTol));
  rb_define_const(rb_mGurobiDoubleParam, "INT_FEAS_TOL", INT2FIX(GRB_DoubleParam_IntFeasTol));
  rb_define_const(rb_mGurobiDoubleParam, "MARKOWITZ_TOL", INT2FIX(GRB_DoubleParam_MarkowitzTol));
  rb_define_const(rb_mGurobiDoubleParam, "MIP_GAP", INT2FIX(GRB_DoubleParam_MIPGap));
  rb_define_const(rb_mGurobiDoubleParam, "MIP_GAP_ABS", INT2FIX(GRB_DoubleParam_MIPGapAbs));
  rb_define_const(rb_mGurobiDoubleParam, "OPTIMALITY_TOL", INT2FIX(GRB_DoubleParam_OptimalityTol));
  rb_define_const(rb_mGurobiDoubleParam, "PERTURB_VALUE", INT2FIX(GRB_DoubleParam_PerturbValue));
  rb_define_const(rb_mGurobiDoubleParam, "HEURISTICS", INT2FIX(GRB_DoubleParam_Heuristics));
  rb_define_const(rb_mGurobiDoubleParam, "OBJ_SCALE", INT2FIX(GRB_DoubleParam_ObjScale));
  rb_define_const(rb_mGurobiDoubleParam, "NODEFILE_START", INT2FIX(GRB_DoubleParam_NodefileStart));
  rb_define_const(rb_mGurobiDoubleParam, "BAR_CONV_TOL", INT2FIX(GRB_DoubleParam_BarConvTol));
  rb_define_const(rb_mGurobiDoubleParam, "BAR_QCP_CONV_TOL", INT2FIX(GRB_DoubleParam_BarQCPConvTol));
  rb_define_const(rb_mGurobiDoubleParam, "PSD_TOL", INT2FIX(GRB_DoubleParam_PSDTol));
  rb_define_const(rb_mGurobiDoubleParam, "IMPROVE_START_GAP", INT2FIX(GRB_DoubleParam_ImproveStartGap));
  rb_define_const(rb_mGurobiDoubleParam, "IMPROVE_START_TIME", INT2FIX(GRB_DoubleParam_ImproveStartTime));
  rb_define_const(rb_mGurobiDoubleParam, "FEAS_RELAX_BIG_M", INT2FIX(GRB_DoubleParam_FeasRelaxBigM));
  rb_define_const(rb_mGurobiDoubleParam, "TUNE_TIME_LIMIT", INT2FIX(GRB_DoubleParam_TuneTimeLimit));
}

static void
init_gurobi_string_param()
{
  rb_mGurobiStringParam = rb_define_module_under(rb_mGurobi, "StringParam");
  rb_define_const(rb_mGurobiStringParam, "LOG_FILE", INT2FIX(GRB_StringParam_LogFile));
  rb_define_const(rb_mGurobiStringParam, "NODEFILE_DIR", INT2FIX(GRB_StringParam_NodefileDir));
  rb_define_const(rb_mGurobiStringParam, "RESULT_FILE", INT2FIX(GRB_StringParam_ResultFile));
  rb_define_const(rb_mGurobiStringParam, "DUMMY", INT2FIX(GRB_StringParam_Dummy));
}

static void
init_gurobi_int_attr()
{
  rb_mGurobiIntAttr = rb_define_module_under(rb_mGurobi, "IntAttr");
  rb_define_const(rb_mGurobiIntAttr, "NUM_CONSTRS", INT2FIX(GRB_IntAttr_NumConstrs));
  rb_define_const(rb_mGurobiIntAttr, "NUM_VARS", INT2FIX(GRB_IntAttr_NumVars));
  rb_define_const(rb_mGurobiIntAttr, "NUM_SOS", INT2FIX(GRB_IntAttr_NumSOS));
  rb_define_const(rb_mGurobiIntAttr, "NUM_Q_CONSTRS", INT2FIX(GRB_IntAttr_NumQConstrs));
  rb_define_const(rb_mGurobiIntAttr, "NUM_N_ZS", INT2FIX(GRB_IntAttr_NumNZs));
  rb_define_const(rb_mGurobiIntAttr, "NUM_QN_ZS", INT2FIX(GRB_IntAttr_NumQNZs));
  rb_define_const(rb_mGurobiIntAttr, "NUM_QCN_ZS", INT2FIX(GRB_IntAttr_NumQCNZs));
  rb_define_const(rb_mGurobiIntAttr, "NUM_INT_VARS", INT2FIX(GRB_IntAttr_NumIntVars));
  rb_define_const(rb_mGurobiIntAttr, "NUM_BIN_VARS", INT2FIX(GRB_IntAttr_NumBinVars));
  rb_define_const(rb_mGurobiIntAttr, "MODEL_SENSE", INT2FIX(GRB_IntAttr_ModelSense));
  rb_define_const(rb_mGurobiIntAttr, "IS_MIP", INT2FIX(GRB_IntAttr_IsMIP));
  rb_define_const(rb_mGurobiIntAttr, "IS_QP", INT2FIX(GRB_IntAttr_IsQP));
  rb_define_const(rb_mGurobiIntAttr, "IS_QCP", INT2FIX(GRB_IntAttr_IsQCP));
  rb_define_const(rb_mGurobiIntAttr, "STATUS", INT2FIX(GRB_IntAttr_Status));
  rb_define_const(rb_mGurobiIntAttr, "SOL_COUNT", INT2FIX(GRB_IntAttr_SolCount));
  rb_define_const(rb_mGurobiIntAttr, "BAR_ITER_COUNT", INT2FIX(GRB_IntAttr_BarIterCount));
  rb_define_const(rb_mGurobiIntAttr, "V_BASIS", INT2FIX(GRB_IntAttr_VBasis));
  rb_define_const(rb_mGurobiIntAttr, "C_BASIS", INT2FIX(GRB_IntAttr_CBasis));
  rb_define_const(rb_mGurobiIntAttr, "BRANCH_PRIORITY", INT2FIX(GRB_IntAttr_BranchPriority));
  rb_define_const(rb_mGurobiIntAttr, "BOUND_VIO_INDEX", INT2FIX(GRB_IntAttr_BoundVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "BOUND_S_VIO_INDEX", INT2FIX(GRB_IntAttr_BoundSVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "CONSTR_VIO_INDEX", INT2FIX(GRB_IntAttr_ConstrVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "CONSTR_S_VIO_INDEX", INT2FIX(GRB_IntAttr_ConstrSVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "CONSTR_RESIDUAL_INDEX", INT2FIX(GRB_IntAttr_ConstrResidualIndex));
  rb_define_const(rb_mGurobiIntAttr, "CONSTR_S_RESIDUAL_INDEX", INT2FIX(GRB_IntAttr_ConstrSResidualIndex));
  rb_define_const(rb_mGurobiIntAttr, "DUAL_VIO_INDEX", INT2FIX(GRB_IntAttr_DualVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "DUAL_S_VIO_INDEX", INT2FIX(GRB_IntAttr_DualSVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "DUAL_RESIDUAL_INDEX", INT2FIX(GRB_IntAttr_DualResidualIndex));
  rb_define_const(rb_mGurobiIntAttr, "DUAL_S_RESIDUAL_INDEX", INT2FIX(GRB_IntAttr_DualSResidualIndex));
  rb_define_const(rb_mGurobiIntAttr, "COMPL_VIO_INDEX", INT2FIX(GRB_IntAttr_ComplVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "INT_VIO_INDEX", INT2FIX(GRB_IntAttr_IntVioIndex));
  rb_define_const(rb_mGurobiIntAttr, "IIS_MINIMAL", INT2FIX(GRB_IntAttr_IISMinimal));
  rb_define_const(rb_mGurobiIntAttr, "IISLB", INT2FIX(GRB_IntAttr_IISLB));
  rb_define_const(rb_mGurobiIntAttr, "IISUB", INT2FIX(GRB_IntAttr_IISUB));
  rb_define_const(rb_mGurobiIntAttr, "IIS_CONSTR", INT2FIX(GRB_IntAttr_IISConstr));
  rb_define_const(rb_mGurobiIntAttr, "IISSOS", INT2FIX(GRB_IntAttr_IISSOS));
  rb_define_const(rb_mGurobiIntAttr, "IISQ_CONSTR", INT2FIX(GRB_IntAttr_IISQConstr));
  rb_define_const(rb_mGurobiIntAttr, "TUNE_RESULT_COUNT", INT2FIX(GRB_IntAttr_TuneResultCount));
}

static void
init_gurobi_char_attr()
{
  rb_mGurobiCharAttr = rb_define_module_under(rb_mGurobi, "CharAttr");
  rb_define_const(rb_mGurobiCharAttr, "V_TYPE", INT2FIX(GRB_CharAttr_VType));
  rb_define_const(rb_mGurobiCharAttr, "SENSE", INT2FIX(GRB_CharAttr_Sense));
  rb_define_const(rb_mGurobiCharAttr, "QC_SENSE", INT2FIX(GRB_CharAttr_QCSense));
}

static void
init_gurobi_double_attr()
{
  rb_mGurobiDoubleAttr = rb_define_module_under(rb_mGurobi, "DoubleAttr");
  rb_define_const(rb_mGurobiDoubleAttr, "RUNTIME", INT2FIX(GRB_DoubleAttr_Runtime));
  rb_define_const(rb_mGurobiDoubleAttr, "OBJ_CON", INT2FIX(GRB_DoubleAttr_ObjCon));
  rb_define_const(rb_mGurobiDoubleAttr, "LB", INT2FIX(GRB_DoubleAttr_LB));
  rb_define_const(rb_mGurobiDoubleAttr, "UB", INT2FIX(GRB_DoubleAttr_UB));
  rb_define_const(rb_mGurobiDoubleAttr, "OBJ", INT2FIX(GRB_DoubleAttr_Obj));
  rb_define_const(rb_mGurobiDoubleAttr, "START", INT2FIX(GRB_DoubleAttr_Start));
  rb_define_const(rb_mGurobiDoubleAttr, "RHS", INT2FIX(GRB_DoubleAttr_RHS));
  rb_define_const(rb_mGurobiDoubleAttr, "QCRHS", INT2FIX(GRB_DoubleAttr_QCRHS));
  rb_define_const(rb_mGurobiDoubleAttr, "MAX_COEFF", INT2FIX(GRB_DoubleAttr_MaxCoeff));
  rb_define_const(rb_mGurobiDoubleAttr, "MIN_COEFF", INT2FIX(GRB_DoubleAttr_MinCoeff));
  rb_define_const(rb_mGurobiDoubleAttr, "MAX_BOUND", INT2FIX(GRB_DoubleAttr_MaxBound));
  rb_define_const(rb_mGurobiDoubleAttr, "MIN_BOUND", INT2FIX(GRB_DoubleAttr_MinBound));
  rb_define_const(rb_mGurobiDoubleAttr, "MAX_OBJ_COEFF", INT2FIX(GRB_DoubleAttr_MaxObjCoeff));
  rb_define_const(rb_mGurobiDoubleAttr, "MIN_OBJ_COEFF", INT2FIX(GRB_DoubleAttr_MinObjCoeff));
  rb_define_const(rb_mGurobiDoubleAttr, "MAX_RHS", INT2FIX(GRB_DoubleAttr_MaxRHS));
  rb_define_const(rb_mGurobiDoubleAttr, "MIN_RHS", INT2FIX(GRB_DoubleAttr_MinRHS));
  rb_define_const(rb_mGurobiDoubleAttr, "OBJ_VAL", INT2FIX(GRB_DoubleAttr_ObjVal));
  rb_define_const(rb_mGurobiDoubleAttr, "OBJ_BOUND", INT2FIX(GRB_DoubleAttr_ObjBound));
  rb_define_const(rb_mGurobiDoubleAttr, "MIP_GAP", INT2FIX(GRB_DoubleAttr_MIPGap));
  rb_define_const(rb_mGurobiDoubleAttr, "ITER_COUNT", INT2FIX(GRB_DoubleAttr_IterCount));
  rb_define_const(rb_mGurobiDoubleAttr, "NODE_COUNT", INT2FIX(GRB_DoubleAttr_NodeCount));
  rb_define_const(rb_mGurobiDoubleAttr, "X", INT2FIX(GRB_DoubleAttr_X));
  rb_define_const(rb_mGurobiDoubleAttr, "RC", INT2FIX(GRB_DoubleAttr_RC));
  rb_define_const(rb_mGurobiDoubleAttr, "PI", INT2FIX(GRB_DoubleAttr_Pi));
  rb_define_const(rb_mGurobiDoubleAttr, "QC_PI", INT2FIX(GRB_DoubleAttr_QCPi));
  rb_define_const(rb_mGurobiDoubleAttr, "SLACK", INT2FIX(GRB_DoubleAttr_Slack));
  rb_define_const(rb_mGurobiDoubleAttr, "QC_SLACK", INT2FIX(GRB_DoubleAttr_QCSlack));
  rb_define_const(rb_mGurobiDoubleAttr, "BOUND_VIO", INT2FIX(GRB_DoubleAttr_BoundVio));
  rb_define_const(rb_mGurobiDoubleAttr, "BOUND_S_VIO", INT2FIX(GRB_DoubleAttr_BoundSVio));
  rb_define_const(rb_mGurobiDoubleAttr, "BOUND_VIO_SUM", INT2FIX(GRB_DoubleAttr_BoundVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "BOUND_S_VIO_SUM", INT2FIX(GRB_DoubleAttr_BoundSVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_VIO", INT2FIX(GRB_DoubleAttr_ConstrVio));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_S_VIO", INT2FIX(GRB_DoubleAttr_ConstrSVio));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_VIO_SUM", INT2FIX(GRB_DoubleAttr_ConstrVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_S_VIO_SUM", INT2FIX(GRB_DoubleAttr_ConstrSVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_RESIDUAL", INT2FIX(GRB_DoubleAttr_ConstrResidual));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_S_RESIDUAL", INT2FIX(GRB_DoubleAttr_ConstrSResidual));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_RESIDUAL_SUM", INT2FIX(GRB_DoubleAttr_ConstrResidualSum));
  rb_define_const(rb_mGurobiDoubleAttr, "CONSTR_S_RESIDUAL_SUM", INT2FIX(GRB_DoubleAttr_ConstrSResidualSum));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_VIO", INT2FIX(GRB_DoubleAttr_DualVio));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_S_VIO", INT2FIX(GRB_DoubleAttr_DualSVio));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_VIO_SUM", INT2FIX(GRB_DoubleAttr_DualVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_S_VIO_SUM", INT2FIX(GRB_DoubleAttr_DualSVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_RESIDUAL", INT2FIX(GRB_DoubleAttr_DualResidual));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_S_RESIDUAL", INT2FIX(GRB_DoubleAttr_DualSResidual));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_RESIDUAL_SUM", INT2FIX(GRB_DoubleAttr_DualResidualSum));
  rb_define_const(rb_mGurobiDoubleAttr, "DUAL_S_RESIDUAL_SUM", INT2FIX(GRB_DoubleAttr_DualSResidualSum));
  rb_define_const(rb_mGurobiDoubleAttr, "COMPL_VIO", INT2FIX(GRB_DoubleAttr_ComplVio));
  rb_define_const(rb_mGurobiDoubleAttr, "COMPL_VIO_SUM", INT2FIX(GRB_DoubleAttr_ComplVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "INT_VIO", INT2FIX(GRB_DoubleAttr_IntVio));
  rb_define_const(rb_mGurobiDoubleAttr, "INT_VIO_SUM", INT2FIX(GRB_DoubleAttr_IntVioSum));
  rb_define_const(rb_mGurobiDoubleAttr, "KAPPA", INT2FIX(GRB_DoubleAttr_Kappa));
  rb_define_const(rb_mGurobiDoubleAttr, "KAPPA_EXACT", INT2FIX(GRB_DoubleAttr_KappaExact));
  rb_define_const(rb_mGurobiDoubleAttr, "SA_OBJ_LOW", INT2FIX(GRB_DoubleAttr_SAObjLow));
  rb_define_const(rb_mGurobiDoubleAttr, "SA_OBJ_UP", INT2FIX(GRB_DoubleAttr_SAObjUp));
  rb_define_const(rb_mGurobiDoubleAttr, "SALB_LOW", INT2FIX(GRB_DoubleAttr_SALBLow));
  rb_define_const(rb_mGurobiDoubleAttr, "SALB_UP", INT2FIX(GRB_DoubleAttr_SALBUp));
  rb_define_const(rb_mGurobiDoubleAttr, "SARHS_LOW", INT2FIX(GRB_DoubleAttr_SARHSLow));
  rb_define_const(rb_mGurobiDoubleAttr, "SAUB_LOW", INT2FIX(GRB_DoubleAttr_SAUBLow));
  rb_define_const(rb_mGurobiDoubleAttr, "SAUB_UP", INT2FIX(GRB_DoubleAttr_SAUBUp));
  rb_define_const(rb_mGurobiDoubleAttr, "SARHS_UP", INT2FIX(GRB_DoubleAttr_SARHSUp));
  rb_define_const(rb_mGurobiDoubleAttr, "XN", INT2FIX(GRB_DoubleAttr_Xn));
  rb_define_const(rb_mGurobiDoubleAttr, "FARKAS_PROOF", INT2FIX(GRB_DoubleAttr_FarkasProof));
  rb_define_const(rb_mGurobiDoubleAttr, "FARKAS_DUAL", INT2FIX(GRB_DoubleAttr_FarkasDual));
  rb_define_const(rb_mGurobiDoubleAttr, "UNBD_RAY", INT2FIX(GRB_DoubleAttr_UnbdRay));
  rb_define_const(rb_mGurobiDoubleAttr, "P_START", INT2FIX(GRB_DoubleAttr_PStart));
  rb_define_const(rb_mGurobiDoubleAttr, "D_START", INT2FIX(GRB_DoubleAttr_DStart));
}


static void
init_gurobi_string_attr()
{
  rb_mGurobiStringAttr = rb_define_module_under(rb_mGurobi, "StringAttr");
  rb_define_const(rb_mGurobiStringAttr, "MODEL_NAME", INT2FIX(GRB_StringAttr_ModelName));
  rb_define_const(rb_mGurobiStringAttr, "VAR_NAME", INT2FIX(GRB_StringAttr_VarName));
  rb_define_const(rb_mGurobiStringAttr, "CONSTR_NAME", INT2FIX(GRB_StringAttr_ConstrName));
  rb_define_const(rb_mGurobiStringAttr, "QC_NAME", INT2FIX(GRB_StringAttr_QCName));
}

#ifdef commentout

ID idGRB_INT_ATTR_MODELSENSE;
ID idGRB_INT_ATTR_STATUS;
ID idGRB_INT_PAR_METHOD;
ID idGRB_DBL_ATTR_START;
ID idGRB_DBL_ATTR_X;
ID idGRB_DBL_ATTR_OBJVAL;
ID idGRB_STR_ATTR_VARNAME;
ID idGRB_STR_ATTR_CONSTRNAME;

static void
init_gurobi_symbol()
{
  idGRB_INT_ATTR_MODELSENSE   = rb_intern(GRB_INT_ATTR_MODELSENSE);
  idGRB_INT_ATTR_STATUS       = rb_intern(GRB_INT_ATTR_STATUS);
  idGRB_INT_PAR_METHOD        = rb_intern(GRB_INT_PAR_METHOD);
  idGRB_DBL_ATTR_START        = rb_intern(GRB_DBL_ATTR_START);
  idGRB_DBL_ATTR_X            = rb_intern(GRB_DBL_ATTR_X);
  idGRB_DBL_ATTR_OBJVAL       = rb_intern(GRB_DBL_ATTR_OBJVAL);
  idGRB_STR_ATTR_VARNAME      = rb_intern(GRB_STR_ATTR_VARNAME);
  idGRB_STR_ATTR_CONSTRNAME   = rb_intern(GRB_STR_ATTR_CONSTRNAME);
}
#endif

VALUE
gurobi_sym(const char *str)
{
  return ID2SYM(rb_intern(str));
}

VALUE
gurobi_sym2(ID sym)
{
  return ID2SYM(sym);
}

extern "C" void
Init_gurobi()
{
#ifndef debug
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
#endif

  rb_mGurobi = rb_define_module("Gurobi");
  init_gurobi_const();
//init_gurobi_symbol();
  init_gurobi_env();
  init_gurobi_model();
  init_gurobi_var();
  init_gurobi_expr();
  init_gurobi_lin_expr();
  init_gurobi_quad_expr();
  init_gurobi_constr();
  init_gurobi_column();
  init_gurobi_qconstr();
  init_gurobi_sos();
  init_gurobi_temp_constr();

  init_gurobi_int_param();
  init_gurobi_double_param();
  init_gurobi_string_param();

  init_gurobi_int_attr();
  init_gurobi_char_attr();
  init_gurobi_double_attr();
  init_gurobi_string_attr();
}

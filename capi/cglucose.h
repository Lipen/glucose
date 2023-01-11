#ifndef Glucose_C_Bindings_h
#define Glucose_C_Bindings_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct CGlucose CGlucose;
typedef int glucose_Var;
typedef int glucose_Lit;
typedef int glucose_lbool;

extern const glucose_lbool glucose_l_True;
extern const glucose_lbool glucose_l_False;
extern const glucose_lbool glucose_l_Undef;

glucose_lbool glucose_get_l_True(void);
glucose_lbool glucose_get_l_False(void);
glucose_lbool glucose_get_l_Undef(void);

const char *glucose_signature(void);
CGlucose *glucose_init(void);
void glucose_release(CGlucose *s);

// Variables/Literals:
glucose_Var glucose_newVar(CGlucose *s);
glucose_Lit glucose_newLit(CGlucose *s);

glucose_Lit glucose_mkLit(glucose_Var x);
glucose_Lit glucose_mkLit_args(glucose_Var x, int sign);
glucose_Lit glucose_negate(glucose_Lit p);
glucose_Var glucose_var(glucose_Lit p);
bool glucose_sign(glucose_Lit p);

void glucose_setPolarity(CGlucose *s, glucose_Var v, bool b);
void glucose_setDecisionVar(CGlucose *s, glucose_Var v, bool b);
void glucose_setFrozen(CGlucose *s, glucose_Var v, bool b);
bool glucose_isEliminated(CGlucose *s, glucose_Var v);
bool glucose_eliminate(CGlucose *s, bool turn_off_elim);

// Clause:
bool glucose_addClause(CGlucose *s, int len, glucose_Lit *ps);
void glucose_addClause_begin(CGlucose *s);
void glucose_addClause_addLit(CGlucose *s, glucose_Lit p);
bool glucose_addClause_commit(CGlucose *s);

bool glucose_simplify(CGlucose *s);

bool glucose_solve(CGlucose *s, int len, glucose_Lit *ps);
glucose_lbool glucose_limited_solve(CGlucose *s, int len, glucose_Lit *ps);
void glucose_solve_begin(CGlucose *s);
void glucose_solve_addLit(CGlucose *s, glucose_Lit p);
bool glucose_solve_commit(CGlucose *s);
glucose_lbool glucose_limited_solve_commit(CGlucose *s);

bool glucose_okay(CGlucose *s);

glucose_lbool glucose_value_Var(CGlucose *s, glucose_Var x);
glucose_lbool glucose_value_Lit(CGlucose *s, glucose_Lit p);
glucose_lbool glucose_modelValue_Var(CGlucose *s, glucose_Var x);
glucose_lbool glucose_modelValue_Lit(CGlucose *s, glucose_Lit p);

int glucose_conflict_len(CGlucose *s);
glucose_Lit glucose_conflict_nthLit(CGlucose *s, int i);

void glucose_set_conf_budget(CGlucose *s, int x);
void glucose_set_prop_budget(CGlucose *s, int x);
void glucose_no_budget(CGlucose *s);

void glucose_interrupt(CGlucose *s);
void glucose_clearInterrupt(CGlucose *s);

void glucose_setIncremental(CGlucose *s);
bool glucose_isIncremental(CGlucose *s);
void glucose_set_verbosity(CGlucose *s, int v);
int glucose_get_verbosity(CGlucose *s, int v);

int glucose_num_assigns(CGlucose *s);
int glucose_num_clauses(CGlucose *s);
int glucose_num_learnts(CGlucose *s);
int glucose_num_vars(CGlucose *s);
int glucose_num_freeVars(CGlucose *s);
int64_t glucose_num_conflicts(CGlucose *s);
int64_t glucose_num_decisions(CGlucose *s);
int64_t glucose_num_restarts(CGlucose *s);
int64_t glucose_num_propagations(CGlucose *s);

#ifdef __cplusplus
}
#endif

#endif

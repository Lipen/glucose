#include <stdlib.h>

#include "simp/SimpSolver.h"

using namespace Glucose;

struct CGlucose : public SimpSolver {
    vec<Lit> clause;
    vec<Lit> assumps;
};

extern "C" {

#include "cglucose.h"

// Note:
//  `glucose_Var` (=`int`) can be converted to `Glucose::Var` (=`int`) via `mkLit`
//  `glucose_Lit` (=`int`) can be converted to `Glucose::Lit` (`struct`) via `toLit`

// This implementation of lbool may or not may be an exact mirror of the C++ implementation:
//
extern const glucose_lbool glucose_l_True = 1;
extern const glucose_lbool glucose_l_False = 0;
extern const glucose_lbool glucose_l_Undef = -1;

static inline glucose_lbool toC(lbool a) {
    return a == l_True    ? glucose_l_True
           : a == l_False ? glucose_l_False
                          : glucose_l_Undef;
}

static inline lbool fromC(glucose_lbool a) {
    return a == glucose_l_True    ? l_True
           : a == glucose_l_False ? l_False
                                  : l_Undef;
}

glucose_lbool glucose_get_l_True(void) { return glucose_l_True; }
glucose_lbool glucose_get_l_False(void) { return glucose_l_False; }
glucose_lbool glucose_get_l_Undef(void) { return glucose_l_Undef; }

const char *glucose_signature(void) { return "glucose"; }
CGlucose *glucose_init(void) { return new CGlucose(); }
void glucose_release(CGlucose *s) { delete s; }

glucose_Var glucose_newVar(CGlucose *s) { return s->newVar(); }
glucose_Lit glucose_newLit(CGlucose *s) { return toInt(mkLit(s->newVar())); }

glucose_Lit glucose_mkLit(glucose_Var x) { return toInt(mkLit(x)); }
glucose_Lit glucose_mkLit_args(glucose_Var x, int sign) { return toInt(mkLit(x, sign)); }
glucose_Lit glucose_negate(glucose_Lit p) { return toInt(~toLit(p)); }
glucose_Var glucose_var(glucose_Lit p) { return var(toLit(p)); }
bool glucose_sign(glucose_Lit p) { return sign(toLit(p)); }

void glucose_setPolarity(CGlucose *s, glucose_Var v, bool b) { s->setPolarity(v, b); }
void glucose_setDecisionVar(CGlucose *s, glucose_Var v, bool b) { s->setDecisionVar(v, b); }

void glucose_setFrozen(CGlucose *s, glucose_Var v, bool b) { s->setFrozen(v, b); }
bool glucose_isEliminated(CGlucose *s, glucose_Var v) { return s->isEliminated(v); }
bool glucose_eliminate(CGlucose *s, bool turn_off_elim) { return s->eliminate(turn_off_elim); }

void glucose_addClause_begin(CGlucose *s) { s->clause.clear(); }
void glucose_addClause_addLit(CGlucose *s, glucose_Lit p) { s->clause.push(toLit(p)); }
bool glucose_addClause_commit(CGlucose *s) { return s->addClause_(s->clause); }

bool glucose_simplify(CGlucose *s) { return s->simplify(); }

// NOTE: Currently these run with default settings for implicitly calling preprocessing. Turn off
// before if you don't need it. This may change in the future.
void glucose_solve_begin(CGlucose *s) { s->assumps.clear(); }
void glucose_solve_addLit(CGlucose *s, glucose_Lit p) { s->assumps.push(toLit(p)); }
bool glucose_solve_commit(CGlucose *s) { return s->solve(s->assumps); }
glucose_lbool glucose_limited_solve_commit(CGlucose *s) { return toC(s->solveLimited(s->assumps)); }

bool glucose_okay(CGlucose *s) { return s->okay(); }

glucose_lbool glucose_value_Var(CGlucose *s, glucose_Var x) { return toC(s->value(x)); }
glucose_lbool glucose_value_Lit(CGlucose *s, glucose_Lit p) { return toC(s->value(toLit(p))); }
glucose_lbool glucose_modelValue_Var(CGlucose *s, glucose_Var x) { return toC(s->modelValue(x)); }
glucose_lbool glucose_modelValue_Lit(CGlucose *s, glucose_Lit p) { return toC(s->modelValue(toLit(p))); }

int glucose_conflict_len(CGlucose *s) { return s->conflict.size(); }
glucose_Lit glucose_conflict_nthLit(CGlucose *s, int i) { return toInt(s->conflict[i]); }

void glucose_set_conf_budget(CGlucose *s, int x) { s->setConfBudget(x); }
void glucose_set_prop_budget(CGlucose *s, int x) { s->setPropBudget(x); }
void glucose_no_budget(CGlucose *s) { s->budgetOff(); }

void glucose_interrupt(CGlucose *s) { s->interrupt(); }
void glucose_clearInterrupt(CGlucose *s) { s->clearInterrupt(); }

void glucose_setIncremental(CGlucose *s) { s->setIncrementalMode(); }
bool glucose_isIncremental(CGlucose *s) { return s->isIncremental(); }
void glucose_set_verbosity(CGlucose *s, int v) { s->verbosity = v; }
int glucose_get_verbosity(CGlucose *s) { return s->verbosity; }
void glucose_set_random_var_freq(CGlucose *s, double random_var_freq) { s->random_var_freq = random_var_freq; }
void glucose_set_random_seed(CGlucose *s, double random_seed) { s->random_seed = random_seed; }

int glucose_num_assigns(CGlucose *s) { return s->nAssigns(); }
int glucose_num_clauses(CGlucose *s) { return s->nClauses(); }
int glucose_num_learnts(CGlucose *s) { return s->nLearnts(); }
int glucose_num_vars(CGlucose *s) { return s->nVars(); }
int glucose_num_freeVars(CGlucose *s) { return s->nFreeVars(); }
int64_t glucose_num_conflicts(CGlucose *s) { return (int64_t)s->conflicts; }
int64_t glucose_num_decisions(CGlucose *s) { return (int64_t)s->decisions; }
int64_t glucose_num_restarts(CGlucose *s) { return (int64_t)s->starts; }
int64_t glucose_num_propagations(CGlucose *s) { return (int64_t)s->propagations; }

// Convenience functions for actual c-programmers (not language interfacing people):
//
bool glucose_solve(CGlucose *s, int len, glucose_Lit *ps) {
    glucose_solve_begin(s);
    for (int i = 0; i < len; i++)
        glucose_solve_addLit(s, ps[i]);
    return glucose_solve_commit(s);
}

glucose_lbool glucose_limited_solve(CGlucose *s, int len, glucose_Lit *ps) {
    glucose_solve_begin(s);
    for (int i = 0; i < len; i++)
        glucose_solve_addLit(s, ps[i]);
    return glucose_limited_solve_commit(s);
}

bool glucose_addClause(CGlucose *s, int len, glucose_Lit *ps) {
    glucose_addClause_begin(s);
    for (int i = 0; i < len; i++)
        glucose_addClause_addLit(s, ps[i]);
    return glucose_addClause_commit(s);
}
}

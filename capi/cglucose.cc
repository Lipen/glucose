#include "mtl/XAlloc.h"
#include "simp/SimpSolver.h"

namespace Glucose {

struct Wrapper {
    SimpSolver *solver;
    vec<Lit> clause;
    vec<Lit> assumptions;

    Wrapper() : solver(new SimpSolver()) {}

    ~Wrapper() { delete solver; }
};

}  // namespace Glucose

using namespace Glucose;

extern "C" {

#include "cglucose.h"

const char *cglucose_signature(void) {
    return "Glucose 4.2.1";
}

CGlucose *cglucose_init(void) {
    return (CGlucose *)new Wrapper();
}

void cglucose_release(CGlucose *handle) {
    delete (Wrapper *)handle;
}

static inline Lit convertLit(int lit) {
    // Note: `lit` is 1-based (external).
    return (lit > 0) ? mkLit(lit - 1) : mkLit(-lit - 1, true);
}

static inline Var convertVar(int lit) {
    // Note: `lit` is 1-based (external).
    return abs(lit) - 1;
}

static void ensure_vars(Wrapper *wrapper, int nvars) {
    while (nvars > wrapper->solver->nVars()) {
        wrapper->solver->newVar();
    }
}

void cglucose_add(CGlucose *handle, int lit) {
    // Note: `lit` is 1-based, 0 means end of clause.
    Wrapper *wrapper = (Wrapper *)handle;
    if (lit) {
        ensure_vars(wrapper, abs(lit));
        wrapper->clause.push(convertLit(lit));
    } else {
        wrapper->solver->addClause(wrapper->clause);
        wrapper->clause.clear();
    }
}

void cglucose_assume(CGlucose *handle, int lit) {
    // Note: `lit` is 1-based (external).
    Wrapper *wrapper = (Wrapper *)handle;
    wrapper->assumptions.push(convertLit(lit));
}

int cglucose_solve(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    lbool res = wrapper->solver->solveLimited(wrapper->assumptions);
    wrapper->assumptions.clear();
    if (res == l_True) {
        return 10;  // SAT
    }
    if (res == l_False) {
        return 20;  // UNSAT
    }
    return 0;  // UNKNOWN
}

int cglucose_val(CGlucose *handle, int lit) {
    // Note: `lit` is 1-based (external).
    Wrapper *wrapper = (Wrapper *)handle;
    Var v = convertVar(lit);  // Note: `v` is 0-based (internal).
    lbool val = wrapper->solver->modelValue(v);
    // Note: result is 1-based, or 0 if unassigned.
    if (val == l_True) {
        return v + 1;  // TRUE
    }
    if (val == l_False) {
        return -(v + 1);  // FALSE
    }
    return 0;  // UNKNOWN
}

bool cglucose_failed(CGlucose *handle, int lit) {
    Wrapper *wrapper = (Wrapper *)handle;
    for (int i = 0; i < wrapper->solver->conflict.size(); ++i) {
        if (wrapper->solver->conflict[i] == convertLit(lit)) {
            return true;  // The literal is in the conflict clause.
        }
    }
    return false;  // The literal is not in the conflict clause.
}

bool cglucose_eliminate(CGlucose *handle, bool turn_off_elim) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->eliminate(turn_off_elim);
}

void cglucose_interrupt(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    wrapper->solver->interrupt();
}
void cglucose_clear_interrupt(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    wrapper->solver->clearInterrupt();
}

void cglucose_set_polarity(CGlucose *handle, int var, bool polarity) {
    // Note: `var` is 1-based (external).
    Wrapper *wrapper = (Wrapper *)handle;
    Var v = var - 1;
    wrapper->solver->setPolarity(v, polarity);
}
void cglucose_set_decision_var(CGlucose *handle, int var, bool decision) {
    // Note: `var` is 1-based (external).
    Wrapper *wrapper = (Wrapper *)handle;
    Var v = var - 1;
    wrapper->solver->setDecisionVar(v, decision);
}
void cglucose_set_frozen(CGlucose *handle, int var, bool frozen) {
    // Note: `var` is 1-based (external).
    Wrapper *wrapper = (Wrapper *)handle;
    Var v = var - 1;
    wrapper->solver->setFrozen(v, frozen);
}
bool cglucose_is_eliminated(CGlucose *handle, int var) {
    // Note: `var` is 1-based (external).
    Wrapper *wrapper = (Wrapper *)handle;
    Var v = var - 1;
    return wrapper->solver->isEliminated(v);
}

void cglucose_set_conf_limit(CGlucose *handle, int64_t limit) {
    Wrapper *wrapper = (Wrapper *)handle;
    wrapper->solver->setConfBudget(limit);
}
void cglucose_set_prop_limit(CGlucose *handle, int64_t limit) {
    Wrapper *wrapper = (Wrapper *)handle;
    wrapper->solver->setPropBudget(limit);
}
void cglucose_reset_limits(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    wrapper->solver->budgetOff();
}

int cglucose_num_vars(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->nVars();
}
int cglucose_num_clauses(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->nClauses();
}
int cglucose_num_learnts(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->nLearnts();
}
int cglucose_num_assigns(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->nAssigns();
}

uint64_t cglucose_num_conflicts(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->conflicts;
}
uint64_t cglucose_num_restarts(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->starts;
}
uint64_t cglucose_num_decisions(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->decisions;
}
uint64_t cglucose_num_propagations(CGlucose *handle) {
    Wrapper *wrapper = (Wrapper *)handle;
    return wrapper->solver->propagations;
}

}  // extern "C"

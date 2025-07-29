#ifndef Glucose_C_Bindings_h
#define Glucose_C_Bindings_h

#ifdef __cplusplus
    /* in case the compiler is a C++ compiler */
    #define DEFAULT(value) = value
#else
    /* otherwise, C compiler, do nothing */
    #define DEFAULT(value)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct CGlucose CGlucose;

const char *cglucose_signature(void);

CGlucose *cglucose_init(void);
void cglucose_release(CGlucose *);

void cglucose_add(CGlucose *, int lit);
void cglucose_assume(CGlucose *, int lit);
int cglucose_solve(CGlucose *);
int cglucose_val(CGlucose *, int lit);
bool cglucose_failed(CGlucose *, int lit);

bool cglucose_eliminate(CGlucose *, bool turn_off_elim DEFAULT(false));

void cglucose_interrupt(CGlucose *);
void cglucose_clear_interrupt(CGlucose *);

void cglucose_set_polarity(CGlucose *, int var, bool polarity);
void cglucose_set_decision_var(CGlucose *, int var, bool decision);
void cglucose_set_frozen(CGlucose *, int var, bool frozen);
bool cglucose_is_eliminated(CGlucose *, int var);

void cglucose_set_conf_limit(CGlucose *, int64_t limit);
void cglucose_set_prop_limit(CGlucose *, int64_t limit);
void cglucose_reset_limits(CGlucose *);

int cglucose_num_vars(CGlucose *);
int cglucose_num_clauses(CGlucose *);
int cglucose_num_learnts(CGlucose *);
int cglucose_num_assigns(CGlucose *);

uint64_t cglucose_num_conflicts(CGlucose *);
uint64_t cglucose_num_restarts(CGlucose *);
uint64_t cglucose_num_decisions(CGlucose *);
uint64_t cglucose_num_propagations(CGlucose *);

#ifdef __cplusplus
}
#endif

#endif // ifndef

#ifndef DUAL_ACC_H

#define DUAL_ACC_H

#include "maillage_acc.h"

SListeD* constr_dual(Maillage *m, Param_maillage *param);
void free_dual(Maillage *m);

#endif

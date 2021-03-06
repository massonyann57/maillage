#ifndef CALL_SING_STRUCT_H

#define CALL_SING_STRUCT_H

#include "../../outils/listes/listeD/listeD_struct.h"
#include "../newton_maill/newton_maill_struct.h"

typedef struct PARAM_CALL_SING{
  PListeD pointes[2*NB_BRANCHES_MAX];
  double eps;
  int dir;
  int dir_sing;
  double psi[NB_BRANCHES_MAX];
  PPt pts[NB_BRANCHES_MAX];
  int ind;
  int nbpts[NB_BRANCHES_MAX];

  PPPt new_pts[NB_BRANCHES_MAX];
  Pint ind_npts[NB_BRANCHES_MAX];
  Param_new2 *param_new;
  Param_maillage *param_m;
  Surf_param *s;
}Param_call_sing;

#endif

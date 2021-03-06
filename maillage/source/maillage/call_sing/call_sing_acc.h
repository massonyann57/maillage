#ifndef CALL_SING_ACC_H

#define CALL_SING_ACC_H

#include "call_sing_struct.h"
#include "../maillage_struct.h"
#include "../../surface/surface_struct.h"

Param_call_sing* 
create_param_call_sing(ListeD *pointe, 
		       double eps, int dir, 
		       int dir_sing, int nb_dual, 
		       int *nbpts, PPt *pts_prec, 
		       PPPt *new_pts, Pint *ind_npts,
		       Param_maillage *param_m, 
		       Surf_param *s, double h);
void free_param_call_sing(Param_call_sing *p);
void callback_sing(Maillage *m, int nb_dual, int *bool_term, void *param);

#endif


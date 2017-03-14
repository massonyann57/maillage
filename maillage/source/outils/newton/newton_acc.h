#ifndef NEWTON_ACC_H

#define NEWTON_ACC_H

#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"

int newton(P3D init, Newton_f f, Newton_f_p f_p, void *param_f, 
	   Mapping_new depl, void *param_map,
	   double delta, double del2, P3D res,
	   Param_maillage *p_new, int dim);
int invmat(P3D* A, double *b, double *res, int dim, double tol);


#endif

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <complex.h>

#include "../../outils/functions/functions_acc.h"
#include "../../outils/functions/functions_struct.h"
#include "../../parametre/global_def.h"
#include "../../outils/general_functions_acc.h"
#include "fonc_surf_param_struct.h"

/*__inline */ void surf_sphere_param(P3D x, void *dummy, P3D res);
/*__inline */ void surf_sphere_param_p(P3D x, void *dummy, P3D *res);
/*__inline */ void surf_sphere_param_p2(P3D x, void *dummy, P3D *res);
/*__inline */ void surf_sphere_param_p3(P3D x, void *dummy, P3D *res);

/*__inline */ void surf_tore_param(double* x, void *par, double* res);
/*__inline */ void surf_tore_param_p(double* x, void *par, P3D *res);

/*__inline */ void surf_catenoid_param(double* x, 
				 void *dummy, double* res);
/*__inline */ void surf_catenoid_param_p(double* x, 
				  void *dummy, P3D *res);
/*__inline */ void surf_catenoid_param_p2(double* x, 
				  void *dummy, P3D *res);
/*__inline */ void surf_catenoid_param_p3(double* x, 
				  void *dummy, P3D *res);

/*__inline */ void surf_catenoid2_param(double* x, 
				 void *dummy, double* res);
/*__inline */ void surf_catenoid2_param_p(double* x, 
				  void *dummy, P3D *res);
/*__inline */ void surf_catenoid2_param_p2(double* x, 
				  void *dummy, P3D *res);
/*__inline */ void surf_catenoid2_param_p3(double* x, 
				  void *dummy, P3D *res);

/*__inline */ void plan_param(P3D x, void *dummy, P3D res);
/*__inline */ void plan_param_p(P3D x, void *dummy, P3D *res);

/*__inline */ void plan_param2(P3D x, void *par, P3D res);
/*__inline */ void plan_param2_p(P3D x, void *par, P3D *res);

/*__inline */ void surf_trinoid_param(P3D x, void *dummy, P3D res);
/*__inline */ void surf_trinoid_param_p(P3D x, void *dummy, P3D *res);

void surf_enneper_param(P3D x, void *dummy, P3D res);
void surf_enneper_param_p(P3D x, void *dummy, P3D *res);
void surf_enneper_param_p2(P3D x, void *par, P3D *res);
void surf_enneper_param_p3(P3D x, void *par, P3D *res);

/*__inline */ void surf_tmp_param(double* x, void *par, double* res);
/*__inline */ void surf_tmp_param_p(double* X, void *par, P3D *res);

/*__inline */ void surf_cube_param(P3D x, void *par, P3D res);
/*__inline */ void surf_cube_param_p(P3D x, void *par, P3D* res);

void surf_boite_oeuf_param(P3D x, void *par, P3D res);
void surf_boite_oeuf_param_p(P3D x, void *par, P3D* res);

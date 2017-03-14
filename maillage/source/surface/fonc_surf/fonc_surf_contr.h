#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../outils/functions/functions_acc.h"
#include "../../outils/functions/functions_struct.h"
#include "../../parametre/global_def.h"
#include "../../outils/general_functions_acc.h"

double surf_pot(double* x, void *par);
void surf_pot_p(double *x, void *par, double *res);
void surf_pot_p2(double *x, void *par, double *res);


double surf_tore(double* x, void *par);
void surf_tore_p(double* X, void *par, double *res);
void surf_tore_p2(double* X, void *par, double *res);

double surf_goursat(double * x, void *par);
void surf_goursat_p(double *x, void *par, double *res);
void surf_goursat_p2(double *x, void *par, double *res);

double surf_hyperb(double *x, void *par);
void surf_hyperb_p(double *x, void *par,
			  double *res);
void surf_hyperb_p2(double *x, void *par,
			   double *res);

double surf_sphere(double* x, void *par);
void surf_sphere_p(double* x, void *par, double *res);
void surf_sphere_p2(double* x, void *par, double* res);
   
double surf_tore2T(double* x, void *par);
void surf_tore2T_p(double *X, void *par, double *res);
void surf_tore2T_p2(double *X, void *par, double *res);

double boule_enfoncee(double* x, void *par);
void boule_enfoncee_p(double* X, void *par, double *res);
void boule_enfoncee_p2(double* X, void *par, double *res);

double conoide(double* x, void *par);
void conoide_p(double* x, void *par, double *res);
void conoide_p2(double* x, void *par, double *res);

double plan(double* x, void *par);
void plan_p(double* x, void *par, double *res);
void plan_p2(double* x, void *par, double *res);


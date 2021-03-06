#ifndef NEWTON_MAILL_ACC_H

#define NEWTON_MAILL_ACC_H

#include "newton_maill_struct.h"
#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"

Param_new1*
create_param_new1(P3D c1, P3D c2, double R1, double R2,
		  Surf_param *s, Param_maillage *param);
Param_new2*
create_param_new2(P3D pos, P3D* base, P3D centre, double R,
		  double theta, Surf_param *s);
Param_new3*
create_param_new3(CourbeP *c, double R, P3D pos);
Param_new4*
create_param_new4(Pt* pos, P3D* dir, double R1, 
		  double R2, Surf_param *s, 
		  Param_new2 *p_inter1, 
		  Param_new2 *p_inter2, Param_new5 *p_inter3, 
		  Param_maillage *param_m, Pt *inter1, 
		  Pt *inter2, Pt *inter_f);
Param_new5*
create_param_new5(P3D pos1, P3D pos2, P3D nor1, P3D nor2,
		  Surf_param *s);

void f_newton_contr_courbe(double* x, void *p, double *res);
void f_newton_contr_courbe_p(double* x, void *p, P3D* res, 
			     double *grad);
void f_newton_contr_surf(double* x, void *p, double *res);
void f_newton_contr_surf_p(double* x, void *p, P3D* res, double *grad);

void f_newton_param_courbe(double* x, void *p, double *res);
void f_newton_param_courbe_p(double* x, void *p, P3D* res, 
			     double *grad);
void f_newton_param_surf(double* x, void *p, double *res);
void f_newton_param_surf_p(double* x, void *p, P3D* res, double *grad);

void f_newton_param_ligne(P3D x, void *p, double *res);
void f_newton_param_ligne_p(P3D x, void *p, double res[3][3],
			    double *grad);

void f_newton_param_courbe_sing(P3D x, void *p, double *res);
void f_newton_param_courbe_sing_p(P3D x, void *p, double res[3][3],
				  double *grad);

void f_newton_contr_inter_plan(P3D x, void *p, double *res);
void f_newton_contr_inter_plan_p(P3D x, void *p, double res[3][3],
				 double *grad);

void f_newton_param_inter_plan(P3D x, void *p, double *res);
void f_newton_param_inter_plan_p(P3D x, void *p, double res[3][3],
				 double *grad);

#endif

#ifndef SURFACE_ACC_H

#define SURFACE_ACC_H

#include "../parametre/global_def.h"
#include "surface_struct.h"
#include "../parametre/parametre_maillage/parametre_maillage_acc.h"
#include "../courbe/courbe_struct.h"

Surf* create_surf(int type, Champ_vect* f_param, 
		  double init1, double fin1, 
		  double init2, double fin2, 
		  int ind_quotient,
		  Champ_r* f_contr);
void get_normal_surf_contr(Surf_contr *s, P3D pt, P3D normal);
void surf_get_normal(Surf_param *s, P3D param, P3D normal);

void surf_projeter_tangent(Surf_param *s, Pt* pt,  
			   P3D dir, P3D res);

int surf_calc_base(Surf_param *s, P3D pt, 
		   P3D base[2], P3D metrique[2]);
int surf_calc_base_opt(Surf_param *s, P3D pt, 
		      P3D base[2], P3D metrique[2]);
void surf_get_bon(Surf_param *s, P3D param, 
		 P3D base[2]);
int surf_calc_bon_dir(Surf_param *s, Pt* pt,
		      P3D dir, P3D res[2], int or_base);

int surf_setpt(Surf_param *s, Pt* p1, P3D param);

int surf_est_dans_borne(Surf_param *s, P3D p);
void surf_proj_borne(Surf_param *s, P3D p, P3D res);
int surf_deplacer(Surf_param *s, 
		  P3D v1, P3D v2, int sgn);
int surf_set_pos(Surf_param *s, P3D param_pos, P3D res);

void surf_coordonne_base_init(Surf_param *s, P3D param, 
			      P3D vect, P3D res);
void surf_export_curvature(Surf_param *s, Pt *pts, int nb, char *path);
void surf_set_init_courbe(Surf *s, Courbe *c, double in);

CourbeP* surf_get_courbe_from_courbeP(Surf_param *s, 
				      CourbeP *c_plan);
int surf_newton(Surf_param *s, P3D init, Newton_f f, 
		Newton_f_p f_p, void *param,
		double delta, double del2, P3D res,
		Param_maillage *p_new, int dim);
void surf_rotation_vect(Surf_param *s, Pt *p, P3D dir, 
			double the, P3D res);
void free_surf(Surf *s);
void free_surface_param(Surf_param *s);
double surf_get_curv(Surf_param *s, P3D param);
void surf_init_curv(Surf_param *s);

#endif

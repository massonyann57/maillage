#ifndef FONCTION_COURBE_PARAM_ACC

#define FONCTION_COURBE_PARAM_ACC

Fonc_vect* create_fonction_ellipse(double a, double b, 
				   double r1, double r2);
Fonc_vect* create_fonction_droite_surf_param(Surf_param *s, 
					    int bool, double l2);
Fonc_vect* create_fonction_courbe_surf_param(Surf_param *s, 
					     CourbeP *c);
Fonc_vect* create_fonction_mult_courbes(PCourbeP *c, int nb);
Fonc_vect* create_fonction_bezier(P3D *pts, P3D *der, 
				  double h);
Fonc_vect* create_fonction_carre(double d1, double d2);
Fonc_vect* create_fonction_triangle(double d1, double d2);

void set_pts_fonc_bezier(Fonc_vect *bez, P3D pts[2], 
			 P3D der[2], double h);
void ajouter_pt_fonc_bezier(Fonc_vect *fonc_bez, double h);
void homothetie_der_fonc_bezier(Fonc_vect *fonc_bez, double lam);
void homothetie_pt_inter_fonc_bezier(Fonc_vect *fonc_bez, double lam);

CourbeP* split_courbe_bezier(CourbeP *courbe, int pos, 
			     double h, double prop);

void fonction_ellipse(double d, void *param, P3D res);
void fonction_ellipse_p(double d, void *param, P3D res);
#endif

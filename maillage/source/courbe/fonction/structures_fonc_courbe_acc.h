#ifndef STRUCTURES_FONC_COURBE_ACC

#define STRUCTURES_FONC_COURBE_ACC

#include "structures_fonc_courbe_struct.h"

Courbe_surf_param* create_droite_surf_param(Surf_param *s, 
					  int bool,
					  double l2);
Courbe_surf_param* create_courbe_surf_param(Surf_param *s, 
					   CourbeP *c);
Bezier* createBezier(P3D *pts, int n);
Mult_courbes* create_mult_courbes(int nb, 
				  PCourbeP *courbes, 
				  double init, double fin);
NewMult_courbes* create_newMult_courbes(Mult_courbes *c, int pos);

Courbe_surf_param* create_bezier_plan(P3D *pts,
				      P3D *der_pts, int nb);

void set_pts_bezier(Bezier *b, P3D *pts);
void ajouter_pt_bezier(Bezier *b, P3D p, int pos);
int mult_courbes_get_pos_dicho(Mult_courbes *m, 
				      double d, int init);
void mult_courbes_ajouter_courbe(Mult_courbes *m, int ind, 
				 CourbeP *new);
void deplacer_pt_mult_bezier(Mult_courbes *m, int ind, double h);
#endif


#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "maillage_struct.h"
#include "maillage_acc.h"

#include "maill_surface_acc.h"

#include "elements/point_acc.h"
#include "elements/point_struct.h"

#include "../outils/listes/listeD/listeD_acc.h"
#include "../outils/listes/liste/liste_acc.h"
#include "../outils/tab/gtab_acc.h"

#include "../outils/newton/newton_acc.h"
#include "newton_maill/newton_maill_struct.h"
#include "newton_maill/newton_maill_acc.h"

#include "../surface/surface_acc.h"

#include "../parametre/parametre_maillage/parametre_maillage_acc.h"

#include "../outils/general_functions_acc.h"
#include "../outils/functions/functions_acc.h"

#include "../courbe/fonction/fonction_courbe_param_acc.h"
#include "../courbe/fonction/structures_fonc_courbe_acc.h"
#include "../courbe/courbe_param_acc.h"
#include "../courbe/courbe_acc.h"

#include "call_sing/call_sing_acc.h"

int ajouter_par(Maillage *m, ListeD *lcur, ListeD *v1,
		ListeD *v2, Param_maillage* param_s,
		Param_new1 *param, Segment *seg1,
		Segment *seg2, 
		int ind_aj, int nbMax,
		int sing, int ind_surf, int ind_constr);

void copier_segment(Ptmaill *p_init, Ptmaill *p_n, 
		    int ind_aj_ros, int sens);
int ajouter_parall_diagonal(Maillage *m,
			    Pt* np, ListeD *lnp,
			    Param_maillage* param_s,
			    ListeD *lprec, int or, 
			    int or_vir, 
			    int or_voistonew,
			    int or_base, int type_maill, 
			    int ind_constr);
int calc_cercle(Surf_param *s, Pt* init,
		Pt* c1, Pt* p1,
		P3D base_p1[2], 
		P3D dir_c, double h, Pt *pts, 
		Param_maillage* param_s, 
		double tol);
int calc_bezier(Surf_param *s, P3D pts[2], P3D der[2], 
		Pt *res, double h, double tol, 
		Param_maillage *p_maill, int or_vir);
void init_inter_droite_sphere(Surf_param *s, Param_maillage *param_s, P3D init, 
				     P3D p1, P3D vect, 
				     P3D c1, double R, 
				     Pt* np, P3D res);

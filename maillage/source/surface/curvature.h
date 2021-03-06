#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

#include "../parametre/parametre_maillage/parametre_maillage_acc.h"
#include "../maillage/elements/quadr_acc.h"
#include "../parametre/global_def.h"

#include "surface_acc.h"
#include "curvature_struct.h"
#include "../outils/listes/listeD/listeD_acc.h"
#include "../outils/listes/liste/liste_acc.h"
#include "../outils/tab/gtab_acc.h"
#include "../outils/general_functions_acc.h"

#include "../outils/init_main/init_main_acc.h"

#include "../maillage/maillage_acc.h"
#include "../maillage/maill_surface_acc.h"
#include "../maillage/newton_maill/newton_maill_acc.h"
#include "../courbe/courbe_acc.h"

Quadr_mesh* create_quadr_mesh(int pts[4], GTab *tab, 
			      double curv, int bool_util);
int curv_aj_pt(Surf_param *s, P3D param, GTab *pts);
El_constr_curv* create_el_constr_curv(ListeD *l, int dir, 
				      int aj_int, int est_connexe);
void calc_curv_sub(Mesh_curv *mesh, PListeD init[2][10], 
		   int *nb_init, double *res, 
		   Param_maillage *param_s);
int calc_inter_fin_courbe(Surf_param *s, ListeD *mesh_cur,
			  Param_new2 *param_new, 
			  SListe *pts, Param_maillage *param_s);

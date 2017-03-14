#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "surface_acc.h"
#include "surface_struct.h"

#include "../outils/functions/functions_acc.h"
#include "../outils/newton/newton_acc.h"
#include "../outils/general_functions_acc.h"
#include "../parametre/global_def.h"

#include "../courbe/courbe_struct.h"
#include "../courbe/courbe_acc.h"
#include "../courbe/fonction/fonction_courbe_param_acc.h"
#include "../courbe/courbe_param_acc.h"

Surf_param*
create_surf_param(Champ_vect* f, double p1_init, 
		  double p1_fin, double p2_init, 
		  double p2_fin, int ind_quotient);
Surf_contr*
create_surf_contr(Champ_r* f_contr);
int surf_deplacer_newton(P3D pt, P3D del, void *param, 
			       int sgn);
void free_surface_param(Surf_param *s);
int surf_set_pos_sub(Surf_param *s, P3D metr[2], 
		      P3D param_pos, P3D res);
double fonc_gauss(P3D p, void *param);


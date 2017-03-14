#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../../outils/general_functions.h"
#include "../../outils/functions/functions_acc.h"

#include "../courbe_param_acc.h"
#include "structures_fonc_courbe_acc.h"
#include "structures_fonc_courbe_struct.h"
#include "../../parametre/global_def.h"
#include "../../surface/surface_acc.h"

void fonction_bezier(double d, void *param, P3D res);
void fonction_bezier_p(double d, void *param, P3D res);

void fonction_ligne_surf_param(double d, void *param, P3D res);
void fonction_ligne_surf_param_p(double d, void *param, P3D res);

void fonction_carre(double d, void *param, P3D res);
void fonction_carre_p(double d, void *param, P3D res);

void fonction_triangle(double d, void *param, P3D res);
void fonction_triangle_p(double d, void *param, P3D res);

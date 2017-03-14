#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <stdlib.h>
#include <string.h>

#include "../../parametre/erreur/erreur_acc.h"

#include "../../surface/fonc_surf/fonc_surf_contr_acc.h"
#include "../../surface/fonc_surf/fonc_surf_param_acc.h"
#include "../../surface/surface_struct.h"
#include "../../surface/surface_acc.h"

#include "../../surface/mapping_acc.h"


#include "../../maillage/maillage_acc.h"
#include "../../maillage/maillage_struct.h"
#include "../../maillage/maill_surface_acc.h"
#include "../../maillage/dual_acc.h"
#include "../../maillage/elements/point_acc.h"

#include "../../maillage/elements/quadr_acc.h"
#include "../../maillage/elements/quadr_struct.h"

#include "../listes/listeD/callback_listeD_acc.h"
#include "../listes/listeD/listeD_acc.h"
#include "../tab/gtab_acc.h"

#include "init_main_acc.h"

#include "../listes/liste/liste_acc.h"
#include "../general_functions_acc.h"

#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"
#include "../../test/test_vecteurs_acc.h"

#include "../../courbe/fonction/fonction_courbe_param_acc.h"
#include "../../courbe/courbe_acc.h"
#include "../../courbe/courbe_param_acc.h"

/*__inline */ double discr_asin(double val);
/*__inline */ double discr_x2(double val);
/*__inline */ double discr_x(double val);

/*__inline */  double r_ext_tore(double t, void *param);
/*__inline */  double r_ext_tore_p(double t, void *param);

void aff_erreur(int nbprl, int nbptr);
void discr_angle(int nb, double(*fonc)(double), double *res);

void creer_triple_ros(Maillage *m, int or);


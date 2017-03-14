#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

#include "maillage_acc.h"
#include "maillage_struct.h"

#include "elements/point_struct.h"
#include "elements/point_acc.h"
#include "elements/quadr_acc.h"

#include "../outils/general_functions_acc.h"
#include "../outils/tab/gtab_acc.h"
#include "../outils/tab/gtab_struct.h"

#include "../outils/listes/listeD/callback_listeD_acc.h"
#include "../outils/listes/listeD/listeD_acc.h"
#include "../outils/listes/liste/liste_acc.h"
#include "../outils/listes/listeD/listeD_struct.h"
#include "../parametre/parametre_maillage/parametre_maillage_acc.h"

#include "dual_acc.h"
#include "dual_struct.h"

int est_bord_pt(ListeD *pt, void *param);
int maillage_suppr_quadr_seg(Maillage *m, Ptmaill *pt, 
			     int or, ListeD *q);
void callback_aj_vois(ListeD *l1, ListeD *l2, 
		      int dir1, int dir2, void *param);

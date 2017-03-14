#include <stdlib.h>
#include <string.h>

#include "maillage_acc.h"
#include "../outils/listes/listeD/listeD_acc.h"
#include "../outils/listes/liste/liste_acc.h"
#include "../outils/tab/gtab_acc.h"
#include "elements/quadr_acc.h"
#include "dual_struct.h"
#include "elements/point_acc.h"

int dual_aj_tab(PListeD pts[4], int nb_pts, 
		GTab *gtab_bool, GTab *quadr, int *tot_pts);
Saut_vois* dual_copier_saut(Saut_vois *s);
void dual_init_ind_vois_sing(SListeD *dual, ListeD *sing, 
			     ListeD *vois, int dir_vois, 
			     GTab *gtab_bool);
int dual_autour_sing(PListeD *pts, GTab *gtab_bool, 
		     GTab *quadr, int dir_suiv, 
		     int *tot_pts, int *bool_aj_sing);
int quadr_contient_pts(PListeD *q, PListeD *pts);

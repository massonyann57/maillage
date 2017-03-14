#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "listeD_struct.h"
#include "listeD_acc.h"
#include "callback_listeD_struct.h"
#include "callback_listeD_acc.h"

#include "../../tab/gtab_acc.h"

#include "../../general_functions_acc.h"

#include "../liste/liste_acc.h"
#include "../../../parametre/global_def.h"

#include "callback_listeD_acc.h"
#include "../../../parametre/parametre_maillage/parametre_maillage_acc.h"

Elliste* createElliste(void *el, int ind_part, 
		       int pos_tab, int est_etoile, 
		       int primal_pos_sing);
ListeD* createListeD(Elliste *el, int est_sing, int ind_sing);

Liste* aj_liste_bord(SListe *l, Liste *bord, int i, int bool);

void get_aj_np(int ind_aj, int *res);

int listeD_aj_ros(SListeD *sl, int ind, ListeD *ros);

void retirer_bord(SListeD *sl, ListeD *el);

int get_voisin_equerre(SListeD *sl, ListeD *l, int or, 
		       int ind_prec_cour, int ind_cour_prec, 
		       ListeD **res);
void DBG_print_liste_bord(SListeD *sl, int ind_part);
Maille_sing* create_maille_sing();

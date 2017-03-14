#ifndef H_LISTED_ACC

#define H_LISTED_ACC

#include "listeD_struct.h"
#include "../../tab/tab_struct.h"
#include "../../../parametre/parametre_maillage/parametre_maillage_acc.h"

SListeD* createSListeD(void *el, int (*cmp)(void*,void*,void*),
		       void (*exp)(void*),  
		       int (*est_bord)(ListeD*,void*),
		       void *param_b, 
		       void (*aj_vois)(ListeD*,ListeD*,int,int,void*),
		       void* param_aj_vois,
		       int est_sing, int est_etoile, 
		       int primal_pos_sing, int use_bord);
ListeD* ajouter_listeD(SListeD *sl, ListeD *vois, void *el, 
		       ListeD *nl, int ind_aj, Saut_vois *saut,
		       int ind_aj_new, int est_sing, int est_etoile, 
		       int primal_pos_sing, Param_maillage *param);
void suppr_el_listeD(SListeD *sl, ListeD *l, Param_maillage *param);
ListeD* liste_getVois(ListeD *l, int i);
void* get_elementL2D(ListeD *l);
void set_elementL2D(ListeD *l, void *el);
void actualiser_bord(SListeD *sl, ListeD *cur, 
		     int ind_aj, int ind_part);
void ajouter_vois(SListeD *sl, ListeD *vois, ListeD *_new,
		 int ind_aj, int ind_aj2, Param_maillage *param);
void set_vois(ListeD *l, int ind_aj, ListeD *l2, int bool_sing);

void parcoursListeD(SListeD *l, Param_maillage *param, 
		    void (*callback)(SListeD*,Param_maillage*,
				     int,int,ListeD*,void*),
		    void *param_call, int bool, int ind_part);

void merger_composante_isole(SListeD *sl, int ind_princ, 
			     int ind_iso);
void ajouter_bord(SListeD *sl, ListeD *el, Liste *el_rech);
void suppr_autres_composantes
(SListeD *sl, Param_maillage *param, 
 int ind, void (*callback_suppr)(void*,void*), void *param_call);

void free_elliste(Elliste *el);
void free_mailleD(ListeD *l, int bool_suppr_elliste);
void free_listeD(SListeD *sl);
void free_slisteD(SListeD *sl);

void print_el_listeD(void *maille, void *fonc);
void listeD_changer_partition(SListeD *sl, ListeD *el, 
			      int new_part);
void listeD_set_sing(SListeD *sl, ListeD *l);
int listeD_fermer_sing(ListeD *l, int dirl, int dirs, int del);
int listeD_get_saut(ListeD *l, int dir);
int listeD_get_nb_max_vois(ListeD *l);

ListeD* get_element_SListeD(SListeD *sl, int ind_part, 
			    int ind_el);
Saut_vois* create_saut_vois(int dir, int del, int dessus);
int get_saut_sing(ListeD *v_sing1, ListeD *v_sing2);
int listeD_saut_ligne_sing(ListeD *l, int dirl);
void listeD_suppr_sing(SListeD *sl, ListeD *l, int *dirs);
void listeD_suppr_sing_tab(SListeD *sl, ListeD *l);

#endif


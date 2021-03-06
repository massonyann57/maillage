#ifndef H_LISTE_ACC

#define H_LISTE_ACC

#include "../../../parametre/global_def.h"
#include "liste_struct.h"

SListe* createSListe(void *el, int (*cmp)(void*,void*,void*),
		     void *param_cmp,
		     void (*exp)(void*,void*), void *param_exp);

Liste* ajouter_liste_trie(SListe *l, void *el, Liste *prec);
void ajouter_fin_liste(SListe *l, void *el);
void ajouter_debut_liste(SListe *l, void *el);

void rech_suppr_el_liste(SListe *l, void *el);
void suppr_el_liste(SListe *sl, Liste *l);
void free_liste(SListe *l);
void merger_listes(SListe *l1, SListe *l2, 
		   int (*call_aj)(void*,void*),
		   void* param);
SListe* copier_trier_liste(SListe *l,
			   int (*call_aj)(void*,void*),
			   void* param);
int liste2tab(SListe *l, Pvoid *res);
void* get_elementL(Liste *l);

int size_liste(SListe *l);
void DBG_print_liste(SListe *l);

void ajouter_apres_liste(SListe *l, Liste *m, void *el);
#endif


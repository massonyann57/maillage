#ifndef CALLBACK_LISTED_ACC

#define CALLBACK_LISTED_ACC

#include "callback_listeD_struct.h"

int estBordL2D(SListeD *sl, ListeD *l);
int estBordL(void *_sl, void *param);
void callback_aj_tab2D(SListeD *sl, Param_maillage *param, int i, int j, 
		       ListeD *l, void *_param);

/* fonctions paramètres */
/* fonctions cmp */
int cmp_L2D_i(void *el1, void *el2, void *_bool);
int cmp_L2D_max(void *el1, void *el2, void *_bool);
int cmp_L2D_ind(void *el1, void *el2, void *dummy);
int cmp_L2D_def(void *el1, void *el2, void *dummy);

/* fonctions is_used */
int listeD_estbordpI(void *el, void* param);
int callback_suppr_ind_max(void *el, void *indm, Param_maillage* param);
void callback_suppr_unused(SListeD *sl, Param_maillage *param, int i, int j, 
			   ListeD *l, void *param_call);

/*fonctions exp */
double exp_L2D_ell(void *el);
double exp_L2D(void *el);
double exp_L2D_ell_ind(void *el);
double exp_L2D_ind(void *el);

Param_isUsed* 
create_param_isUsed(int (*is_used)(void*,void*), 
		    void* param_is_used, 
		    void (*callback_suppr)(void*,void*), 
		    void* param_suppr);

void exp_pos_tab(void *el);
int cmp_pos_tab(void *el1, void *el2, void *param);


#endif

#ifndef H_GTAB_ACC

#define H_GTAB_ACC

#include "gtab_struct.h"
#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"

GTab* create_gtab(void (*print_el)(void*,void*), void *param);
int aj_gtab(GTab *tab, void* el);
void* get_element_gtab(GTab *tab, int i);
void set_element_gtab(GTab *tab, int i, void *el);
void free_gtab(GTab *tab, void (*free_el)(void*));
void exp_gtab(GTab *tab);

void suppr_el_gtab(GTab *tab, int pos, Param_maillage *param);
GTab* init_bool_gtab(GTab *tab);


#endif

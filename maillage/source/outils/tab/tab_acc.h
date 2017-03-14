#ifndef H_TAB_ACC

#define H_TAB_ACC

#include "tab_struct.h"
#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"

Tab2D* createTab2D(int (*is_used)(void*,void*),
		   double (*exp)(void*), void *param, int nbmax);
void aj_tab2D(Tab2D *t, Param_maillage *param, int i, int j, void *el, int bool_aj);
int tab_get_nb(Tab2D *t);
void tab_export(Tab2D *t);
void init_tab2D(Tab2D *t, int (*is_used)(void*,void*,Param_maillage*),
		double (*exp)(void*), void *param, int nbmax);



#endif

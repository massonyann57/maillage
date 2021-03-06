#include "callback_listeD.h"


/*
 * Constructeur de param_is_used (utilisé pour la 
 *  suppression de certains éléments de la liste)
 * is_used : callback disant si on doit supprimer 
 *    un élément ou non
 * param_is_used : paramètre du callback is_used
 * callback_suppr : callback permettant la 
 *    suppression d'un élément
 * param_suppr : paramètre du callback suppr
 */
Param_isUsed* 
create_param_isUsed(int (*is_used)(void*,void*), 
		    void* param_is_used, 
		    void (*callback_suppr)(void*,void*), 
		    void* param_suppr){
  Param_isUsed *res = malloc(sizeof(Param_isUsed));
  res->is_used = is_used;
  res->param_is_used = param_is_used;
  res->param_suppr = param_suppr;
  return res;
}

/*
 * Fonction permettant de dire si un élément est au bord ou non
 */
int estBordL2D(SListeD *sl, ListeD *l){
  if(sl->use_bord){
    if(sl->est_bord == NULL){
      if(l->vois != NULL)
	return 1;
      return l->nH == NULL || l->pH == NULL ||
	l->pL == NULL || l->nL == NULL;
    }
    return sl->est_bord(l, sl->param_bord);
  } else
    return 0;
}

int estBordL(void *_sl, void *param){
  ListeD *sl = (ListeD*) _sl;
  SListeD *sl2D = (SListeD*) param;
  return sl2D->est_bord(sl, sl2D->param_bord);
}

/*
 * On compare 2 éléments de la liste grace au callback de celle-ci
 */
int cmp_L2D_def(void *el1, void *el2, void *fonc){
  int (*f)(void*,void*,void*) = fonc;
  return f(((Elliste*)el1)->el, ((Elliste*)el2)->el,0);
}

int cmp_pos_tab(void *el1, void *el2, void *param){
  Elliste *_el1 = ((ListeD*)el1)->el;
  Elliste *_el2 = ((ListeD*)el2)->el;
  if(_el1->pos_tab < _el2->pos_tab)
    return -1;
  else if(_el1->pos_tab > _el2->pos_tab)
    return 1;
  else
    return 0;
}

void exp_pos_tab(void *el){
  Elliste *_el = ((ListeD*)el)->el;
  printf("%d\n", _el->pos_tab);
}

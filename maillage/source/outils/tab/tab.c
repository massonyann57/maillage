#include "tab.h"

/*
 * Constructeur de Tab2D
 * is_used : callback disant si un élément est utilisé ou non
 * param : paramètre de ce callback
 * exp : callback permettant d'exporter un élément
 */
Tab2D* createTab2D(int (*is_used)(void*,void*,Param_maillage*),
		   double (*exp)(void*), void *param, int nbmax){
  Tab2D* res = malloc(sizeof(Tab2D));
  int i;
  res->nbi = 0;
  memset(res->init_j, -1, nbmax*sizeof(int));
  memset(res->fin_j, 0, nbmax*sizeof(int));
  res->is_used = is_used;
  res->exp = exp;
  res->param = param;
  for(i = 0;i<nbmax;i++)
    memset(res->el[i], 0, nbmax*sizeof(void*));
  return res;
}

void init_tab2D(Tab2D *res, int (*is_used)(void*,void*,Param_maillage*),
		   double (*exp)(void*), void *param, int nbmax){
  int i;
  res->nbi = 0;
  memset(res->init_j, -1, nbmax*sizeof(int));
  memset(res->fin_j, 0, nbmax*sizeof(int));
  res->is_used = is_used;
  res->exp = exp;
  res->param = param;
  for(i = 0;i<nbmax;i++)
    memset(res->el[i], 0, nbmax*sizeof(void*));
}

/* 
 * Ajoute un élément dans le tableau
 * i : ligne de l'ajout
 * j : colonne de l'ajout
 * el : l'élément
 * bool_aj : sera exporté ou non
 */
void aj_tab2D(Tab2D *t, Param_maillage *param, int i, int j, void *el, int bool_aj){
  if(t->el[i][j] != NULL && t->el[i][j] != el){
    printf("element double dans le tableau\n");
    return;
  }
  if(i>=t->nbi){
    t->nbi = i+1;
    t->init_j[i] = j;
    t->fin_j[i] = j+1;
  }
  t->el[i][j] = el;
  t->ind[i][j] = bool_aj-1;
  t->bool[i][j] = t->is_used == NULL
	|| t->is_used(el, t->param, param);
  if(j>=t->fin_j[i])
    t->fin_j[i] = j+1;
  if(t->init_j[i] == -1 || j<t->init_j[i])
    t->init_j[i] = j;
}

/*
 * retourne le nombre d'élément dans le tableau
 */
int tab_get_nb(Tab2D *t){
  int i, j, res = 0;
  for(i = 0;i<t->nbi;i++){
    for(j = t->init_j[i];j<t->fin_j[i];j++){
      if(t->bool[i][j] && t->el[i][j] != NULL && t->ind[i][j] != -1)
	res++;
    }
  }
  return res;
}

/*
 * Affiche le tableau
 */
void tab_export(Tab2D *t){
  int i, j, s = t->fin_j[0];
  for(i = 0;i<t->nbi;i++)
	s = _max(s, t->fin_j[i]);
  for(j = s-1;j>=0;j--){
	for(i = 0;i<t->nbi;i++){
	  if(j>t->init_j[i] && j < t->fin_j[i]
		 && t->bool[i][j])
		printf("%.0lf ", t->exp(t->el[i][j]));
	  else
		printf("0 ");
	}
	printf("\n");
  }
}
  

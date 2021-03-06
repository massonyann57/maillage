#include "gtab.h"

GTab gtabs[NB_GTAB];
int nb_gtab_util = 0;
PGTab pgtabs[NB_GTAB];

void init_gtab(){
  int i;
  for(i = 0;i<NB_GTAB;i++){
    pgtabs[i] = gtabs+i;
  }
}

GTab* create_gtab(void (*print_el)(void*,void*), void *param){
  GTab *res;
  if(nb_gtab_util==0)
    init_gtab();
  if(nb_gtab_util>=NB_GTAB){
    res = malloc(sizeof(GTab));
    res->pos_gtab = -1;
  } else {
    res = pgtabs[nb_gtab_util];
    res->pos_gtab = nb_gtab_util;
    nb_gtab_util++;
  }
  res->el[0] = malloc(SIZE_TAB*sizeof(Pvoid));
  res->bool_suppr[0] = malloc(SIZE_TAB*sizeof(int));
  res->nbi = 1;
  res->nbj = 0;
  res->nb_tot = 0;
  res->print_el = print_el;
  res->param = param;
  res->nb_suppr = 0;
  return res;
}

int aj_gtab(GTab *tab, void* el){
  int nbs = tab->nb_suppr;
  int nbi = tab->nbi;
  int nbj = tab->nbj;
  if(nbs == 0){
    nbi = tab->nbi;
    nbj = tab->nbj;
    tab->nbj++;
    tab->nb_tot++;
    if(tab->nbj == SIZE_TAB){
      tab->el[tab->nbi] = malloc(SIZE_TAB*sizeof(Pvoid));
      tab->bool_suppr[tab->nbi] = malloc(SIZE_TAB*sizeof(int));
      tab->nbi++;
      tab->nbj = 0;
    }
  } else {
    nbi = tab->pos_suppr[nbs-1][0]+1;
    nbj = tab->pos_suppr[nbs-1][1];
    tab->nb_suppr--;
  }
  tab->el[nbi-1][nbj] = el;
  tab->bool_suppr[nbi-1][nbj] = 0;
  return (nbi-1)*SIZE_TAB+nbj;
}

void* get_element_gtab(GTab *tab, int i){
  int l = i/SIZE_TAB;
  int c = i%SIZE_TAB;
  if(tab->bool_suppr[l][c] == 1)
    return NULL;
  return tab->el[l][c];
}

void free_gtab(GTab *tab, void (*free_el)(void*)){
  int i, nbi = tab->nbi;
  int j, jmax;
  for(i = 0;i<nbi;i++){
    if(free_el != NULL){
      if(i == nbi-1)
	jmax = tab->nbj;
      else
	jmax = SIZE_TAB;
      for(j = 0;j<jmax;j++){
	if(tab->el[i][j] != NULL)
	  free_el(tab->el[i][j]);
      }
    }
    free(tab->el[i]);
    free(tab->bool_suppr[i]);
  }
  if(tab->pos_gtab == -1)
    free(tab);
  else{
    for(i = tab->pos_gtab;i<nb_gtab_util-1;i++){
      pgtabs[i] = pgtabs[i+1];
      pgtabs[i]->pos_gtab = i;
    }
    pgtabs[nb_gtab_util-1] = tab;
    nb_gtab_util--;
  }
}

void set_element_gtab(GTab *tab, int i, void *el){
  int l = i/SIZE_TAB;
  int c = i%SIZE_TAB;
  tab->el[l][c] = el;
  tab->bool_suppr[l][c] = 0;
}

void exp_gtab(GTab *tab){
  int i, j;
  int j_max;
  printf("taille : %d\n", tab->nb_tot);
  for(i = 0;i<tab->nbi;i++){
    if(i<tab->nbi-1)
      j_max = SIZE_TAB;
    else
      j_max = tab->nbj;
    for(j = 0;j<j_max;j++){
      if(!tab->bool_suppr[i][j])
	tab->print_el(tab->el[i][j], tab->param);
    }
  }
}

void suppr_el_gtab(GTab *tab, int pos, Param_maillage *param){
  int nbs = tab->nb_suppr;
  int l = pos/SIZE_TAB;
  int c = pos%SIZE_TAB;
  tab->el[l][c] = NULL;
  tab->bool_suppr[l][c] = 1;
  if(nbs >= SIZE_SUPPR_GTAB){
    PRINT_WAR_LOG(param, "ATTENTION ON DEPASSE LA TAILLE DU TABLEAU\n");
    return;
  }
  tab->pos_suppr[nbs][0] = l;
  tab->pos_suppr[nbs][1] = c;
  tab->nb_suppr++;
}

GTab* init_bool_gtab(GTab *tab){
  GTab *res = create_gtab(NULL, NULL);
  int i, j, nbj;
  for(i = 0;i<tab->nbi;i++){
    nbj = SIZE_GTAB;
    if(i == tab->nbi-1)
      nbj = tab->nbj;
    for(j = 0;j<nbj;j++){
      aj_gtab(res, NULL);
    }
  }
  return res;
}

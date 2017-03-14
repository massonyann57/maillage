#include "quadr.h"

/*
 * Constructeur de Quadr
 */
Quadr* createQuadr(PListeD pts[4], int *ind_seg, int ind_surf, 
		   int ind){
  Quadr *res = malloc(sizeof(Quadr));
  int i;
  for(i = 0;i<4;i++){
    res->pts[i] = pts[i];
    if(ind_seg != NULL)
      res->ind_seg[i] = ind_seg[i];
  }
  res->ind_surf = ind_surf;
  res->ind = ind;
  res->est_diag = 0;
  return res;
}

/*
 * Utilisé en callback : compare 2 Quadr
 */
int cmp_quadr(void *q1, void *q2,void* dummy){
  Quadr *_q1 = (Quadr*)q1;
  Quadr *_q2 = (Quadr*)q2;
  if(q1 == q2)
	return 0;
  if(_q1->ind > _q2->ind)
	return 1;
  else return -1;
}

void freeQuadr(Quadr *q){
  free(q);
}

int appartientQuadr(Quadr *q, ListeD *pt){
  int i;
  for(i = 0;i<4;i++){
    if(q->pts[i] == pt)
      return 1;
  }
  return 0;
}

void print_el_quadr(void *quadr){
  Quadr *el = quadr;
  int i;
  for(i = 0;i<4;i++){
    printf("%d ", el->pts[i]->el->pos_tab);
  }
  printf("\n");
}


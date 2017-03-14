#ifndef H_QUADR_ACC

#define H_QUADR_ACC

#include "quadr_struct.h"
#include "../../outils/listes/listeD/listeD_struct.h"

Quadr* createQuadr(PListeD pts[4], int *ind_seg, 
		   int ind_surf, int ind);
int cmp_quadr(void *q1, void *q2, void* dummy);
int appartientQuadr(Quadr *q, ListeD *pt);
void print_el_quadr(void *quadr);
void freeQuadr(Quadr *q);

#endif

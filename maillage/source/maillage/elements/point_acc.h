#ifndef POINT_ACC_H

#define POINT_ACC_H

#include "point_struct.h"
#include "quadr_struct.h"

Point* createPoint(Pt* p, int ind_constr);
//Jonction* createJonction(ListeD *suiv, int ind_seg, int or);
Ptmaill* createPtmaill(Pt* p, int ind_constr);
Ptmaill* createPtmaillp(Point *p);
Segment* createSegment(Point *p1, Point *p2);

int cmp_pts(void *p1, void *p2, void *dummy);
int is_used_pt(void *p, void* param_call, Param_maillage *param);
void free_ptmaill(Ptmaill *pt);
void free_segment(Segment *seg);
int suppr_quadr_seg(Segment *seg, ListeD *q);
void seg_ajouter_quadr(Segment *s, ListeD *q);
void ajouter_jonction(Ptmaill *pt, ListeD *suiv, 
		      int ind_seg, int or);
void print_ptMaill(void *pt);

Jonc_sing* create_jonc_sing();
void ptmaill_ajouter_sing(Maillage *m, Ptmaill *ptm);
int ptmaill_get_ind_seg(Ptmaill* ptm, int i);
void ptmaill_set_ind_seg(Ptmaill *ptm, int pos, int ind);
void ptmaill_fermer_sing(Ptmaill *ptm);
void ptmaill_suppr_sing(Maillage *m, Ptmaill *ptm, int *dir);

#endif

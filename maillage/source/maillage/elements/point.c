#include "point.h"

/*
 * Constructeur point
 */
Point* createPoint(Pt *p, int ind_constr){
  Point *res = malloc(sizeof(Point));
  res->pt = p;
  res->ind_constr = ind_constr;
  res->the = 0.;
  res->est_bord_sector = 0;
  return res;
}

Jonc_sing* create_jonc_sing(){
  Jonc_sing *res = malloc(sizeof(Jonc_sing));
  int i;
  res->nb_seg = 0;
  for(i = 0;i<NB_BRANCHES_MAX;i++)
    res->ind_seg[i] = -1;
  res->est_ferme = 0;
  return res;
}

/*
 * Constructeur jonction
 */
/*Jonction* createJonction(ListeD *suiv, int ind_seg, int or){
  Jonction *res = malloc(sizeof(Jonction));
  res->el = suiv;
  res->or = or;
  res->ind_seg = ind_seg;
  return res;
  }*/

/*
 * Constructeur segment
 */
Segment* createSegment(Point *p1, Point *p2){
  Segment *res = malloc(sizeof(Segment));
  res->p1 = p1;
  res->p2 = p2;
  res->q1 = NULL;
  res->q2 = NULL;
  res->b_new = 1;
  return res;
}

/*
 * Constructeur Ptmaill
 */
Ptmaill* createPtmaill(Pt *p, int ind_constr){
  int i;
  Ptmaill *res = malloc(sizeof(Ptmaill));
  res->p = createPoint(p, ind_constr);
  for(i = 0;i<4;i++)
    res->ind_seg[i] = -1;
  // res->jonc = NULL;
  res->sing = NULL;
  return res;
}

/*
 * 2eme constructeur Ptmaill
 */
Ptmaill* createPtmaillp(Point *p){
  int i;
  Ptmaill *res = malloc(sizeof(Ptmaill));
  res->p = p;
  for(i = 0;i<4;i++)
    res->ind_seg[i] = -1;
  res->sing = NULL;
  return res;
}


/*
 * Utilisé en callback pour l'exportation : 
 * regarde si un point est utilisé ou non
 */
int is_used_pt(void *p, void *param_call, Param_maillage *param){
  Ptmaill *pt = (Ptmaill*)(((ListeD*)p)->el->el);
  Maillage *m = (Maillage*) param_call;
  Segment *s;
  int i, ind_seg;
  for(i = 0;i<4;i++){
    /* TODO : get_ind_seg */
    ind_seg = pt->ind_seg[i];
    if(ind_seg != -1){
      s = get_element_gtab(m->seg, ind_seg);
      if(s->q1 != NULL)
	return 1;
    }
  }
  return 0;
}

/*
 * Destructeur Ptmaill : 
 * /!\ Ne pas utiliser sur une rosace (elle a été dupliqué)
 */
void free_ptmaill(Ptmaill *pt){
  free(pt->p->pt);
  free(pt->p);
  free(pt);
}

void free_segment(Segment *seg){
  free(seg);
}
/*
 * Supprime le segment si le ||gramme q le contient
 * seg : le segment
 * q : le ||gramme
 * retourne 1 si on a trouvé le ||gramme
 */
int suppr_quadr_seg(Segment *seg, ListeD *q){
  if(seg->q1 == q){
    seg->q1 = seg->q2;
    seg->q2 = NULL;
    return 1;
  } 
  if(seg->q2 == q) {
    seg->q2 = NULL;
    return 1;
  }
  return 0;
}

/*
 * Ajoute un quadrangle à s
 */
void seg_ajouter_quadr(Segment *s, ListeD *q){
  if(s->q1 != NULL)
    s->q2 = q;
  else 
    s->q1 = q;
}

/*void ajouter_jonction(Ptmaill *pt, ListeD *suiv, 
		      int ind_seg, int or){
  pt->jonc = createJonction(suiv, ind_seg, or);
  }*/

void print_ptMaill(void *pt){
  Ptmaill *ptm = pt;
  double *tmp = ptm->p->pt->p;
  printf("%lf %lf %lf\n", tmp[0], tmp[1], tmp[2]);
}

void ptmaill_ajouter_sing(Maillage *m, Ptmaill *ptm){
  int i, ind, nb = 0, _i;
  int i_init = 4;
  ListeD *q;
  Segment *seg;

  ptm->sing = create_jonc_sing();
  while(i_init>0 && ptm->ind_seg[i_init] != -1)
    i_init--;
  if(i_init==4)
    i_init = 0;
  for(i = i_init;i<i_init+4;i++){
    _i = i%4;
    ind = ptm->ind_seg[_i];
    if(ind != -1){
      ptm->sing->ind_seg[nb] = ind;
      seg = get_element_gtab(m->seg, ind);
      q = seg->q1;
      if(q != NULL)
	q->el->est_etoile = 1;
      q = seg->q2;
      if(q != NULL)
	q->el->est_etoile = 1;

      nb++;
    }
  }
  ptm->sing->nb_seg = nb;
}
void ptmaill_suppr_sing(Maillage *m, Ptmaill *ptm, int *dir){
  int i, ind, nb = 0;
  ListeD *q;
  Segment *seg;
  if(ptm->sing != NULL){
    nb = ptm->sing->nb_seg;
    memset(ptm->ind_seg, -1, 4*sizeof(int));
    for(i = 0;i<nb;i++){
      if(dir[i] != -1){
	ind = ptm->sing->ind_seg[i];
	ptm->ind_seg[dir[i]] = ind;
	if(ind != -1){
	  seg = get_element_gtab(m->seg, ind);
	  q = seg->q1;
	  if(q != NULL)
	    q->el->est_etoile = 0;
	  q = seg->q2;
	  if(q != NULL)
	    q->el->est_etoile = 0;
	}
      }
    }
    free(ptm->sing);
    ptm->sing = NULL;
  }
}

int ptmaill_get_ind_seg(Ptmaill* ptm, int i){
  if(ptm->sing == NULL){
    i = i%4;
    return ptm->ind_seg[i];
  }
  else{
    if(ptm->sing->est_ferme)
      i = i%ptm->sing->nb_seg;
    return ptm->sing->ind_seg[i];
  }
}

void ptmaill_set_ind_seg(Ptmaill *ptm, int pos, int ind){
  if(ptm->sing == NULL)
    ptm->ind_seg[pos] = ind;
  else{
    ptm->sing->ind_seg[pos] = ind;
    if(pos >= ptm->sing->nb_seg)
      ptm->sing->nb_seg++;
  }
}

void ptmaill_fermer_sing(Ptmaill *ptm){
  if(ptm->sing != NULL)
    ptm->sing->est_ferme = 1;
}
		      

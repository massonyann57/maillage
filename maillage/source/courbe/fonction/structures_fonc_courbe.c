#include "structures_fonc_courbe.h"

Courbe_surf_param* create_droite_surf_param(Surf_param *s, 
					    int bool,
					    double l2){
  Courbe_surf_param *res = malloc(sizeof(Courbe_surf_param));
  res->s = s;
  res->bool = bool;
  res->l2 = l2;
  res->c = NULL;
  return res;
}

Courbe_surf_param* create_courbe_surf_param(Surf_param *s, 
					    CourbeP *c){
  Courbe_surf_param *res = malloc(sizeof(Courbe_surf_param));
  res->s = s;
  res->bool = 0;
  res->l2 = 0;
  res->c = c;
  return res;
}

Bezier* createBezier(P3D *pts, int n){
  Bezier *res = malloc(sizeof(Bezier));
  int i;
  if(pts != NULL){
    int _n = n-1;
    int fact_n = factorielle(_n);
    for(i = 0;i<4;i++){
      VECT_CPY(res->pts[i], pts[i]);
      res->coeff[i] = fact_n/(factorielle(_n-i)*factorielle(i));
    }
  } else {
    for(i = 0;i<n;i++)
      memset(res->pts[i], 0, 3*sizeof(double));
  }
  res->nb = n;
  return res;
}

Mult_courbes* create_mult_courbes(int nb, 
				  PCourbeP *courbes, 
				  double init, double fin){
  int i;
  double pos = init, del = (fin-init)/nb;
  Mult_courbes *res = malloc(sizeof(Mult_courbes));
  for(i = 0;i<nb;i++){
    res->c[i] = courbes[i];
    res->init[i] = pos;
    pos += del;
    res->fin[i] = pos;
  }
  res->nb = nb;
  return res;
}

NewMult_courbes* create_newMult_courbes(Mult_courbes *c, int pos){
  NewMult_courbes *res = malloc(sizeof(NewMult_courbes));
  res->courbes = c;
  res->pos = pos;
  return res;
}

/*
 * Change les points de notre courbe de Bézier
 */
void set_pts_bezier(Bezier *b, P3D *pts){
  int i;
  int _n = b->nb-1;
  int fact_n = factorielle(_n);
  for(i = 0;i<4;i++){
    VECT_CPY(b->pts[i], pts[i]);
    b->coeff[i] = fact_n/(factorielle(_n-i)*factorielle(i));
  }
}

/*
 * On ajoute un point à notre courbe de Bezier
 */
void ajouter_pt_bezier(Bezier *b, P3D p, int pos){
  int i;
  int _n = b->nb;
  int fact_n = factorielle(_n);
  b->nb++;
  for(i = _n;i>=pos;i--){
    VECT_CPY(b->pts[i], b->pts[i-1]);
    b->coeff[i] = fact_n/(factorielle(_n-i)*factorielle(i));
  }
  VECT_CPY(b->pts[pos], p);
  b->coeff[pos] = fact_n/(factorielle(_n-pos)*factorielle(pos));
  for(i = 0;i<pos;i++)
    b->coeff[i] = fact_n/(factorielle(_n-i)*factorielle(i));
}

int mult_courbes_get_pos_dicho(Mult_courbes *m, 
			       double d, int init){
  int res = init;
  int is_term = 0;
  int pos;
  int max = m->nb-1, min = 0;
  double del;
  if(res < 0)
    res = (m->nb-1)/2;
  while(! is_term){
    pos = 0;
    del = d-m->init[res];
    if(del < -EPS_TOL){
      pos = -1;
      if(res == 0)
	return 0;
    }
    else{
      del = m->fin[res] - d;
      if(del < -EPS_TOL){
	pos = 1;
	if(res == m->nb-1)
	  return m->nb-1;
      }
      else
	is_term = 1;
    }
    if(! is_term){
      if(pos == 1){
	res = (res+max+1)/2;
      } else {
	res = (res+min)/2;
      }
    }
  }
  return res;
}

/*
 * On déplace un point de la réunion des courbes de Bezier
 * dans la direction de sa normale
 * ind : l'indice du morceau de courbe que l'on déplace
 * h : la taille du déplacement
 */
void deplacer_pt_mult_bezier(Mult_courbes *m, int ind, double h){
  int i, nb;
  P3D del, orth;
  CourbeP *c1 = m->c[ind-1];
  CourbeP *c2 = m->c[ind];
  Bezier *bez1 = (Bezier*) c1->param->param;
  Bezier *bez2 = (Bezier*) c2->param->param;
  nb = bez1->nb;
  VECT_DEL(del, bez2->pts[0], bez2->pts[1], i);
  normer(del);
  orth[0] = -del[1]; orth[1] = del[0]; orth[2] = 0.0;
  homothetie(orth, h);
  for(i = 0;i<3;i++){
    bez1->pts[nb-2][i] += orth[i];
    bez1->pts[nb-1][i] += orth[i];
    bez2->pts[0][i] += orth[i];
    bez2->pts[1][i] += orth[i];
  }
}

void mult_courbes_ajouter_courbe(Mult_courbes *m, int ind, 
				 CourbeP *new){
  int i, nb_courbes = m->nb;
  double pos = m->fin[nb_courbes-1];
  double del = (pos - m->init[0])/(nb_courbes+1);
  for(i = nb_courbes;i>pos;i--){
    m->c[i] = m->c[i-1];
    m->fin[i] = pos;
    pos -= del;
    m->init[i] = pos;
  }
  m->c[ind] = new;
  pos = m->init[0];
  for(i = 0;i<=pos;i++){
    m->init[i] = pos;
    pos += del;
    m->fin[i] = pos;
  }
  m->nb++;
}

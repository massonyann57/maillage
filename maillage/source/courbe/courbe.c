#include "courbe.h"

Courbe* create_courbe(int type, CourbeP *c_param, 
		      CourbeG *c_geod, CourbePts *c_pts){
  Courbe *res = malloc(sizeof(Courbe));
  res->type = type;
  res->c_param = c_param;
  res->c_geod = c_geod;
  res->c_pts = c_pts;
  return res;
}

CourbeG* create_courbeG(Pt *init, P3D dir_init, 
			double pas, int size, 
			double *courbure){
  CourbeG *res = malloc(sizeof(CourbeG));
  res->size = size;
  res->init = init;
  VECT_CPY(res->dir_init, dir_init);
  res->pas = pas;
  if(courbure == NULL){
    memset(res->courbure, 0, size*sizeof(double));
  } else {
    int i;
    for(i = 0;i<size;i++)
      res->courbure[i] = courbure[i];
  }
  return res;
}

CourbePts* create_courbePts(P3D *param, P3D *pts, int nb){
  CourbePts *res = malloc(sizeof(CourbePts));
  int i;
  res->nb = nb;
  for(i = 0;i<nb;i++){
    VECT_CPY(res->pts[i].param, param+i);
    VECT_CPY(res->pts[i].p, pts+i);
  }
  return res;
}

void init_courbe_geod(Courbe* c, P3D dir_init, 
		      Pt *init, double pas, 
		      int nb, double *courbure){
  c->c_geod = create_courbeG(init, dir_init, 
			     pas, nb, courbure);
  if(c->type == -1){
    c->c_param = NULL;
    c->c_pts = NULL;
  }
  c->type = 1;
}

void init_courbe_param(Courbe* c, Fonc_vect *f, double p_init, 
		       double p_fin, int est_per, double in, 
		       int dir){
  P3D dummy = {0.,0.,0.};
  c->c_param = create_courbeP(f, p_init, p_fin, 
			      est_per, in, dir, dummy);
  if(c->type == -1){
    c->c_geod = NULL;
    c->c_pts = NULL;
  }
  c->type = 0;
}

void init_courbePts(Courbe* c, P3D *param, P3D *pts, int nb){
  c->c_pts = create_courbePts(param, pts, nb);
  if(c->type == -1){
    c->c_param = NULL;
    c->c_geod = NULL;
  }
  c->type = 2;
}

void set_init_courbe(Courbe *c, double in){
  if(c->type == 0)
    c->c_param->in = in;
  else if(c->type == 2)
    c->c_param->param_init[c->c_param->dir] = in;
}

void free_courbeG(CourbeG* c){
  free(c);
}

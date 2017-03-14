#include "courbe_param.h"

CourbeP* create_courbeP(Fonc_vect *f, double p_init, 
			double p_fin, int est_periodique,
			double in, int dir, P3D param_init){
  CourbeP *res = malloc(sizeof(CourbeP));
  res->param = f;
  res->p_init = p_init;
  res->p_fin = p_fin;
  res->in = in;
  res->dir = dir;
  res->est_periodique = est_periodique;
  res->param_init[0] = param_init[0];
  res->param_init[1] = param_init[1];
  res->param_init[2] = 0.0;
  
  return res;
}

double courbe_get_dir(CourbeP *c, double d, P3D res){
  double no;
  image_fvect_p(c->param, d, res);
  no = norme(res);
  normer(res);
  return no;
}

int courbe_position_borne(CourbeP *c, double d){
  if(d < c->p_init)
    return -1;
  if(d > c->p_fin)
    return 1;
  return 0;
}

/*
 * Additionne le paramètre delta au paramètre d1 
 */
int courbe_deplacer(CourbeP *c, double *d1, double delta){
  double tmp = d1[0] + delta;
  if(tmp < c->p_init){
    if(c->est_periodique)
      d1[0] = c->p_fin + tmp - c->p_init;
    else return 0;
  } else if(tmp > c->p_fin){
    if(c->est_periodique)
      d1[0] = c->p_init + tmp - c->p_fin;
    else return 0;
  } else
    d1[0] += delta;
  return 1;
}

void free_courbeP(CourbeP *c){
  free(c);
}

void courbeP_get_image(CourbeP *c, double d, P3D res){
  if(c->param != NULL)
    image_fvect(c->param, d, res);
  else{
    int dir = c->dir%2;
    int _dir = 1-dir;
    res[dir] = d;
    res[_dir] = c->param_init[_dir];
  }
}

void courbeP_get_image_p(CourbeP *c, double d, P3D res){
  if (c->param == NULL){
    int bool = c->dir % 2;
    res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;
    res[bool] = 1.0;
  }
  else {
    image_fvect_p(c->param, d, res);
  }
}


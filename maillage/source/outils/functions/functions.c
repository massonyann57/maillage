#include "functions.h"

Champ_vect* create_champ_vect(void (*f)(P3D, void*, P3D),
				    void (*f_p)(P3D, void*, P3D*), 
				    void (*f_p2)(P3D, void*, P3D*), 
				    void (*f_p3)(P3D, void*, P3D*), 
				    void *param){
  Champ_vect *res = malloc(sizeof(Champ_vect));
  res->f = f;
  res->f_p = f_p;
  res->f_p2 = f_p2;
  res->f_p3 = f_p3;
  res->param = param;
  
#ifdef _DBG_FONC
  res->init[0] = init[0];res->init[1] = init[1];
  res->fin[0] = fin[0];res->fin[1] = fin[1];
#endif
  return res;
}

Fonc_vect* create_fonction_vect(void (*f)(double, void*, P3D),
				void (*f_p)(double, void*, P3D), 
				void *param){
  Fonc_vect *res = malloc(sizeof(Fonc_vect));
  res->f = f;
  res->f_p = f_p;
  res->param = param;
  return res;
}

Champ_r* create_champ_r(double (*f)(P3D, void*),
			      void (*f_p)(P3D, void*, P3D),
			      void *param){
  Champ_r *res = malloc(sizeof(Champ_r));
  res->f = f;
  res->f_p = f_p;
  res->param = param;
  return res;
}
Fonc_r* create_fonc_r(double (*f)(double, void*),
		      double (*f_p)(double, void*),
		      void *param){
  Fonc_r *res = malloc(sizeof(Fonc_r));
  res->f = f;
  res->f_p = f_p;
  res->param = param;
  return res;
}

 double image_cr(Champ_r* f, P3D pt){
  return f->f(pt, f->param);
}

 void image_cr_p(Champ_r* f, P3D pt, P3D res){
  f->f_p(pt, f->param, res);
}

 void image_cvect(Champ_vect* f, P3D pt, P3D res){
  f->f(pt, f->param, res);
}

 void image_cvect_p(Champ_vect* f, P3D pt, P3D *res){
  f->f_p(pt, f->param, res);
}
 void image_cvect_p2(Champ_vect* f, P3D pt, P3D *res){
  f->f_p2(pt, f->param, res);
}
 void image_cvect_p3(Champ_vect* f, P3D pt, P3D *res){
  f->f_p3(pt, f->param, res);
}

 void image_fvect(Fonc_vect* f, double d, P3D res){
  f->f(d, f->param, res);
}

 void image_fvect_p(Fonc_vect* f, double d, P3D res){
  f->f_p(d, f->param, res);
}

 double image_fr(Fonc_r* f, double d){
  return f->f(d, f->param);
}

 double image_fr_p(Fonc_r* f, double d){
  return f->f_p(d, f->param);
}

void free_champ_vect(Champ_vect *c){
  free(c->param);
  free(c);
}

void free_fonc_vect(Fonc_vect *f){
  free(f->param);
  free(f);
}

void free_champ_r(Champ_r *c){
  free(c->param);
  free(c);
}

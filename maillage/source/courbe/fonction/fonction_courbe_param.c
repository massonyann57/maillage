#include "fonction_courbe_param.h"

void fonction_ellipse(double d, void *param, P3D res){
  double *R = (double*)param;
  res[0] = R[2] + R[0]*cos(d);
  res[1] = R[3] + R[1]*sin(d);
  res[2] = 0.0;
}

void fonction_ellipse_p(double d, void *param, P3D res){
  double *R = (double*)param;
  res[0] = -R[0]*sin(d);
  res[1] = R[1]*cos(d);
  res[2] = 0.0;
}

void fonction_carre(double d, void *param, P3D res){
  double *R = (double*)param;
  double tmp = 1+pow(cos(2*d),2);
  res[0] = R[0]/pow(tmp, 1.0/(4.0-R[1]));
  res[1] = d;
  res[2] = 0.0;
}

void fonction_carre_p(double d, void *param, P3D res){
  double *R = (double*)param;
  double tmp = 1+pow(cos(2*d),2);
  double p = 1.0+1.0/(4.0-R[1]);
  double dr = 8*R[0]*cos(2*d)*sin(2*d)/((4.0-R[1])*pow(tmp, p));
  res[0] = dr;
  res[1] = 1.0;
  res[2] = 0.0;
}

void fonction_triangle(double d, void *param, P3D res){
  double *R = (double*)param;
  double tmp = 3./2.+cos(3*d);
  res[0] = R[0]/pow(tmp, 1.0/R[1]);
  res[1] = d;
  res[2] = 0.0;
}

void fonction_triangle_p(double d, void *param, P3D res){
  double *R = (double*)param;
  double tmp = 3./2.+cos(3*d);
  double p = 1.0+1.0/R[1];
  double dr = 3*R[0]*sin(3*d)/(R[1]*pow(tmp, p));
  res[0] = dr;
  res[1] = 1.0;
  res[2] = 0.0;
}

void fonction_bezier(double d, void *param, P3D res){
  Bezier *b = (Bezier*) param;
  int i;
  double tmp;
  memset(res, 0, 3*sizeof(double));
  for(i = 0;i<b->nb;i++){
    tmp = b->coeff[i]*pow(d,i)*pow(1.0-d,b->nb-1-i);
    res[0] += b->pts[i][0]*tmp;
    res[1] += b->pts[i][1]*tmp;
  }
}

void fonction_bezier_p(double d, void *param, P3D res){
  Bezier *b = (Bezier*) param;
  int i;
  double tmp;
  memset(res, 0, 3*sizeof(double));
  for(i = 0;i<b->nb;i++){
    if(i>0){
      tmp = i*b->coeff[i]*pow(d,i-1)*pow(1.0-d,b->nb-1-i);
      res[0] += b->pts[i][0]*tmp;
      res[1] += b->pts[i][1]*tmp;
    }
    if(i<b->nb-1){
      tmp = (b->nb-1-i)*b->coeff[i]*pow(d,i)*pow(1.0-d,b->nb-i-2);
      res[0] -= b->pts[i][0]*tmp;
      res[1] -= b->pts[i][1]*tmp;
    }
  }
}

void fonction_ligne_surf_param(double d, void *param, P3D res){
  Courbe_surf_param *lsurf = (Courbe_surf_param*) param;
  Surf_param *s = lsurf->s;
  CourbeP *cp = lsurf->c;
  Pt pt;
  P3D tmp;
  if(cp != NULL)
    courbeP_get_image(cp, d, tmp);
  else{
    tmp[(lsurf->bool+1)%2] = lsurf->l2;
    tmp[lsurf->bool%2] = d;
  }
  surf_setpt(s, &pt, tmp);
  VECT_CPY(res, pt.p);
}

/*
* N'appelle pas courbeP_get_image_p (mais fait pourtant la même chose)
* ==> petit gain de temps
*/
void fonction_ligne_surf_param_p(double d, void *param, P3D res){
  Courbe_surf_param *lsurf = (Courbe_surf_param*) param;
  P3D par, tmp[3];
  CourbeP *cp = lsurf->c;
  int i;

  if(cp != NULL)
    courbeP_get_image(cp, d, par);  
  else{
     par[(lsurf->bool+1)%2] = lsurf->l2;
     par[lsurf->bool] = d;
  }
  image_cvect_p(lsurf->s->f, par, tmp);
  if(cp == NULL){
    int bool = lsurf->bool%2;
    if(bool){
      for(i = 0;i<3;i++)
	res[i] = tmp[i][1];
    } else {
      for(i = 0;i<3;i++)
	res[i] = tmp[i][0];
    } 
  } else {
    P3D tmp2;
    image_fvect_p(cp->param, d, tmp2);
    prod_mat_vect(tmp, 3, 2, tmp2, res);
  }
}

void fonction_mult_courbes(double d, void *param, P3D res){
  NewMult_courbes *_param = (NewMult_courbes*) param;
  Mult_courbes *mult = (Mult_courbes*) _param->courbes;
  int pos = mult_courbes_get_pos_dicho(mult, d, _param->pos);
  CourbeP *seg = mult->c[pos];
  double coeff = (seg->p_fin - seg->p_init)
    /(mult->fin[pos] - mult->init[pos]);
  double pos_seg = (d - mult->init[pos])*coeff;
  image_fvect(seg->param, pos_seg, res);
}

void fonction_mult_courbes_p(double d, void *param, P3D res){
  NewMult_courbes *_param = (NewMult_courbes*) param;
  Mult_courbes *mult = (Mult_courbes*) _param->courbes;
  int pos = mult_courbes_get_pos_dicho(mult, d, _param->pos);
  CourbeP *seg = mult->c[pos];
  double coeff = (seg->p_fin - seg->p_init)
    /(mult->fin[pos] - mult->init[pos]);
  double pos_seg = (d - mult->init[pos])*coeff;
  image_fvect_p(seg->param, pos_seg, res);
  homothetie(res, coeff);
}

Fonc_vect* create_fonction_ellipse(double a, double b, 
				   double r1, double r2){
  double *R = malloc(4*sizeof(double));
  R[0] = r1; R[1] = r2;
  R[2] = a;  R[3] = b;
  return create_fonction_vect(fonction_ellipse, 
			      fonction_ellipse_p, R);
}

Fonc_vect* create_fonction_droite_surf_param(Surf_param *s, 
					    int bool, 
					    double l2){
  Courbe_surf_param *par = create_droite_surf_param(s, bool, l2);
  return create_fonction_vect(fonction_ligne_surf_param, 
			      fonction_ligne_surf_param_p, 
			      par);
}

Fonc_vect* create_fonction_courbe_surf_param(Surf_param *s, 
					     CourbeP *c){
  Courbe_surf_param *par = create_courbe_surf_param(s, c);
  return create_fonction_vect(fonction_ligne_surf_param, 
			      fonction_ligne_surf_param_p, 
			      par);
}

Fonc_vect* create_fonction_bezier(P3D *pts, P3D *der, 
				  double h){
  Bezier *be;
  if(pts != NULL){
    P3D _pts[4];
    int j;
    VECT_CPY(_pts[0], pts[0]);
    VECT_CPY(_pts[3], pts[1]);
    for(j = 0;j<3;j++){
      _pts[1][j] = pts[0][j] + h*der[0][j];
      _pts[2][j] = pts[1][j] - h*der[1][j];
    }
    be = createBezier(_pts, 4);
  } else {
    be = createBezier(NULL, 4);
  }
  return create_fonction_vect(fonction_bezier, 
			      fonction_bezier_p, be);
}

Fonc_vect* create_fonction_mult_courbes(PCourbeP *c, int nb){
  Mult_courbes *mult = create_mult_courbes(nb, c, 0.0, 1.0);
  NewMult_courbes *param = create_newMult_courbes(mult, -1);
  return create_fonction_vect(fonction_mult_courbes, 
			      fonction_mult_courbes_p, param);
}

Fonc_vect* create_fonction_carre(double d1, double d2){
  double *R = malloc(2*sizeof(double));
  R[0] = d1; R[1] = d2;
  return create_fonction_vect(fonction_carre, fonction_carre_p, R);
}
Fonc_vect* create_fonction_triangle(double d1, double d2){
  double *R = malloc(2*sizeof(double));
  R[0] = d1; R[1] = d2;
  return create_fonction_vect(fonction_triangle, 
			      fonction_triangle_p, R);
}

void set_pts_fonc_bezier(Fonc_vect *bez, P3D pts[2], 
			 P3D der[2], double h){
  P3D _pts[4];
  int j;
  Bezier *be = (Bezier*) bez->param;
  VECT_CPY(_pts[0], pts[0]);
  VECT_CPY(_pts[3], pts[1]);
  for(j = 0;j<3;j++){
    _pts[1][j] = pts[0][j] + h*der[0][j];
    _pts[2][j] = pts[1][j] - h*der[1][j];
  }
  set_pts_bezier(be, _pts);
}

void homothetie_der_fonc_bezier(Fonc_vect *fonc_bez, double lam){
  P3D der1, der2;
  int i, n;
  Bezier *bez = (Bezier*) fonc_bez->param;
  n = bez->nb-1;
  VECT_DEL(der1, bez->pts[0], bez->pts[1], i);
  VECT_DEL(der2, bez->pts[n], bez->pts[n-1], i);
  homothetie(der1, lam);
  homothetie(der2, lam);
  for(i = 0;i<3;i++){
    bez->pts[1][i] = bez->pts[0][i] + der1[i];
    bez->pts[n-1][i] = bez->pts[n][i] + der2[i];
  }
}

void ajouter_pt_fonc_bezier(Fonc_vect *fonc_bez, double h){
  P3D p, del, orth;
  int i;
  Bezier *bez = (Bezier*) fonc_bez->param;
  VECT_DEL(del, bez->pts[1], bez->pts[2], i);
  orth[0] = -del[1]; orth[1] = del[0]; orth[2] = 0.0;
  normer(orth);
  for(i = 0;i<2;i++){
    p[i] = (bez->pts[1][i]+bez->pts[2][i])/2.0 + h*orth[i];
  }
  p[2] = 0.0;
  ajouter_pt_bezier(bez, p, 2);
}

void homothetie_pt_inter_fonc_bezier(Fonc_vect *fonc_bez, double lam){
  P3D vect;
  int i;
  Bezier *bez = (Bezier*) fonc_bez->param;
  for(i = 0;i<3;i++){
    vect[i] = bez->pts[2][i] 
      - (bez->pts[1][i]+bez->pts[3][i])/2.0;
  }
  homothetie(vect, lam);
  for(i = 0;i<2;i++){
    bez->pts[2][i] = (bez->pts[1][i]+bez->pts[3][i])/2.0 
      + vect[i];
  }
}

/*
 * Découpe une courbe de Bézier en 2 courbes jointent
 * bool : première découpe ou non
 */
CourbeP* split_courbe_bezier(CourbeP *courbe, 
			     int pos, double h, double prop){
    P3D pts[2], der[2];
  P3D dummy = {0.,0.,0.};
    Bezier *bez;
    Fonc_vect *nfonc;
    CourbeP *ncourbe;
    int i;
    int nb;
    PCourbeP courbes[2];
    Fonc_vect *res_fonc = NULL;
    CourbeP *res_courbe = NULL;
    CourbeP *courbe_bez = NULL;
    Mult_courbes *mult = NULL;
    int bool = (pos<0);

    if(bool){
      courbe_bez = courbe;
    } else {
      mult = (Mult_courbes*) courbe->param->param;
      courbe_bez = mult->c[pos];
      res_courbe = courbe;
    }
    bez = (Bezier*) courbe_bez->param->param;
    nb = bez->nb;
    VECT_CPY(pts[0], bez->pts[0]);
    VECT_DEL(der[0], bez->pts[0], bez->pts[1], i);

    image_fvect(courbe_bez->param, prop, pts[1]);
    image_fvect_p(courbe_bez->param, prop, der[1]);
    homothetie(der[1], h*prop/norme(der[1]));
    homothetie(der[0], h*prop/norme(der[0]));

    nfonc = create_fonction_bezier(pts, der, 1.0);
    ncourbe = create_courbeP(nfonc, 0.0, 1.0, 0, 0.0, 0, dummy);

    VECT_CPY(pts[0], pts[1]);
    VECT_CPY(der[0], der[1]);
    VECT_DEL(der[1], bez->pts[nb-2], bez->pts[nb-1], i);
    VECT_CPY(pts[1], bez->pts[nb-1]);
    homothetie(der[0], h*prop/norme(der[0]));
    homothetie(der[1], h*prop/norme(der[1]));
    set_pts_fonc_bezier(courbe_bez->param, pts, der, 1.0);

    if(bool){
      P3D dummy = {0.,0.,0.};
      /*
       * on découpe pour la 1ere fois => on créé la structure
       */
      courbes[0] = ncourbe;
      courbes[1] = courbe_bez;
      res_fonc = create_fonction_mult_courbes(courbes, 2);
      res_courbe = create_courbeP(res_fonc, 0.0, 1.0, 
				  0, 0.0, 0,dummy);
    } else {
      mult_courbes_ajouter_courbe(mult, pos, ncourbe);
    }
    return res_courbe;
}

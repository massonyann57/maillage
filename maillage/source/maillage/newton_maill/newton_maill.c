#include "newton_maill.h"

/*
 * Constructeur de Param_new1 (paramètre de l'application définissant 
 * les contraintes pour le calcul du 4eme point d'un ||gramme)
 */
Param_new1*
create_param_new1(P3D c1, P3D c2, double R1, 
		  double R2, Surf_param *s, Param_maillage *param){
  Param_new1* res = malloc(sizeof(Param_new1));
  VECT_CPY(res->c1, c1);
  VECT_CPY(res->c2, c2);
  res->R1 = R1;
  res->R2 = R2;
  res->s = s;
  res->param = param;
  return res;
}

/*
 * Constructeur de Param_new2 (paramètre de l'application définissant 
 * les contraintes pour le calcul du point suivant d'une courbe)
 */
Param_new2*
create_param_new2(P3D pos, P3D* base, P3D centre, double R,
		  double theta, Surf_param *s){
  Param_new2* res = malloc(sizeof(Param_new2));
  VECT_CPY(res->pos, pos);
  VECT_CPY(res->centre, centre);
  if(base != NULL){
    VECT_CPY(res->base[0], base[0]);
    VECT_CPY(res->base[1], base[1]);
  } else {
    VECT_NULL(res->base[0]);
    VECT_NULL(res->base[1]);
  }
  res->R = R;
  res->the = theta;
  res->s = s;
  return res;
}

/*
 * Constructeur de Param_new3 (paramètre de l'application définissant 
 * les contraintes pour le calcul du point suivant d'une ligne de 
 * paramétrage d'une surface paramétrée)
 */
Param_new3*
create_param_new3(CourbeP *c, double R, P3D pos){
  Param_new3* res = malloc(sizeof(Param_new3));
  res->c = c;
  VECT_CPY(res->pos, pos);
  res->R = R;
  return res;
}

/*
 * Constructeur de Param_new4 (paramètre de l'application définissant 
 * les contraintes pour le calcul du point suivant d'une 
 * courbe avec une singularitée )
 */
Param_new4*
create_param_new4(Pt* pos, P3D* dir, double R1, 
		  double R2, Surf_param *s, 
		  Param_new2 *p_inter1, 
		  Param_new2 *p_inter2, Param_new5 *p_inter3, 
		  Param_maillage *param_m, Pt *inter1, 
		  Pt *inter2, Pt *inter_f){
  Param_new4* res = malloc(sizeof(Param_new4));
  res->pos = pos;
  VECT_CPY(res->dir[0], dir[0]);
  VECT_CPY(res->dir[1], dir[1]);
  res->R1 = R1;
  res->R2 = R2;
  res->s = s;
  res->p_inter1 = p_inter1;
  res->p_inter2 = p_inter2;
  res->p_inter3 = p_inter3;
  res->inter1 = inter1;
  res->inter2 = inter2;
  res->res = inter_f;
  res->param_m = param_m;
  return res;
}

/*
 * Constructeur de Param_new5 (paramètre de l'application définissant 
 * les contraintes pour le calcul de l'intersection de 2 plans 
 * et d'une surface)
 */
Param_new5*
create_param_new5(P3D pos1, P3D pos2, P3D nor1, P3D nor2,
		  Surf_param *s){
  Param_new5* res = malloc(sizeof(Param_new5));
  VECT_CPY(res->pos1, pos1);
  VECT_CPY(res->pos2, pos2);
  VECT_CPY(res->nor1, nor1);
  VECT_CPY(res->nor2, nor2);
  res->s = s;
  return res;
}

/*
 * application définissant les contraintes pour le calcul 
 * du 4eme point d'un ||gramme
 */
/*void f_newton_contr_surf(P3D x, void *p, double *res){
  Param_new1* p_new = (Param_new1*) p;
  Surf_contr *s = p_new->s->s_contr; 
  res[0] = func_sphere(x, p_new->R1, p_new->c1);
  res[1] = func_sphere(x, p_new->R2, p_new->c2);
  res[2] = image_cr(s->f, x);
  }*/

/*
 * dérivée de l'application ci-dessus
 */
/*
void f_newton_contr_surf_p(P3D x, void *p, double res[3][3],
			     double *grad){
  P3D tmp;
  int i, j;
  double n;
  Param_new1* p_new = (Param_new1*) p;
  Surf_contr *s = p_new->s->s_contr; 
  func_sphere_p(x, p_new->R1, p_new->c1, res[0]);
  func_sphere_p(x, p_new->R2, p_new->c2, res[1]);
  image_cr_p(s->f, x, res[2]);
  
  f_newton_contr_surf(x, p, tmp);
  n = norme(tmp);
  for(i=0;i<3;i++){
    tmp[i] /= n;
    for(j=0;j<3;j++){
      res[i][j] *= signe(tmp[i]);
    }
  }
  for(i=0;i<3;i++){
    grad[i] = 0;
    for(j=0;j<3;j++){
      grad[i] += 2*(res[j][i]*tmp[j]);
    }
  }
  }*/

/*
 * application définissant les contraintes pour le calcul 
 * du point suivant d'une courbe
 */
/*void f_newton_contr_courbe(P3D x, void *p, double *res){
  Param_new2* p_new = (Param_new2*) p;
  Surf_contr *s = p_new->s->s_contr; 
  res[0] = func_sphere(x, p_new->R, p_new->centre);
  res[1] = fonc_angle(x, p_new->base, p_new->the, p_new->pos);
  res[2] = image_cr(s->f, x);
  }*/

/*
 * dérivée de l'application ci-dessus
 */
/*void f_newton_contr_courbe_p(P3D x, void *p, double res[3][3],
			     double *grad){
  Param_new2* p_new = (Param_new2*) p;
  Surf_contr *s = p_new->s->s_contr; 
  int i, j;
  double n;
  P3D tmp;
  func_sphere_p(x, p_new->R, p_new->centre, res[0]);
  fonc_angle_p(x, p_new->base, p_new->the, 
	  p_new->pos, res[1]);
  image_cr_p(s->f, x, res[2]);
  f_newton_contr_courbe(x, p, tmp);
  n = norme(tmp);
  for(i=0;i<3;i++){
    tmp[i] /= n;
    for(j=0;j<3;j++){
      res[i][j] *= signe(tmp[i]);
    }
  }
  
  for(i=0;i<3;i++){
    grad[i] = 0;
    for(j=0;j<3;j++){
      grad[i] += 2*(res[j][i]*tmp[j]);
    }
  }
  }*/


/* Pour les surfaces définie par un paramètre */

/*
 * application définissant les contraintes pour le calcul 
 * du 4eme point d'un ||gramme
 */
void f_newton_param_surf(P3D x, void *p, double *res){
  Param_new1* p_new = (Param_new1*) p;
  Surf_param *s = p_new->s; 
  P3D tmp;
  image_cvect(s->f, x, tmp);
  res[0] = func_sphere(tmp, p_new->R1, p_new->c1);
  res[1] = func_sphere(tmp, p_new->R2, p_new->c2);
}

/*
 * dérivée de l'application ci-dessus
 */
void f_newton_param_surf_p(P3D x, void *p, double res[3][3],
			     double *grad){
  P3D tmp;
  int i, j;
  double n;
  Param_new1* p_new = (Param_new1*) p;
  Surf_param *s = p_new->s; 
  P3D pt_tmp;
  P3D mat_tmp[3];
  P3D der_tmp[3];
  image_cvect(s->f, x, pt_tmp);
  func_sphere_p(pt_tmp, p_new->R1, p_new->c1, mat_tmp[0]);
  func_sphere_p(pt_tmp, p_new->R2, p_new->c2, mat_tmp[1]);
  image_cvect_p(s->f, x, der_tmp); prod_mat(mat_tmp, 2, 3, der_tmp, 2, res);
  
  f_newton_param_surf(x, p, tmp);
  n = _norme(tmp, 2);
  for(i=0;i<2;i++){
    tmp[i] /= n;
    for(j=0;j<2;j++){
      res[i][j] *= signe(tmp[i]);
    }
  }
  for(i=0;i<2;i++){
    grad[i] = 0;
    for(j=0;j<2;j++){
      grad[i] += 2*(res[j][i]*tmp[j]);
    }
  }
}

/*
 * application définissant les contraintes pour le calcul 
 * du point suivant d'une courbe
 */
void f_newton_param_courbe(P3D x, void *p, double *res){
  Param_new2* p_new = (Param_new2*) p;
  Surf_param *s = p_new->s; 
  P3D tmp;
  image_cvect(s->f, x, tmp);
  res[0] = func_sphere(tmp, p_new->R, p_new->centre);
  res[1] = fonc_angle(tmp, p_new->base, p_new->the, p_new->pos);
}

/*
 * dérivée de l'application ci-dessus
 */
void f_newton_param_courbe_p(P3D x, void *p, double res[3][3],
			     double *grad){
  P3D tmp;
  int i, j;
  double n;
  Param_new2* p_new = (Param_new2*) p;
  Surf_param *s = p_new->s; 
  P3D pt_tmp;
  P3D mat_tmp[3];
  P3D der_tmp[3];
  image_cvect(s->f, x, pt_tmp);

  func_sphere_p(pt_tmp, p_new->R, p_new->centre, mat_tmp[0]);
  fonc_angle_p(pt_tmp, p_new->base, p_new->the, p_new->pos, mat_tmp[1]);
  image_cvect_p(s->f, x, der_tmp);
  prod_mat(mat_tmp, 2, 3, der_tmp, 2, res);
  
  f_newton_param_courbe(x, p, tmp);
  n = _norme(tmp, 2);
  for(i=0;i<2;i++){
    tmp[i] /= n;
    for(j=0;j<2;j++){
      res[i][j] *= signe(tmp[i]);
    }
  }
  for(i=0;i<2;i++){
    grad[i] = 0;
    for(j=0;j<2;j++){
      grad[i] += 2*(res[j][i]*tmp[j]);
    }
  }
}

/*
 * application définissant les contraintes pour le calcul 
 * du point suivant d'une ligne d'une surface paramétrée
 */
void f_newton_param_ligne(P3D x, void *p, double *res){
  Param_new3* p_new = (Param_new3*) p;
  P3D pttmp;
  image_fvect(p_new->c->param, x[0], pttmp);
  res[0] = func_sphere(pttmp, p_new->R, p_new->pos);
}

/*
 * dérivée de l'application ci-dessus
 */
void f_newton_param_ligne_p(P3D x, void *p, double res[3][3],
			   double *grad){
  Param_new3* p_new = (Param_new3*) p;
  P3D der_tmp;
  P3D grad_tmp;
  P3D tmp;
  P3D pttmp;
  image_fvect(p_new->c->param, x[0], pttmp);
  func_sphere_p(pttmp, p_new->R, p_new->pos, der_tmp);
  image_fvect_p(p_new->c->param, x[0], grad_tmp);
  res[0][0] = prod_scal(der_tmp, grad_tmp);

  f_newton_param_ligne(x, p, tmp);
  res[0][0] *= signe(tmp[0]);
  grad[0] = res[0][0];
}

/* calcul des courbes avec une singularitée */

/*
 * application définissant les contraintes pour le calcul 
 * du point suivant d'une courbe avec une singularitée 
 * d'une surface paramétrée.
 * Commun aux surfaces paramétrées et aux surfaces définies
 * par une contrainte
 * x[0] : le pas sur la 1ère courbe
 * x[1] : le pas sur la 2ème courbe
 * res[0] : le pas obtenu pour la 3ème courbe
 * res[1] : le pas obtenu pour la 4ème courbe
 */
void f_newton_param_courbe_sing(P3D x, void *p, double *res){
  Param_new4* p_new = (Param_new4*) p;
  P3D base[2], metr[2];
  int i;

  p_new->p_inter1->R = x[0];
  p_new->p_inter2->R = x[1];
  point_suivant_courbeG(p_new->pos, 
		       NULL, p_new->p_inter1, 
		       p_new->param_m->nbMax_new, 
		       p_new->inter1, p_new->param_m, 
			NULL, 0.0, NULL);
  point_suivant_courbeG(p_new->pos, 
		       NULL, p_new->p_inter2, 
		       p_new->param_m->nbMax_new, 
		       p_new->inter2, p_new->param_m, 
			NULL, 0.0, NULL);
  surf_calc_base_opt(p_new->s, p_new->pos->param, 
		      base, metr);
  inters_double_plan_surface(p_new->inter1, p_new->inter2,
			     p_new->p_inter3->nor1, 
			     p_new->p_inter3->nor2, 
			     base, p_new->p_inter3, 
			     p_new->param_m->nbMax_new, 
			     p_new->res, p_new->param_m);
  for(i = 0;i<3;i++){
    p_new->delta[0][i] = p_new->res->p[i] - p_new->inter1->p[i];
    p_new->delta[1][i] = p_new->res->p[i] - p_new->inter2->p[i];
  }
  res[0] = norme(p_new->delta[0]) - p_new->R1;
  res[1] = norme(p_new->delta[1]) - p_new->R2;
}

/*
 * dérivée de l'application ci-dessus
 */
void f_newton_param_courbe_sing_p(P3D x, void *p, double res[3][3],
				  double *grad){
  Param_new4* p_new = (Param_new4*) p;
  Surf_param *s = p_new->s; 
  P3D tmp;
  int i, j;
  P3D base[3], metr[3], loc_v[2];
  P3D mat_inv[3];
  double n;
  double ps;
  P3D v_orth;

  f_newton_param_courbe_sing(x, p, tmp);
  surf_calc_base_opt(s, p_new->pos->param, 
		      base, metr);
  loc_v[0][1] = -prod_scal(base[1], p_new->dir[1]);
  loc_v[0][0] = -prod_scal(base[1], p_new->dir[0]);
  loc_v[1][1] = prod_scal(base[0], p_new->dir[1]);
  loc_v[1][0] = prod_scal(base[0], p_new->dir[0]);
  inverse(loc_v, 2, mat_inv);

  res[0][0] = -mat_inv[0][0]*loc_v[1][0] 
    + mat_inv[0][1]*loc_v[0][0];
  res[1][0] = -mat_inv[1][0]*loc_v[1][0] 
    + mat_inv[1][1]*loc_v[0][0];
  res[0][1] = mat_inv[0][0]*loc_v[1][1] 
    - mat_inv[0][1]*loc_v[0][1];
  res[1][1] = mat_inv[1][0]*loc_v[1][1] 
    - mat_inv[1][1]*loc_v[0][1];
    
  for(i = 0;i<2;i++){
    for(j = 0;j<3;j++)
      v_orth[j] = loc_v[0][i]*base[0][j]
	+ loc_v[1][i]*base[1][j];
    ps = prod_scal(v_orth, p_new->delta[i]);
    res[i][0] *= (-2*i+1)*signe(ps);
    res[i][1] *= (-2*i+1)*signe(ps);
  }

  n = _norme(tmp, 2);
  for(i=0;i<2;i++){
    tmp[i] /= n;
    for(j=0;j<2;j++){
      res[i][j] *= signe(tmp[i]);
    }
  }
  for(i=0;i<2;i++){
    grad[i] = 0;
    for(j=0;j<2;j++){
      grad[i] += 2*(res[j][i]*tmp[j]);
    }
  }
}

/*
 * application définissant les contraintes pour le calcul 
 * de l'intersection de 2 plans avec une surface
 */
/*void f_newton_contr_inter_plan(P3D x, void *p, double *res){
  Param_new5* p_new = (Param_new5*) p;
  Surf_contr *s = p_new->s->s_contr; 
  P3D tmp1, tmp2;
  int i;
  for(i = 0;i<3;i++){
    tmp1[i] = x[i] - p_new->pos1[i];
    tmp2[i] = x[i] - p_new->pos2[i];
  }
  res[0] = prod_scal(tmp1, p_new->nor1);
  res[1] = prod_scal(tmp2, p_new->nor2);
  res[2] = image_cr(s->f, x);
  }*/


/*
 * dérivée de l'application ci-dessus
 */
/*void f_newton_contr_inter_plan_p(P3D x, void *p, double res[3][3],
		 double *grad){
  Param_new5* p_new = (Param_new5*) p;
  Surf_contr *s = p_new->s->s_contr; 
  int i, j;
  double n;
  P3D tmp;
  for(i=0;i<3;i++){
    res[0][i] = 2*p_new->nor1[i];
    res[1][i] = 2*p_new->nor2[i];
  }
  image_cr_p(s->f, x, res[2]);
  f_newton_contr_inter_plan(x, p, tmp);
  n = norme(tmp);
  for(i=0;i<3;i++){
    tmp[i] /= n;
    for(j=0;j<3;j++){
      res[i][j] *= signe(tmp[i]);
    }
  }
  
  for(i=0;i<3;i++){
    grad[i] = 0;
    for(j=0;j<3;j++){
      grad[i] += 2*(res[j][i]*tmp[j]);
    }
  }
  }*/

/*
 * application définissant les contraintes pour le calcul 
 * de l'intersection de 2 plans avec une surface
 */
void f_newton_param_inter_plan(P3D x, void *p, double *res){
  Param_new5* p_new = (Param_new5*) p;
  Surf_param *s = p_new->s; 
  P3D tmp1, tmp2;
  P3D tmp;
  int i;
  image_cvect(s->f, x, tmp);
  for(i = 0;i<3;i++){
    tmp1[i] = tmp[i] - p_new->pos1[i];
    tmp2[i] = tmp[i] - p_new->pos2[i];
  }
  res[0] = prod_scal(tmp1, p_new->nor1);
  res[1] = prod_scal(tmp2, p_new->nor2);
}

/*
 * dérivée de l'application ci-dessus
 */
void f_newton_param_inter_plan_p(P3D x, void *p, double res[3][3],
				 double *grad){
  Param_new5* p_new = (Param_new5*) p;
  Surf_param *s = p_new->s; 
  int i, j;
  double n;
  P3D tmp;
  P3D pt_tmp;
  P3D mat_tmp[3];
  P3D der_tmp[3];

  image_cvect(s->f, x, pt_tmp);
  for(i=0;i<3;i++){
    mat_tmp[0][i] = 2*p_new->nor1[i];
    mat_tmp[1][i] = 2*p_new->nor2[i];
  }
  image_cvect_p(s->f, x, der_tmp);
  prod_mat(mat_tmp, 2, 3, der_tmp, 2, res);
  f_newton_param_inter_plan(x, p, tmp);
  n = norme(tmp);
  for(i=0;i<3;i++){
    tmp[i] /= n;
    for(j=0;j<3;j++){
      res[i][j] *= signe(tmp[i]);
    }
  }
  
  for(i=0;i<3;i++){
    grad[i] = 0;
    for(j=0;j<3;j++){
      grad[i] += 2*(res[j][i]*tmp[j]);
    }
  }
}

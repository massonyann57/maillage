#include "surface.h"

/*
 * Constructeur de Surf_param (surface paramétrée)
 */
Surf_param*
create_surf_param(Champ_vect *f, double p1_init,
		  double p1_fin, double p2_init,
		  double p2_fin,
		  int ind_quotient){
  Surf_param* res = malloc(sizeof(Surf_param));
  res->f = f;
  res->init[0] = p1_init;
  res->init[1] = p2_init;
  res->fin[0] = p1_fin;
  res->fin[1] = p2_fin;
  res->ind_quotient = ind_quotient;
  res->gauss = NULL;
  return res;
}

/*
 * Constructeur de Surf_contr (surface définie par une contrainte)
 * f : la contrainte définissant la surface
 */
Surf_contr*
create_surf_contr(Champ_r *f){
  Surf_contr* res = malloc(sizeof(Surf_contr));
  res->f = f;
  return res;
}

/*
 * Constructeur de Surf :
 * type : le type de la surface
 *      = 1 si surface paramétrée
 *      = 2 si surface définie par contrainte
 *      = 3 si possède les 2 définitions
 */
Surf* create_surf(int type, Champ_vect* f_param,
		  double init1, double fin1,
		  double init2, double fin2,
		  int ind_quotient,
		  Champ_r* f_contr){
  Surf* res = malloc(sizeof(Surf));
  res->type = type;
  res->s_contr = NULL;
  res->s_param = NULL;
  if (type == 1 || type == 3){
    res->s_param =
      create_surf_param(f_param, init1, fin1,
			init2, fin2, ind_quotient);
  }
  if (type == 2 || type == 3){
    res->s_contr = create_surf_contr(f_contr);
  }
  return res;
}


void get_normal_surf_contr(Surf_contr *s, P3D pt, P3D normal){
  int i;
  double n;
  image_cr_p(s->f, pt, normal);
  n = norme(normal);
  for (i = 0; i < 3; i++)
    normal[i] /= n;
}


/*
 * Retourne la base associé à la paramétrisation de s au
 * point de paramètre pt
 */
int surf_calc_base(Surf_param *s, P3D pt,
		   P3D base[2], P3D metrique[2]){
  int i, j;
  double n;
  P3D b_tmp[3];
  double tmp = 1.0;

  if (!surf_est_dans_borne(s, pt))
    return 0;
  image_cvect_p(s->f, pt, b_tmp);
  for (i = 0; i < 2; i++){
    for (j = 0; j < 3; j++)
      base[i][j] = b_tmp[j][i];
    n = norme(base[i]);
    metrique[i][i] = n*n;
    if (!estNulle(n)){
      for (j = 0; j < 3; j++)
	base[i][j] /= n;
    }
    else{
      for (j = 0; j < 3; j++)
	base[i][j] = 0.0;
    }
    tmp *= n;
  }
  metrique[0][1] = prod_scal(base[0], base[1])*tmp;
  metrique[1][0] = metrique[0][1];
  return 1;
}

/*
 * Retourne la base associé à la paramétrisation de s au
 * point de paramètre pt
 */
int surf_calc_base_opt(Surf_param *s, P3D pt,
		       P3D base[2], P3D metrique[2]){
  if (!surf_calc_base(s, pt, base, metrique))
    return 0;
  if (estPetit(deter(metrique, 2))){
    double _eps = 0.02;
    double me;
    int ind = 0;
    int sgn = 1;
    if (estNulle(metrique[0][0]))
      ind = 1;
    me = metrique[ind][ind];
    pt[ind] += _eps / me;
    if (pt[ind] > s->fin[ind]){
      pt[ind] -= 2.0 * _eps / me;
      sgn = -1;
    }
    if (!surf_calc_base(s, pt, base, metrique))
      return 0;
    pt[ind] -= ((double)sgn)*_eps / me;
  }
  return 1;
}

void surf_get_normal(Surf_param *s, P3D param, P3D normal){
  P3D dummy[2], base[2];
  double n;
  int i;

  surf_calc_base(s, param, base, dummy);
  prod_vect(normal, base[0], base[1]);
  n = norme(normal);
  for (i = 0; i < 3; i++)
    normal[i] /= n;
}

void surf_projeter_tangent(Surf_param *s, Pt* pt, 
			   P3D dir, P3D res){
  int i;
  P3D nor;
  double ps;
  //  if (s->type == 1){
    P3D base[2], dummy[2];
    double n;
    surf_calc_base(s, pt->param, base, dummy);
    prod_vect(nor, base[0], base[1]);
    n = norme(nor);
    for (i = 0; i < 3; i++)
      nor[i] /= n;
    /* }
  else {
    get_normal_surf_contr(s, pt->p, nor);
    }*/
  ps = prod_scal(dir, nor);
  for (i = 0; i < 3; i++)
    res[i] = dir[i] - ps*nor[i];
}

/*
 * calcul la base orthonormé de s en pt
 *   ayant dir comme 1er vecteur
 */
int surf_calc_bon_dir(Surf_param *s, Pt* pt,
		      P3D dir, P3D res[2], int or_base){
  /*  if (s->type == 2){
    P3D normal;
    get_normal_surf_contr(s, pt->p, normal);
    calc_nouv_base_contr(normal, dir, res);
    return 1;
  }
  else{*/
    P3D der[2], dummy[2];
    P3D _dir;
    surf_projeter_tangent(s, pt, dir, _dir);
    if (!surf_calc_base(s, pt->param, der, dummy))
      return 0;
    calc_base_dir(der, dummy, _dir, res);
    orienter_base(res, or_base, der);
    return 1;
    //  }
}

int surf_est_dans_borne(Surf_param *s, P3D p){
  double *init = s->init;
  double *fin = s->fin;
  if (p[0] < init[0] - EPS_TOL || p[0] > fin[0] + EPS_TOL
      || p[1] < init[1] - EPS_TOL || p[1] > fin[1] + EPS_TOL)
    return 0;
  return 1;
}

void surf_proj_borne(Surf_param *s, P3D p, P3D res){
  double *init = s->init;
  double *fin = s->fin;
  int i;
  for (i = 0; i < 2; i++){
    res[i] = _fmin(_fmax(init[i], p[i]), fin[i]);
  }
}

/*
 * calcul les coordonnées de vect dans la base associée
 * à la paramétrisation
 */
void surf_coordonne_base(Surf_param *s, P3D param,
			 P3D vect, P3D res){
  P3D base[2], metr[2];
  surf_calc_base(s, param, base, metr);
  coordonne(vect, base, metr, res, -1, 1, 1);
}

void surf_export_curvature(Surf_param *s, Pt *pts, 
			   int nb, char *path){
  int i, j;
  FILE *f;
  double the;
  P3D tmp1, tmp2;
  P3D base[2];

  f = fopen(path, "w+");
  VECT_DEL(tmp2, pts[0].p, pts[1].p, j);
  for (i = 1; i < nb - 1; i++){
    VECT_CPY(tmp1, tmp2);
    VECT_DEL(tmp2, pts[i].p, pts[i + 1].p, j);
    surf_calc_bon_dir(s, &pts[i], tmp1, base, 1);
    the = angle(tmp1, tmp2);
    the *= signe(prod_scal(tmp2, base[1]));
    fprintf(f, "%lf\n", the);
  }
  fclose(f);
}

/*
 * créé la courbe de la surface associée à une courbe
 * dans l'espace des paramétrisations
 */
CourbeP* surf_get_courbe_from_courbeP(Surf_param *s,
				      CourbeP *c_plan){
  Fonc_vect *fonc;
  double param_init, param_fin, init_p;
  int sgn = 1;
  int est_per=0;
  P3D dummy = {0.,0.,0.};

  if (c_plan->param == NULL){
    P3D dummy[2], metr[2];
    P3D tmp, tmp2;
    int i;
    int dir = c_plan->dir;
    int _dir = dir % 2;
    int _bool[2];

    _bool[0] = s->ind_quotient % 2;
    _bool[1] = s->ind_quotient / 2;
    sgn = 1 - 2 * (dir / 2);

    fonc =
      create_fonction_droite_surf_param(s, dir,
					c_plan->param_init[(dir + 1) % 2]);
    param_init = s->init[_dir];
    param_fin = s->fin[_dir];
    init_p = c_plan->param_init[_dir];

    /* on regarde si le bout de la courbe est une singularité :
     * si c'est le cas on double la limite
     * (et on prend en compte cette singularité
     * lors du déplacement du point)
     */
    for (i = 0; i < 2; i++){
      if (_dir == i){
	tmp[i] = s->init[i];
	tmp2[i] = s->fin[i];
      }
      else{
	tmp[i] = (s->init[i] + s->fin[i]) / 2.0;
	tmp2[i] = tmp[i];
      }
    }
    tmp[2] = 0.0; tmp2[2] = 0.0;
    surf_calc_base(s, tmp, dummy, metr);
    if (estNulle(metr[(dir + 1) % 2][(dir + 1) % 2])){
      param_init += param_init - param_fin;
    }
    surf_calc_base(s, tmp2, dummy, metr);
    if (estNulle(metr[(dir + 1) % 2][(dir + 1) % 2])){
      param_fin += param_fin - param_init;
    }
    est_per = _bool[_dir];
  }
  else {
    fonc = create_fonction_courbe_surf_param(s, c_plan);
    param_init = c_plan->p_init;
    param_fin = c_plan->p_fin;
    init_p = c_plan->in;
    est_per = c_plan->est_periodique;
  }
  return create_courbeP(fonc, param_init,
			param_fin, est_per, init_p, sgn,dummy);
}

/*
 * Additionne le paramètre v2 au paramètre v1 (retour dans v1)
 */
int surf_deplacer(Surf_param *s,
		  P3D v1, P3D v2, int sgn){
  P3D tmp;
  double _sgn = (double)sgn;
  tmp[0] = v1[0] + v2[0] * _sgn;
  tmp[1] = v1[1] + v2[1] * _sgn; tmp[2] = 0.0;
  return surf_set_pos(s, tmp, v1);
}

/*
 * utilisé en callback pour la méthode Newton
 */
int surf_deplacer_newton(P3D pt, P3D del,
			 void *param, int sgn){
  Surf_param *s = (Surf_param*)param;
  return surf_deplacer(s, pt, del, sgn);
}

/*
 * Appelé par surf_set_pos
 */
int surf_set_pos_sub(Surf_param *s, P3D metr[2],
		     P3D param_pos, P3D res){
  int i;
  int compl, ind = -1;
  double *init = s->init;
  double *fin = s->fin;
  int term, _bool[2];

  _bool[0] = s->ind_quotient % 2;
  _bool[1] = s->ind_quotient / 2;
  for (i = 0; i < 2; i++){
    compl = (i + 1) % 2;
    if (param_pos[i] < init[i] - EPS_TOL){
      if (estPetit(metr[compl][compl])){
	param_pos[i] = 2 * init[i] - param_pos[i];
	if (ind == i)
	  ind = -1;
	else
	  ind = i;
	i--;
      }
      else if (_bool[i]){
	term = 0;
	res[i] = param_pos[i];
	while (!term){
	  res[i] += fin[i] - init[i];
	  term = (res[i] >= init[i]);
	}
      }
      else return 0;
    }
    else if (param_pos[i] > fin[i] + EPS_TOL){
      if (estPetit(metr[compl][compl])){
	param_pos[i] = 2 * fin[i] - param_pos[i];
	if (ind == i)
	  ind = -1;
	else
	  ind = i;
	i--;
      }
      else if (_bool[i]){
	term = 0;
	res[i] = param_pos[i];
	while (!term){
	  res[i] += init[i] - fin[i];
	  term = res[i] <= fin[i];
	}
      }
      else return 0;
    }
    else {
      res[i] = param_pos[i];
    }
  }
  if (ind != -1){
    compl = (ind + 1) % 2;
    res[compl] += (fin[compl] - init[compl]) / 2.0;
    if (res[compl] > fin[compl])
      res[compl] += init[compl] - fin[compl];
  }
  return 1;
}

/*
 * on met la position de res sur la surface à param
 */
int surf_set_pos(Surf_param *s,
		 P3D param, P3D res){
  P3D metr[2], dummy[2];
  P3D tmp;
  P3D *metr2 = NULL;
  param[2] = 0.0;
  if (!surf_est_dans_borne(s, param)){
    surf_proj_borne(s, param, tmp);
    surf_calc_base(s, tmp, dummy, metr);
    metr2 = metr;
  }
  if (!surf_set_pos_sub(s, metr2, param, res))
    return 0;
  res[2] = 0.;
  return 1;
}

int surf_setpt(Surf_param *s, Pt* p1, P3D param){
  if (!surf_set_pos(s, param, p1->param))
    return 0;
  image_cvect(s->f, p1->param, p1->p);
  return 1;
}

int surf_newton(Surf_param *s, P3D init, Newton_f f,
		Newton_f_p f_p, void *param,
		double delta, double del2, P3D res,
		Param_maillage *p_new, int dim){
  Mapping_new depl = surf_deplacer_newton;
  void *param_depl = (void*)s;
  int out = newton(init, f, f_p, param, depl, param_depl,
		   delta, del2, res, p_new, dim);
  if (out == -1){
    p_new->err = PB_CONV;
  } else if (out == -2){
    p_new->err = HORS_BORNE;
    //   PRINT_ERR_LOG(p_new, "sortie des bornes de la surface\n");
  }
  return out;
}

void free_surf(Surf *s){
  if (s->s_param != NULL)
    free_surface_param(s->s_param);
  free(s);
}

void free_surface_param(Surf_param *s){
  free_champ_vect(s->f);
  free(s);
}

/*
 * Effectue la rotation d'un vecteur dans le plan tangent
 */
void surf_rotation_vect(Surf_param *s, Pt *p, P3D dir,
			double the, P3D res){
  P3D tmp;
  P3D base[2];
  surf_get_bon(s, p->param, base);
  surf_projeter_tangent(s, p, dir, tmp);
  rotation_vect(tmp, base, the, res);
}

void surf_get_bon(Surf_param *s, P3D param, P3D res[2]){
  P3D dummy[2];
  surf_calc_base(s, param, res, dummy);
  orthonormer(res);
}

double surf_get_curv(Surf_param *s, P3D param){
  if (s->gauss == NULL){
    surf_init_curv(s);
    if (s->gauss == NULL)
      return 0.;
  }
  return image_cr(s->gauss, param);
}

void surf_init_curv(Surf_param *s){
  if (s->gauss == NULL){
    s->gauss = create_champ_r(fonc_gauss, NULL, s);
  }
}

double fonc_gauss(P3D p, void *param){
  Surf_param *s = (Surf_param*)param;
  Champ_vect *f = s->f;
  double res = 0;
  double C1, C2;
  P3D v[2], der[3], der2[2], _der[3];
  double E, G, Eu, Ev, Gu, Gv, Evv, Guu;
  int i, j;
  image_cvect_p2(f, p, _der);
  for (j = 0; j < 3; j++){
    for (i = 0; i < 3; i++)
      der[j][i] = _der[i][j];
  }
  image_cvect_p3(f, p, _der);
  for (j = 0; j < 2; j++){
    for (i = 0; i < 3; i++)
      der2[j][i] = _der[i][j];
  }
  image_cvect_p(f, p, _der);
  for (j = 0; j < 2; j++){
    for (i = 0; i < 3; i++)
      v[j][i] = _der[i][j];
  }
  E = prod_scal(v[0], v[0]);
  G = prod_scal(v[1], v[1]);
  C1 = E*G;
  Gu = 2.*prod_scal(der[2], v[1]);
  Ev = 2.*prod_scal(der[2], v[0]);
  Gv = 2.*prod_scal(der[1], v[1]);
  Eu = 2.*prod_scal(der[0], v[0]);
  C2 = 2.*prod_scal(der[2], der[2]);
  Evv = 2.*prod_scal(der2[1], v[0]) + C2;
  Guu = 2.*prod_scal(der2[0], v[1]) + C2;

  res = -(Guu + Evv) / (2.*C1);
  res += (Eu*Gu*G + E*Gu*Gu + Ev*Ev*G + E*Ev*Gv) / (4.*C1*C1);
  return res;
}

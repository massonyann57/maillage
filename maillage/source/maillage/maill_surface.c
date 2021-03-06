#include "maill_surface.h"

/*
 * Estimation du point suivant lors du calcul de la courbe
 * (utilis� pour initialiser le Newton)
 * s : la surface
 * prec : le point pr�c�dent (vecteur 2D contenant le param�trage
 * dans le cas d'une surface param�tr�e et vecteur 3D pour une surface
 * d�finie par une contrainte)
 * base : une bon au point pr�c�dent
 * alph : la courbure discr�te de la courbe au point pr�c�dent
 * pourc : param�tre permettant de jouer sur l'initialisation
 * pour �viter un demi-tour
 * res : le r�sultat en sortie implicite
 */
int estim_pt_suiv_courb(Surf_param *s, Param_maillage *param,
			Pt* prec, P3D base[2],
			double alph, double pourc,
			double h, P3D res){
  int i;
  P3D delta;
  for (i = 0; i < 3; i++)
    delta[i] = h*(base[0][i] * cos(alph)
		  + base[1][i] * sin(alph));
  return deplacement_infinitesimal(s, param, 
				   prec, delta, res);
}

/*
 * Estimation du point suivant lors du calcul d'un nouveau ||gramme
 * (utilis� pour initialiser le Newton)
 * s : la surface
 * Xi_, Xi1, Xi2 : les 3 points du futur ||gramme
 *      (peut �tre mis � nulle dans le cas o� init != NULL)
 * prec_param : le point pr�c�dent (vecteur 2D contenant le param�trage
 * dans le cas d'une surface param�tr�e)
 * pourc : param�tre permettant de jouer sur l'initialisation
 * pour �viter un demi-tour
 * res : le r�sultat en sortie implicite
 */
int estim_pt_suiv_surf(Surf_param *s, Param_maillage* param,
		       P3D Xi1, P3D Xi2, Pt *prec,
		       double pourc, P3D res){
  int i;
  P3D delta;
  for (i = 0; i < 3; i++)
    delta[i] = (1 + pourc)*Xi1[i]
      + (1 + pourc)*Xi2[i] - (2 + 2 * pourc)*prec->p[i];
  return deplacement_infinitesimal(s, param, 
				   prec, delta, res);
}

/*
 * Donne les coordonn�es du point suivant � partir
 * des coordonn�es du point pr�c�dent et de la direction
 * � prendre
 *
 * !!!!!! tol est important : va permettre de converger lorsqu'on
 * est pr�s d'une rosace
 */
int deplacement_infinitesimal(Surf_param *s, Param_maillage* param,
			      Pt* prec, P3D delta, P3D res){
  int i;
  P3D del, diff;
  int it = 0, it_max = 100;
  int term = 0;
  double depl, tol_init = 0.01;
  double n = norme(delta);
  Pt tmp;
  double tol_cur;
  double det;
  P3D metr[2], base[2];
  /*    double *init = s->s_param->init;
	double *fin = s->s_param->fin;*/

  ptcopy(&tmp, prec);
  VECT_CPY(diff, delta);
  while (it < it_max && !term){
    if (!surf_calc_base(s, tmp.param, base, metr)){
      return 0;
    }
    det = deter(metr, 2);
    tol_cur = _fmin(det,1./det)*tol_init;
    depl = iter_deplacement_infinitesimal(s, &tmp, diff, res);
    if (depl < 0){
      return 0;
    }
    if (!surf_setpt(s, &tmp, res)){
      return 0;
    }
    VECT_DEL(del, prec->p, tmp.p, i);
    VECT_DEL(diff, del, delta, i);
    term = (_fmin(norme(diff), depl) < tol_cur*n);
    it++;
  }
  /*  if (it == it_max){
      return 0;
      }*/
  return 1;
}

/*
 * Donne les coordonn�es du point suivant � partir
 * des coordonn�es du point pr�c�dent et de la direction
 * � prendre
 * retourne la norme du d�placement
 */
double iter_deplacement_infinitesimal(Surf_param *s, Pt* prec,
				      P3D delta, P3D res){
  P3D base[2];
  double ps[3];
  int ind = -1;
  P3D metr[2];
  int i;
  int bool_non_dege;

  if (!surf_calc_base(s, prec->param, base, metr))
    return -1.0;
  bool_non_dege = coordonne(delta, base, metr, ps, -0.1, 1, 1);
  res[0] = prec->param[0];
  res[1] = prec->param[1];
  res[2] = 0.0;

  if (!bool_non_dege){
    P3D aj;
    aj[0] = 0.0;aj[1]=0.0;aj[2]=0.0;
    if (!estPetit(metr[0][0])){
      aj[0] = ps[0] / sqrt(metr[0][0]);
    } else
      ind = 0;
    /* if (!surf_deplacer(s, res, aj, 1))
       return -1.0;*/
    if (!estPetit(metr[1][1])){
      aj[1] = ps[1] / sqrt(metr[1][1]);
    } else
      ind = 1;
    if (ind >= 0){
      /* on cherche � la main... */
      int N = 10, N2 = 20, j, k;
      int _ind = (ind+1)%2;
      double tol = pow(10.,-7);
      double init = res[_ind] - N*tol;
      double fin = s->fin[_ind];
      P3D cp_res, res_prec, res_max;
      double deb2 = s->init[ind];
      double fin2 = s->fin[ind];
      double del, del2 = (fin2-deb2)/N2;
      P3D delta_cur, diff_delta;
      double ndiff, ndiff_prec, ndiff_min;
      Pt pt_surf_cur;
      int bool_init = 1;
      int bool_inverse = 0;
      int jmax = 0;
      double delMinJ = 0.001;
      double _tmp;
      int termj;
      int initj = 0, finj = N2;

      if(fabs(fin-res[_ind])>fabs(s->init[_ind]-res[_ind])){
	fin = s->init[_ind];
	init = res[_ind]+N*tol;
      }
      del = (fin-init)/N;
      VECT_CPY(cp_res, res);
      i = 0;
      ndiff_prec = INT_MAX;
      ndiff = 0.;
      while(i<N && fabs(del) > tol){
	ndiff_min = INT_MAX;
	cp_res[_ind] = init+i*del;
	termj = 0;
	while(!termj){
	  jmax = initj;
	  for(j = initj;j<finj;j++){
	    cp_res[ind] = deb2+j*del2;
	    if (!surf_setpt(s, &pt_surf_cur, cp_res))
	      return -1.;
	    VECT_DEL(delta_cur, prec->p, 
		     pt_surf_cur.p, k);
	    VECT_DEL(diff_delta, delta, delta_cur, k);
	    ndiff = norme(diff_delta);
	    if(ndiff_min >= ndiff){
	      jmax = j;
	      VECT_CPY(res_max, cp_res);
	      ndiff_min = ndiff;
	    }
	  }
	  if(jmax == initj && deb2>s->init[ind]){
	    if(initj == 0){
	      _tmp = fin2;
	      fin2 = deb2+del2;
	      deb2 += (deb2-_tmp);
	      finj = N2;
	      del2 = (fin2-deb2)/N2;
	    } else{
	      initj -= _max(finj-initj,0);
	      if(initj<0)
		initj = 0;
	    }
	  } else if(jmax == finj-1 && fin2 < s->fin[ind]){
	    if(finj == N2){
	      _tmp = fin2;
	      fin2 = deb2+del2;
	      deb2 += (deb2-_tmp);
	      initj = 0;
	      del2 = (fin2-deb2)/N2;
	    } else {
	      finj = _min(2*finj-initj,N2);
	    }
	  } else if(del2>delMinJ){
	    deb2 = deb2+(jmax-1)*del2;
	    fin2 = deb2+2.*del2;
	    del2 = (fin2-deb2)/N2;
	  } else {
	    if(finj-initj>3){
	      initj = jmax-1;
	      finj = jmax+2;
	    }
	    termj = 1;
	  }
	}
	if(ndiff_min > ndiff_prec){
	  if(i==1 && !bool_inverse){
	    i = 0;
	    fin = _fmax(_fmin(2.*init-fin, s->fin[_ind]),
			s->init[_ind]);
	    del = (fin-init)/N;
	    bool_inverse=1;
	  } else {
	    bool_init = 0;
	    if(ndiff_prec>tol){
	      bool_inverse = 0;
	      init = _fmax(_fmin(init+(i-1)*del, s->fin[_ind]),
			   s->init[_ind]);
	      fin = _fmax(_fmin(init+del, s->fin[_ind]),
			  s->init[_ind]);
	      i = 0;
	      del = (fin-init)/N;
	    } else {
	      //	      ndepl = distance(res, res_prec);
	      VECT_CPY(res, res_prec);
	      return 0.;
	    }
	  }
	} else {
	  if(i==1 && bool_init){
	    i = 0;
	    init = _fmax(_fmin(init+del, s->fin[_ind]),
			 s->init[_ind]);
	    fin = _fmax(_fmin(3.*fin-2.*init, s->fin[_ind]),
			s->init[_ind]);
	    del = (fin-init)/N;
	    bool_inverse = 1;
	  } else if(i>1)
	    bool_init = 0;
	  ndiff_prec = ndiff_min;
	  VECT_CPY(res_prec, res_max);
	  i++;
	}
      }
      VECT_CPY(res, res_prec);
      return 0.;
    }
    else {
      if (!surf_deplacer(s, res, aj, 1))
	return -1.0;
      return norme(aj);
    }
  }
  else {
    if (!surf_deplacer(s, res, ps, 1))
      return -1.0;
  }
  return norme(ps);
}

/*
 * Calcul le point suivant pour construire un ||gramme
 * 3 �quations : la surface, la distance � 2 points
 * param : l'ensemble des param�tres permettant le calcul
 * Xi_, Xi1, Xi2 : les 3 points du futur ||gramme
 *      (peut �tre mis � nulle dans le cas o� init != NULL)
 */
double point_suivant_surface(P3D Xi1, P3D Xi2, Pt* prec,
			     Param_new1 *param,
			     int nbMax, Pt* res,
			     Param_maillage* param_s, P3D init){
  double X0[3];
  int i;
  double pourc = param_s->pourc_ajout;
  double tol_rebr = param_s->tol_rebr;
  double theta = 0.0;
  P3D v[2];
  P3D tmp;
  P3D del_;
  int dim = 2;
  int ret;
  Surf_param *s = param->s;
  Newton_f fonc = f_newton_param_surf;
  Newton_f_p fonc_p = f_newton_param_surf_p;

  /* if (s->type == 2){
    dim = 3;
    fonc = f_newton_contr_surf;
    fonc_p = f_newton_contr_surf_p;
    }*/

  if (prec != NULL){
    for (i = 0; i < 3; i++){
      v[0][i] = Xi1[i] - prec->p[i];
      v[1][i] = Xi2[i] - prec->p[i];
    }
    theta = prod_scal(v[0], v[1]);
    theta /= pow(param->R1, 2);
    if (param_s->bool_sing > 0 && fabs(theta) >= param_s->tol_angle)
      return theta;
  }
  if (init == NULL){
    if (!estim_pt_suiv_surf(s, param_s, Xi1, Xi2, prec,
			    pourc, X0)){
      param_s->err = HORS_BORNE;
      //    PRINT_WAR_LOG(param_s, "point sortie des bornes de la surface\n");
      return -10;
    }
  }
  else
    VECT_CPY(X0, init);
  ret = surf_newton(s, X0, fonc, fonc_p,
		    (void*)param, 10.*param->R1, 10.*param->R1,
		    tmp, param_s, dim);
  if (ret == -1){
    /* on a un probl�me de convergence */
    return -1;
  } else if(ret == -2){
    //    PRINT_WAR_LOG(param_s, "point suivant surface => sorti des bornes\n");
    /* on est sorti des bornes */
    return -2;
  }
  if (dim == 3){
    VECT_CPY(res->p, tmp);
    if (prec != NULL){
      for (i = 0; i < 3; i++){
	del_[i] = res->p[i] - prec->p[i];
      }
      if (norme(del_) < tol_rebr){
	PRINT_WAR_LOG(param_s, "Rebroussement!!!\n");
	return -10;
      }
    }
  }
  else {
    if (!surf_setpt(s, res, tmp))
      return -2;
  }
  return theta;
}

/*
 * Calcul le point suivant d'une courbe
 * 3 �quations : la surface, la distance et la direction
 * param : l'ensemble des param�tres permettant le calcul
 * Xi : le point courant
 * Xi_, Xi__ : le point pr�c�dent et celui d'avant
 */
int point_suivant_courbeG(Pt *Xi, double* Xi_,
			  Param_new2 *param,
			  int nbMax, Pt* res,
			  Param_maillage* param_s, P3D *metr,
			  double pas_init, double* init){
  double X0[3];
  int i, ret;
  double pourc = param_s->pourc_ajout;
  P3D del_;
  double tol_rebr = param_s->tol_rebr;
  int dim = 2;
  Newton_f fonc = f_newton_param_courbe;
  Newton_f_p fonc_p = f_newton_param_courbe_p;
  Surf_param *s = param->s;
  double coeff_metr = 1.0;
  P3D tmp;

  if (pas_init <= 0.0)
    pas_init = param->R;

  /*  if (s->type == 2){
    dim = 3;
    fonc = f_newton_contr_courbe;
    fonc_p = f_newton_contr_courbe_p;
    }*/

  if (init == NULL){
    if (metr != NULL){
      if (estNulle(metr[0][0]))
	coeff_metr = metr[1][1];
      else if (estNulle(metr[1][1]))
	coeff_metr = metr[0][0];
      else
	coeff_metr = sqrt(_fmin(metr[0][0], metr[1][1]));
    }
    if (!estim_pt_suiv_courb(s, param_s, Xi,
			     param->base, param->the,
			     pourc, pas_init, X0))
      return -2;
  }
  else
    VECT_CPY(X0, init);
  ret = surf_newton(s, X0, fonc, fonc_p,
		    (void*)param, 20.*param->R / coeff_metr,
		    20.*param->R / coeff_metr,
		    tmp, param_s, dim);
  if (ret != 1)
    return ret;
  if (dim == 3){
    VECT_CPY(res->p, tmp);
    if (Xi_ != NULL){
      VECT_DEL(del_, res->p, Xi_, i);
      if (norme(del_) < tol_rebr)
	return 0;
    }
  }
  else{
    surf_setpt(s, res, tmp);
    if (!surf_est_dans_borne(s, res->param))
      return -2;
  }
  return 1;
}

/*
 * Calcul le point suivant d'une courbe
 * 3 �quations : la surface, la distance et la direction
 * param : l'ensemble des param�tres permettant le calcul
 * Xi : le point courant
 * Xi_, Xi__ : le point pr�c�dent et celui d'avant
 */
int point_suivant_courbeP(Surf_param *s, double init_p,
			  double dist_courb,
			  Param_new3 *param,
			  Param_maillage* param_s,
			  double *res_param_courbe){
  double X0[3];
  int i, ret;
  P3D tmp;
  CourbeP *c_surf = param->c;
  double m_courbe;
  double del, dist, h = param->R;
  double tol = h / param_s->tol_new_courbeP;
  int nb_etape = param_s->nb_etape_new_courbeP;
  double tol_del = param_s->tol_del_new_courbeP;
  int sgn = c_surf->dir;
  if (dist_courb <= 0)
    dist_courb = param->R;

  m_courbe = courbe_get_dir(c_surf, init_p, tmp);
  if (estNulle(m_courbe))
    m_courbe = 1.0;
  X0[0] = init_p;
  if (!courbe_deplacer(c_surf, X0, sgn*h / m_courbe)){
    return -2;
  }
  image_fvect(c_surf->param, X0[0], tmp);
  dist = distance(tmp, param->pos);
  if (fabs(dist - h) > tol){
    X0[0] = init_p;
    if (!courbe_deplacer(c_surf, X0, sgn*h / (2.0*m_courbe))){
      return -2;
    }
    for (i = 0; i < nb_etape; i++){
      m_courbe = courbe_get_dir(c_surf, X0[0], tmp);
      image_fvect(c_surf->param, X0[0], tmp);
      if (estNulle(m_courbe))
	m_courbe = 1.0;
      dist = distance(tmp, param->pos);
      del = sgn*(h - dist) / (2.0*m_courbe);
      if (del < tol_del){
	if (!courbe_deplacer(c_surf, X0, del)){
	  return -2;
	}
      }
      else{
	if (!courbe_deplacer(c_surf, X0, tol_del)){
	  return -2;
	}
      }
    }
    if (!courbe_deplacer(c_surf, X0, sgn*(h - dist) / (2.0*m_courbe))){
      return 0;
    }
  }
  ret = newton(X0, f_newton_param_ligne,
	      f_newton_param_ligne_p,
	      (void*)param, NULL, NULL, 10.*h / sqrt(m_courbe),
	      10.*h / sqrt(m_courbe),
	       tmp, param_s, 1);
  if (ret != 1){
    return ret;
  }
  if (res_param_courbe != NULL)
    res_param_courbe[0] = tmp[0];
  return 1;
}
/*
 * Calcul le point suivant d'une courbe
 * 3 �quations : la surface, la distance et la direction
 * param : l'ensemble des param�tres permettant le calcul
 * Xi : le point courant
 * Xi_, Xi__ : le point pr�c�dent et celui d'avant
 */

/*int point_suivant_dual_primal(Surf_param *s, double init_p,
  Param_new3 *param,
  Param_maillage* param_s,
  double *res_param_courbe){
  double X0[3];
  int i;
  P3D tmp;
  CourbeP *c_surf = param->c;
  double m_courbe;
  double del, dist, h = param->R;
  double tol = h/param_s->tol_new_courbeP;
  int nb_etape = param_s->nb_etape_new_courbeP;
  double tol_del = param_s->tol_del_new_courbeP;
  int sgn = 1-2*(c_surf->dir/2);

  m_courbe = courbe_get_dir(c_surf, init_p, tmp);
  if(estNulle(m_courbe))
  m_courbe = 1.0;
  X0[0] = init_p;
  if (!courbe_deplacer(c_surf, X0, sgn*h / m_courbe)){
  PRINT_WAR_LOG(param_s, "SORTIE0  est periodique : %d\nBorne courbe : [%lf;%lf]\n",
  c_surf->est_periodique, c_surf->p_init, c_surf->p_fin);
  return 0;
  }
  image_fvect(c_surf->param, X0[0], tmp);
  dist = distance(tmp, param->pos);
  if(fabs(dist-h) > tol){
  X0[0] = init_p;
  if (!courbe_deplacer(c_surf, X0, sgn*h / (2.0*m_courbe))){
  return 0;
  }
  for(i = 0;i<nb_etape;i++){
  m_courbe = courbe_get_dir(c_surf, X0[0], tmp);
  image_fvect(c_surf->param, X0[0], tmp);
  if(estNulle(m_courbe))
  m_courbe = 1.0;
  dist = distance(tmp, param->pos);
  del = sgn*(h-dist)/(2.0*m_courbe);
  if(del < tol_del){
  if (!courbe_deplacer(c_surf, X0, del)){
  return 0;
  }
  }
  else{
  if (!courbe_deplacer(c_surf, X0, tol_del)){
  return 0;
  }
  }
  }
  if (!courbe_deplacer(c_surf, X0, sgn*(h - dist) / (2.0*m_courbe))){
  return 0;
  }
  }

  if (!newton(X0, f_newton_param_ligne,
  f_newton_param_ligne_p,
  (void*)param, NULL, NULL, 10.*h / sqrt(m_courbe),
  10.*h / sqrt(m_courbe),
  tmp, param_s, 1)){
  return 0;
  }
  if(res_param_courbe != NULL)
  res_param_courbe[0] = tmp[0];
  return 1;
  }*/

/*
 * Calcul l'intersection entre 2 plans et une surface
 * Xi1 : le point par lequel passe le plan 1
 * par1 : les param�tres du point par lequel passe le plan 1
 * Xi2 : le point par lequel passe le plan 2
 * par2 : les param�tres du point par lequel passe le plan 2
 * v1 : la normale au plan 1
 * v2 : la normale au plan 2
 * base : une bon dans lequel on peut �crire le syst�me
 * param : l'ensemble des param�tres permettant le calcul.
 *   La normale et la surface doivent �tre entr�s,
 *   le reste est actualis�
 * nbMax : nombre d'it�ration maximal
 * res : retour implicite : les coordonn�es du point
 * par_res : retour implicite : les param�tres du point
 * param_s : param�tres du calcul
 */
int inters_double_plan_surface(Pt* Xi1, Pt* Xi2,
			       P3D v1, P3D v2, P3D *base,
			       Param_new5 *param,
			       int nbMax, Pt* res,
			       Param_maillage* param_s){
  double X0[3];
  int i;

  Newton_f fonc = f_newton_param_inter_plan;
  Newton_f_p fonc_p = f_newton_param_inter_plan_p;
  int dim = 2;

  Surf_param *s = param->s;
  P3D del;
  double alpha;
  P3D loc_del;
  P3D loc_v[3];
  P3D mat_inv[3];
  P3D delta;

  VECT_CPY(param->pos1, Xi1);
  VECT_CPY(param->pos2, Xi2);
  VECT_DEL(del, Xi1->p, Xi2->p, i);

  coordonne_bon(del, base, loc_del);
  loc_v[0][1] = -prod_scal(base[1], v2);
  loc_v[0][0] = -prod_scal(base[1], v1);
  loc_v[1][1] = prod_scal(base[0], v2);
  loc_v[1][0] = prod_scal(base[0], v1);
  inverse(loc_v, 2, mat_inv);

  alpha = mat_inv[0][0] * loc_del[0] + mat_inv[0][1] * loc_del[1];
  for (i = 0; i < 3; i++)
    delta[i] = alpha*(base[0][i] * loc_v[0][0]
		      + base[1][i] * loc_v[1][0]);
  if (!deplacement_infinitesimal(s, param_s, Xi1, 
				 delta, X0))
    return 0;
  /* if (s->type == 2){
    dim = 3;
    fonc = f_newton_contr_inter_plan;
    fonc_p = f_newton_contr_inter_plan_p;
    }*/
  if (surf_newton(s, X0, fonc, fonc_p,
		  (void*)param, 2 * param_s->pas_max_new,
		  10.*param_s->pas_max_new,
		  res->p, param_s, dim) != 1)
    return 0;
  if (dim == 2){
    res->param[0] = res->p[0];
    res->param[1] = res->p[1];
    image_cvect(s->f, res->param, res->p);
  }
  return 1;
}

/* calcul une des courbes initiales
 * s : la surface
 * dir_init2 : 2�me direction initiale (peut-�tre � revoir)
 * p_maill : les param�tres du calcul
 * res : les points calcul�s
 */
int calc_courbeG(CourbeG *cg, Surf_param *s,
		 double *h, Param_maillage *p_maill,
		 Pt *res, int _nb){
  int j;
  Param_new2 *param;
  P3D dir1;
  double *tmp = NULL, taille_subd;
  int boo, term;
  int nbMax = p_maill->nbMax_new;
  double *the = cg->courbure;
  P3D vect_tmp, del1, del2;
  Pt ptcalc[10][10];
  Pt *ptres;
  PPt ptcur[10][2];
  double Rsubd;
  int haut_max = 4;
  int nbsubd = 5, it[10];
  int nb[10], haut = 0;

  _nb = _min(cg->size, _nb);
  memset(nb, 0, 10*sizeof(int));
  memset(it, 0, 10*sizeof(double));
  nb[0] = _nb;
  it[0] = 1;

  image_cvect(s->f, cg->init->param, cg->init->p);
  surf_projeter_tangent(s, cg->init, cg->dir_init, vect_tmp);
  VECT_CPY(cg->dir_init, vect_tmp);

  param = create_param_new2(cg->init->p, NULL, cg->init->p,
			    h[1], the[0], s);
  ptcopy(res, cg->init);
  ptcur[0][1] = res;
  tmp = NULL;
  /* on a un arbre sur les subdivisions et on parcours l'arbre :
   *   - haut : la hauteur 
   *   - it : la position � chaque niveau de l'arbre
   *   - nb : le nombre de noeuds � atteindre � chaque hauteur
   * lorsqu'on ne convergence pas, on descend dans l'arbre 
   * et on met nb � nbsubd
   */ 
  while(haut >= 0){
    taille_subd = ((double)pow(nbsubd, haut));
    Rsubd = h[it[0]-1]/taille_subd;
    if(it[haut]>1){
      VECT_DEL(dir1, ptcur[haut][0]->p, ptcur[haut][1]->p, j);
    } else {
      VECT_CPY(dir1, cg->dir_init);
    }
    param->R = Rsubd;
    if(it[0]>1)
      param->the = the[it[0]-1]/taille_subd;
    else if(haut == 0)
      param->the = the[0];
    else
      param->the = 0.;
    if (!surf_calc_bon_dir(s, ptcur[haut][1], 
			   dir1, param->base, 1)){
      PRINT_ERR_LOG(p_maill, "erreur calcul base\n");
      return it[0];
    }
    VECT_CPY(param->pos, ptcur[haut][1]->p);
    VECT_CPY(param->centre, ptcur[haut][1]->p);
    if(haut == 0)
      ptres = &res[it[0]];
    else
      ptres = &ptcalc[haut][it[haut]];
    boo = point_suivant_courbeG(ptcur[haut][1], tmp, param,
				nbMax, ptres,
				p_maill, NULL, 0.0, NULL);
    term = 0;
    if (boo == -2){
      //      PRINT_WAR_LOG(p_maill, "sorti des bornes %d\n", i);
      return it[0];
    } 
    if(boo == 1){
      /* TODO : on regarde si on n'a pas fait demi-tour */
      if(it[haut] > 1){
	VECT_DEL(del1, ptcur[haut][0]->p, 
		 ptcur[haut][1]->p, j);
	VECT_DEL(del2, ptcur[haut][1]->p, 
		 ptres->p, j);
	if(prod_scal(del1, del2) > 0)
	  term = 1;
	else term = 0; /* on a fait un demi tour */
      } else term = 1;
    }
    if(!term){
      haut++;
      if(haut >= haut_max){
	/* on atteint la taille maximal */
	return it[0];
      }
      nb[haut] = nbsubd+2;
      it[haut] = 2;
      if(it[haut-1]>1)
	ptcur[haut][0] = ptcur[haut-1][0];
      ptcur[haut][1] = ptcur[haut-1][1];
    } else {
      it[haut]++;
      if(it[haut] == nb[haut]){
	if(haut > 0){
	  if(haut == 1)
	    ptres = &res[it[0]];
	  else
	    ptres = &ptcalc[haut-1][it[haut-1]];
	  ptcopy(ptres, ptcur[haut][1]);
	  ptcur[haut-1][0] = ptcur[haut-1][1];
	  ptcur[haut-1][1] = ptcur[haut][1];
	}
	if(haut != 0){
	  ptcur[haut][0] = NULL;
	  ptcur[haut][1] = NULL;
	  nb[haut] = 0;
	  it[haut] = 0;
	}
	haut--; 
      } else {
	ptcur[haut][0] = ptcur[haut][1];
	ptcur[haut][1] = ptres;
      }
    }
  }
  return it[0];
}

/*
 * calcul la discr�tisation d'une courbe
 * La courbe c doit �tre � valeur dans le plan
 * nbptr : la nombre de points
 * init : le param�trage du 1er point
 * h : la distance entre chaque points
 * dir : pair -> ligne x et impair -> ligne y
 dir/2 == 0 -> demi-ligne des valeurs positives
 dir/2 == 1 -> demi-ligne des valeurs negatives
 * s : la surface
 * p_maill : les param�tres du calcul
 * res : les points calcul�s
 * dern_param : un retour implicite : le param�tre du dernier point
 */
int calc_courbeP_surf(int nbptr,
		      CourbeP *c_plan, CourbeP *c_surf,
		      double *h, Surf_param *s,
		      Param_maillage *p_maill,
		      Pt *res, double *dern_param,
		      double tol_gross){
  int i;
  P3D res_new;
  Param_new3* param;
  double init_p;
  double tmp;
  CourbeP *c_surf2 = c_surf;
  double dist_prec = tol_gross + 1.0;
  double dist;
  int bool_stop = tol_gross > 0;
  int _init = 1;
  Pt init;
  if (c_plan->param != NULL){
    image_fvect(c_plan->param, c_plan->in, res_new);
    if (!surf_setpt(s, &init, res_new)){
      PRINT_ERR_LOG(p_maill, "err pt_init\n");
      return 0;
    }
  }
  else {
    surf_setpt(s, &init, c_plan->param_init);
  }
  ptcopy(res, &init);
  if (c_surf == NULL)
    c_surf2 = surf_get_courbe_from_courbeP(s, c_plan);

  param = create_param_new3(c_surf2, h[0], init.p);
  if (c_plan->param != NULL)
    init_p = c_plan->in;
  else
    init_p = init.param[c_plan->dir % 2];

  for (i = 1; i < nbptr; i++){
    param->R = h[i - 1];
    VECT_CPY(param->pos, res[i - 1].p);
    if (point_suivant_courbeP(s, init_p, 0.0, 
			      param, p_maill, &tmp) != 1){
      PRINT_WAR_LOG(p_maill, "point %d : (%lf;%lf) sortie des bornes de la surface\n", i, res_new[0], res_new[1]);
      return i;
    }
    courbeP_get_image(c_plan, tmp, res_new);
    if (!surf_setpt(s, &res[i], res_new)){
      PRINT_WAR_LOG(p_maill, "point %d : (%lf;%lf) sortie des bornes de la surface\n", i, res_new[0], res_new[1]);
      return i + 1;
    }

    init_p = tmp;
    dist = distance(res[0].p, res[i].p);
    if (bool_stop){
      if (dist < tol_gross && !_init){
	if (dist_prec < dist){
	  PRINT_WAR_LOG(p_maill, "courbe ferm�e...\n");
	  return i;
	}
	else
	  dist_prec = dist;
      }
      else if (dist > tol_gross && _init)
	_init = 0;
      else if (dist_prec < tol_gross)
	return i;
    }
    if (courbe_position_borne(c_surf2, tmp) != 0){
      if (dern_param != NULL)
	dern_param[0] = tmp;
      PRINT_WAR_LOG(p_maill, "point sorti des bornes de la courbe\n");
      return i + 1;
    }
  }
  if (dern_param != NULL)
    dern_param[0] = tmp;
  if (c_surf == NULL){
    free_fonc_vect(c_surf2->param);
    free_courbeP(c_surf2);
  }
  return i;
}

int calc_courbePts(Surf_param *s, CourbePts *c, Param_maillage *param,
		   double *h, int nb, Pt *res){
  int i;
  int ind = 0;
  int ret = 0;
  P3D dummy[2];
  Param_new2 *p_new = create_param_new2(dummy[0], dummy, dummy[0],
					0.0, 0.0, s);
  ptcopy(res, c->pts);
  for (i = 1; i < nb; i++){
    ret = calc_nouv_pt_courbe(s, c->nb - ind, res+i-1, 
			      NULL, c->pts+ind,
			      h[i-1], h[i-1], p_new, 
			      res+i, param, 0);
    if (ret < -1)
      return i;
    else
      ind += ret + 1;
  }
  return nb;
}

/*
 * Meme fonction que pr�c�demment mais referme la courbe
 * La courbe c doit �tre � valeur dans le plan
 * nbptr : la nombre de points
 * init : le param�trage du 1er point
 * h : la distance entre chaque points
 * dir : pair -> ligne x et impair -> ligne y
 dir/2 == 0 -> demi-ligne des valeurs positives
 dir/2 == 1 -> demi-ligne des valeurs negatives
 * s : la surface
 * p_maill : les param�tres du calcul
 * pts : les points calcul�s
 * par : les param�trages des points calcul�s
 * dern_param : un retour implicite : le param�tre du dernier point
 */
int calc_courbeP_ferme(int nbptr,
		       CourbeP *c_plan, CourbeP *c_surf,
		       double *h, Surf_param *s,
		       Param_maillage *p_maill,
		       Pt *res, double *dern_param,
		       double tol){
  int est_term = 0;
  double del = 0.0;
  double del2 = 0.0;
  int nb, i;
  double tol_gross = 3 * h[0];
  double dist, dist2;
  int succ = 1;
  int ind = 0;
  if (tol == -1)
    tol = p_maill->tol_fermeture_courbe;

  while (!est_term){
    nb = calc_courbeP_surf(nbptr, c_plan, c_surf,
			   h, s, p_maill, res, dern_param, tol_gross);
    del = distance(res[0].p, res[nb - 1].p);
    del2 = distance(res[0].p, res[nb - 2].p);
    dist = del - h[nb - 2];
    dist2 = del2 - h[nb - 2];
    if (fabs(dist2) < fabs(dist)){
      nb--;
      dist = dist2;
      del = del2;
    }
    est_term = (fabs(dist) < tol);
    if (est_term)
      break;
    if (dist > p_maill->courbeFerme_dist_max*h[0]
	|| nb < p_maill->courbeFerme_nbMin){
      est_term = 1;
      succ = -1;
    }
    else if (ind > p_maill->courbeFerme_nbMax){
      est_term = 1;
      succ = -1;
    }
    else{
      for (i = 0; i < 2 * nb; i++){
	h[i] += dist / ((double)nb);
      }
      ind++;
    }
  }
  return succ*nb;
}

/* Cr�� un maillage � partir d'une singularit�.
 * On cherche le maillage contenant 2 courbes donn�es
 * (pas forcement otho.) et dont l'angle est pi/2
 * nbptr : la nombre de points
 * init : le param�trage du 1er point
 * h : la distance entre chaque points
 * dir : les directions initiales des 2 courbes du maillage
 * s : la surface
 * the1 : la courbure discrete de la courbe 1
 * the2 : la courbure discr�te de la coube 2
 * p_maill : les param�tres du calcul
 */
Maillage*
calc_maillage_init_sing(int nbptr, Pt* init,
			double h, P3D dir[2],
			Surf_param *s, double *the1,
			double *the2,
			Param_maillage *p_maill){
  P3D base1[2], base2[2];
  P3D X0, tmp;
  int n1, n2;
  Param_new2* param_tmp1;
  Param_new2* param_tmp2;
  Param_new5* param_tmp3;
  Param_new4* param;
  Maillage *res;
  Pt pts[SIZE_TAB];
  PPt _pts[SIZE_TAB];
  P3D v1, v2;
  Pt* new_pt = createPtN(), *pt1 = createPtN(), *pt2 = createPtN();
  int i;
  ListeD *linit;
  double ps = prod_scal(dir[0], dir[1]);
  double H[SIZE_TAB];
  CourbeG *cg, *cg2;
  int ind_constr = 0;

  for (i = 0; i < nbptr; i++)
    H[i] = h;

  surf_get_bon(s, init->param, base1);
  surf_get_bon(s, init->param, base1);
  //surf_get_base_opt(s->s_param, init->param, grad, metr);
  //orthonormer_base(grad, metr, dir[0], base1);
  calc_base_dir(base1, NULL, dir[1], base2);

  param_tmp1 = create_param_new2(init->p, base1,
				 init->p, h, 0, s);
  param_tmp2 = create_param_new2(init->p, base2,
				 init->p, h, 0, s);
  param_tmp3 = create_param_new5(init->p, init->p,
				 dir[0], dir[1], s);

  param = create_param_new4(init, dir, h, h, s,
			    param_tmp1, param_tmp2,
			    param_tmp3, p_maill, pt1, pt2, new_pt);
  X0[0] = h / tan(acos(ps) / 2);
  X0[1] = h / tan(acos(ps) / 2);
  if (surf_newton(s, X0, f_newton_param_courbe_sing,
		  f_newton_param_courbe_sing_p,
		  (void*)param, 10.*h, 10.*h,
		  tmp, p_maill, 2) != 1)
    return NULL;

  for (i = 0; i < 3; i++){
    v1[i] = pt1->p[i] - init->p[i];
    v2[i] = pt2->p[i] - init->p[i];
  }
  res = createMaillage(new_pt, h, s);
  ajoute_pt(res, res->lignes->l[0], 1, -1,
	    createPtmaill(pt1,ind_constr), p_maill);
  ajoute_pt(res, res->lignes->l[0], 2, -1,
	    createPtmaill(pt2,ind_constr), p_maill);

  cg = create_courbeG(pt1, v1, H, nbptr, the1 + 1);
  n1 = calc_courbeG(cg, s, NULL,
		    p_maill, pts, cg->size);
  linit = liste_getVois(res->lignes->l[0], 1);
  for (i = 0; i < n1; i++){
    _pts[i] = createPtN();
    ptcopy(_pts[i], pts + i);
  }
  ajoute_courbe_maill(res, _pts, n1, 1,
		      linit, 0, -1, p_maill->tol_pas, p_maill,1);

  cg2 = create_courbeG(pt2, v2, H, nbptr, the2 + 1);
  n2 = calc_courbeG(cg2, s, NULL,
		    p_maill, pts, cg2->size);
  for (i = 0; i < n2; i++){
    _pts[i] = createPtN();
    ptcopy(_pts[i], pts + i);
  }
  linit = liste_getVois(res->lignes->l[0], 2);
  ajoute_courbe_maill(res, _pts, n2, 1,
		      linit, 3, -1, p_maill->tol_pas, p_maill,1);
  maillage_che(res, p_maill, 0, NULL, 0, ind_constr);
  return res;
}

/*
 * On ajoute un nouveau ||gramme gr�ce � une de ses diagonales
 * m :le maillage
 * s : la surface
 * np : le nouveau point (non voisin du pr�c�dent : lprec)
 * lnp : cas o� le point existe d�ja
 * param_np : param�trage du point np
 * param_s : les param�tres du calcul
 * lprec : le point pr�c�dent
 * or : l'orientation pour l'ajout du prochain point
 *   (diff�rent de np)
 */
int ajouter_parall_diagonal(Maillage *m, Pt* np, ListeD *lnp,
			    Param_maillage* param_s,
			    ListeD *lprec, int or, int or_vir,
			    int or_voistonew,
			    int or_base, int type_maill, int ind_constr){

  Param_new1 *param_new;
  P3D init, base[2];
  P3D dir;
  Pt *p1 = getvect(lprec);
  double c, d;
  int i;
  Pt* res1 = createPtN(), *res2 = createPtN();
  ListeD* prec = lprec;
  P3D X0;
  P3D del;
  int bool1 = (type_maill == 1 || type_maill == 3);
  int bool2 = (type_maill == 2 || type_maill == 3);
  int ind_seg1, ind_seg2;
  Ptmaill *ptm;
  int _del = 3 + 2 * or_vir;
  int ind_tmp;
  Quadr *tmp;
  ListeD *prec_tmp;
  Segment *seg1, *seg2;
  int bool_inter = 0;

  param_new = create_param_new1(p1->p, np->p,
				m->h, m->h, m->s, param_s);
  VECT_DEL(dir, p1->p, np->p, i);
  d = norme(dir);
  c = sqrt(m->h*m->h - d*d / 4.0);
  if (!surf_calc_bon_dir(m->s, p1, dir, base, or_base)){
    PRINT_ERR_LOG(param_s, "erreur calcul base\n");
    free(param_new);
    return 0;
  }

  for (i = 0; i < 3; i++)
    del[i] = (np->p[i] - p1->p[i]) / 2.0 + c*base[1][i];
  if (!deplacement_infinitesimal(m->s, param_s, p1, del, X0)){
    free(param_new);
    return -10;
  }
  if (point_suivant_surface(p1->p, np->p, NULL, param_new,
			    param_s->nbMax_new, res1,
			    param_s, X0) <= -10){
    free(param_new);
    return -10;
  }
  for (i = 0; i < 3; i++)
    init[i] = (np->p[i] - p1->p[i]) / 2.0 - c*base[1][i];
  if (!deplacement_infinitesimal(m->s, param_s, p1,
				 init, X0)){
    free(param_new);
    return -10;
  }
  if (point_suivant_surface(p1->p, np->p, NULL, param_new,
			    param_s->nbMax_new, res2,
			    param_s, X0) <= -10){
    free(param_new);
    return -10;
  }
  if (prec->vois != NULL){
    if (or == 0)
      prec_tmp = NULL;
    else
      prec_tmp = liste_getVois(prec, or - 1);
  }
  else
    prec_tmp = liste_getVois(prec, or + _del + 2);
  if (prec_tmp != NULL){
    Pt *pt_prec_tmp = getvect(prec_tmp);
    double min_d = distance(pt_prec_tmp->p, res1->p);
    double d_tmp = distance(pt_prec_tmp->p, res2->p);
    double d_tmp2, del_dist = fabs(min_d-d_tmp);
    //	PRINT_WAR_LOG(param_s, "prec_tmp != NULL     min_d : %lf    d_tmp : %lf\n", min_d, d_tmp);
    if (min_d > d_tmp){
      //		PRINT_WAR_LOG(param_s, "inter1 : 1\n");
      bool_inter = 1;
      min_d = d_tmp;
    }
    if (prec->vois == NULL)
      prec_tmp = liste_getVois(prec, or + 2);
    else
      prec_tmp = liste_getVois(prec, or - 2);
    pt_prec_tmp = getvect(prec_tmp);
    d_tmp = distance(pt_prec_tmp->p, res1->p);
    d_tmp2 = distance(pt_prec_tmp->p, res2->p);
    if(bool_inter && d_tmp < d_tmp2)
      bool_inter = 1;
    else if(!bool_inter && d_tmp2 < d_tmp)
      bool_inter = 0;
    else if(fabs(d_tmp - d_tmp2) > del_dist)
      bool_inter = !bool_inter;
    //	PRINT_WAR_LOG(param_s, "inter2 : %d\n", bool_inter);
  }
  if (bool_inter){
    Pt *copy;
    copy = res1;
    res1 = res2;
    res2 = copy;
  }

  ajoute_pt(m, prec, or, or_voistonew, createPtmaill(res1, ind_constr), param_s);
  prec = liste_getVois(prec, or);

  if (lprec->vois != NULL){
    or = (or_voistonew + 2) % 4;
  }
  if (lnp == NULL){
    ajoute_pt(m, prec, (or + _del) % 4, -1,
	      createPtmaill(np, ind_constr), param_s);
  }
  else
    ajoute_pt_exis(m, prec, (or + _del) % 4,
		   get_elementL2D(lnp), lnp, param_s);

  ajouterPoint_comp(m, param_s, res2, ind_constr, 
		    prec, 0,
		    (or + 3 - or_vir) % 4, 0);
  ind_tmp = m->quadr->nb_part - 1;
  tmp = get_elementL2D(m->quadr->l[ind_tmp]);
  tmp->est_diag = 1;
  if (!bool1){
    ptm = get_elementL2D(prec);
    ind_seg1 = ptmaill_get_ind_seg(ptm, or + 2);
    ind_seg2 = ptmaill_get_ind_seg(ptm, or + 1);
    seg1 = get_element_gtab(m->seg, ind_seg1);
    seg2 = get_element_gtab(m->seg, ind_seg2);
    seg1->b_new = 0;
    seg2->b_new = 0;
  }
  if (!bool2){
    prec = liste_getVois(lprec, or + _del);
    ptm = get_elementL2D(prec);
    ind_seg1 = ptmaill_get_ind_seg(ptm, or + 2);
    ind_seg2 = ptmaill_get_ind_seg(ptm, or + 1);
    seg1 = get_element_gtab(m->seg, ind_seg1);
    seg2 = get_element_gtab(m->seg, ind_seg2);
    seg1->b_new = 0;
    seg2->b_new = 0;
  }
  free(param_new);
  return 1;
}

/*
 * Maille une surface en d�marrant avec une bifurcation.
 * Le maillage contient dans son dual 2 courbes donn�es.
 * TODO : peut-etre revoir tout �a (notamment calc_courbe_sing).
 *    Celle-ci ressemble beaucoup � un ajout de diagonal
 *     => la ref�re � partir de l'ajout de diagonal
 * nbptr : la nombre de points
 * init, param_init : le 1er point
 * h : la distance entre chaque points
 * dir : les directions initiales des 2 courbes du dual
 * phi : les angles autour de la 1ere courbe du dual
 * psi2 : le premier angle autour de la 2eme diagonale
 *     (les autres n'�tant pas des param�tres)
 * s : la surface
 * the1 : la courbure discrete de la courbe 1
 * the2 : la courbure discr�te de la coube 2
 * p_maill : les param�tres du calcul
 * or : l'orientation pour l'ajout
 */
Maillage* calc_maillage_init_sing2(int nbptr, Pt* init,
				   double h, P3D dir[2],
				   double *psi, double psi2,
				   Surf *s, double *the1,
				   double *the2,
				   Param_maillage *p_maill, int or){
  P3D base1[2], grad[2], base2[2];
  P3D dir_tmp[2];
  P3D metr[2];
  P3D X0, tmp;
  Param_new2* param_tmp1;
  Param_new2* param_tmp2;
  Param_new5* param_tmp3;
  Param_new4* param;
  Maillage *res;
  Pt* pt1, *pt2;
  Pt _pt[2];
  P3D tmp_par[2];
  double _h[2];
  int i, j;
  ListeD *linit;
  double ps = prod_scal(dir[0], dir[1]);
  int ind_seg;
  double h_c[SIZE_TAB];
  double the;
  Pt tmp_inter;
  int or_vir_diag1 = 0;
  int ind_constr = 0;

  for (i = 0; i < nbptr; i++)
    h_c[i] = 2 * h*cos((PI - psi[i]) / 2.0);

  _h[0] = h*cos(psi[0] / 2.0); _h[1] = h*cos(psi2 / 2.0);

  surf_calc_base_opt(s->s_param, init->param, grad, metr);
  if (!estNulle(deter(metr, 2))){
    calc_base_dir(grad, metr, dir[0], base1);
    calc_base_dir(grad, metr, dir[1], base2);
  }
  else {
    calc_base_dir(dir, NULL, dir[0], base1);
    calc_base_dir(dir, NULL, dir[1], base2);
  }
  param_tmp1 = create_param_new2(init->p, base1, init->p,
				 _h[0], 0, s->s_param);
  param_tmp2 = create_param_new2(init->p, base2, init->p,
				 _h[1], 0, s->s_param);
  param_tmp3 = create_param_new5(init->p, init->p,
				 dir[0], dir[1], s->s_param);

  param = create_param_new4(init, dir, _h[0], _h[1], s->s_param,
			    param_tmp1, param_tmp2,
			    param_tmp3, p_maill, _pt,
			    _pt + 1, &tmp_inter);

  the = acos(ps);
  X0[0] = _h[1] / sin(the) + cos(the)*_h[0] / sin(the);
  X0[1] = _h[0] / sin(the) + cos(the)*_h[1] / sin(the);

  if (!surf_newton(s->s_param, X0, f_newton_param_courbe_sing,
		   f_newton_param_courbe_sing_p,
		   (void*)param, h, h,
		   tmp, p_maill, 2))
    return NULL;

  for (j = 0; j < 2; j++){
    pt1 = createPtN();
    pt2 = createPtN();
    surf_calc_base_opt(s->s_param, _pt[j].param, grad, metr);
    for (i = 0; i < 3; i++){
      dir_tmp[0][i] = _pt[j].p[i] - init->p[i];
      dir_tmp[1][i] = -dir_tmp[0][i];
    }
    calc_base_dir(grad, metr, dir_tmp[0], param_tmp1->base);
    calc_base_dir(grad, metr, dir_tmp[1], param_tmp2->base);
    if (j == 0)
      param_tmp1->R = h_c[0] / 2.0;
    else
      param_tmp1->R = h*cos((PI - psi2) / 2.0);

    param_tmp2->R = param_tmp1->R;
    VECT_CPY(param_tmp1->pos, _pt[j].p);
    VECT_CPY(param_tmp1->centre, _pt[j].p);
    VECT_CPY(param_tmp2->pos, _pt[j].p);
    VECT_CPY(param_tmp2->centre, _pt[j].p);

    point_suivant_courbeG(_pt + j, tmp_par[j],
			  param_tmp1, p_maill->nbMax_new, pt1,
			  p_maill, metr, 0.0, NULL);
    point_suivant_courbeG(_pt + j, tmp_par[j],
			  param_tmp2, p_maill->nbMax_new, pt2,
			  p_maill, metr, 0.0, NULL);
    if (j == 0){
      res = createMaillage(pt2, h, s->s_param);
      linit = res->lignes->l[0];
      if (ajouter_parall_diagonal(res, pt1, NULL,
				  p_maill, linit, (or + j) % 4,
				  or_vir_diag1, -1,
				  1, 3, ind_constr) <= -10)
	return NULL;
      //TODO : � changer...
      linit = liste_getVois(res->lignes->l[0], or);
      ind_seg = ptmaill_get_ind_seg((Ptmaill*)res->lignes->l[0]->el->el, or);
      ((Segment*)get_element_gtab(res->seg, ind_seg))->b_new = 0;
    }
  }
  calc_courbe_sing(res, nbptr, pt2, pt1,
		   the1, the2, h, h_c, base1[0], base1[1],
		   p_maill, or, res->lignes->l[0]);

  return res;
}

/*
 * Cherche dans la liste des points au bord les nouveaux ||grammes
 * qu'il peut construire
 * bool_unique : construit un seul ||gramme si � 1
 * init : uniquement utilis� avec bool_unique, on a commence �
 *    mailler par init
 */
int maillage_che(Maillage *m, Param_maillage* param_s,
		 int bool_unique, ListeD *init,
		 int ind_part, int ind_constr){
  SListe *bord;
  Liste *tmp;
  ListeD *tmp2D;
  Ptmaill *cour;
  int i, ind1, ind2, new_par;
  Segment *seg1, *seg2;
  int nbMax = param_s->nbMax_new;
  Param_new1 *param;
  ListeD *v1, *v2;
  int ind_surf = 1;
  int bool_aj;
  int sing = param_s->bool_sing;
  int bool_init = 1;
  int bool_term = 0;
  int nb_aj_max = 10 * NB_AJ_MAX;
  int nb_aj = 0;
  int imax;
  int ind_aj;

  tmp2D = init;
  if (tmp2D == NULL){
    bord = m->lignes->bord[ind_part];
    tmp = bord->l;
    tmp2D = get_elementL(tmp);
  }
  else{
    ind_part = tmp2D->el->ind_part;
    bord = m->lignes->bord[ind_part];
    tmp = bord->l;
  }
  cour = get_elementL2D(tmp2D);

  param = create_param_new1(cour->p->pt->p, cour->p->pt->p,
			    m->h, m->h, m->s, param_s);
  seg1 = 0;
  seg2 = 0;

  //on parcours le bord tri�
  while (!bool_term && tmp != NULL){
    if (init != NULL && bool_init)
      tmp2D = init;
    else
      tmp2D = get_elementL(tmp);
    //on parcours les couples d'ar�tes de l'�l�ment du bord
    bool_aj = 0;

    if (tmp2D->vois == NULL){
      imax = 4;
    }
    else {
      imax = tmp2D->vois->nb_vois;
      if (!tmp2D->vois->est_ferme)
	imax--;
    }
    cour = get_elementL2D(tmp2D);
    for (i = 0; i < imax; i++){
      new_par = 0;
      ind1 = ptmaill_get_ind_seg(cour, i);
      ind2 = ptmaill_get_ind_seg(cour, i + 1);
      if (ind1 != -1 && ind2 != -1 && ind1 != ind2){
	seg1 = get_element_gtab(m->seg, ind1);
	seg2 = get_element_gtab(m->seg, ind2);
	if (seg1->b_new && seg2->b_new
	    && seg1->q2 == NULL && seg2->q2 == NULL
	    && (seg1->q1 == NULL || seg1->q1 != seg2->q1)){
	  new_par = 1;
	}
      }
      if (new_par){
	nb_aj++;
	bool_aj = 1;
	v1 = liste_getVois(tmp2D, i);
	v2 = liste_getVois(tmp2D, i + 1);
	ind_aj = i + 1;
	if (tmp2D->vois == NULL)
	  ind_aj = ind_aj % 4;
	bool_term = ajouter_par(m, tmp2D, v1, v2,
				param_s, param, seg1,
				seg2, ind_aj, nbMax,
				sing, ind_surf, ind_constr);
	if (bool_unique)
	  bool_term = 1;
	if (bool_term)
	  break;
      }
    }
    if (sing != 1 && !bool_unique)
      bool_term = 0;
    if (nb_aj > nb_aj_max)
      bool_term = 1;
    if (bool_aj || (bool_init && init != NULL)){
      //on a ajout� un point => on reprend la liste depuis le d�but
      tmp = bord->l;
    }
    else{
      tmp = tmp->n;
    }
    bool_init = 0;
  }
  free(param);
  return !bool_term;
}

/*
 * On calcul le dernier point du ||gramme et on l'ajoute
 * lcur : le point de base du ||gramme
 * v1, v2 : ses 2 voisins
 * param : les param�tres pour la m�thode de Newton
 * seg1, seg2 : les segments reliant lcur � v1 et v2
 * ind_aj : l'orientation de l'ajout du ||gramme par rapport
 *    � lcur
 * nbMax : nombre maximal d'it�ration de Newton
 * sing : on traite les singularit�s ou non
 * ind_surf : inutilis�
 */
int ajouter_par(Maillage *m, ListeD *lcur, ListeD *v1,
		ListeD *v2, Param_maillage* param_s,
		Param_new1 *param, Segment *seg1,
		Segment *seg2,
		int ind_aj, int nbMax,
		int sing, int ind_surf, int ind_constr){
  Ptmaill *cur = get_elementL2D(lcur);
  double the;
  Pt* np = createPtN();
  int boo_sing = 0;
  int i;
  P3D del1, del2, N1, N2;
  Pt *pt_cur, *pt1, *pt2;

  /* calcul la normal associ� aux 2 premiers vecteurs*/
  pt_cur = getvect(lcur);
  pt1 = getvect(v1);
  pt2 = getvect(v2);
  if (lcur->vois == NULL){
    VECT_DEL(del1, pt_cur->p, pt1->p, i);
    VECT_DEL(del2, pt_cur->p, pt2->p, i);
    prod_vect(N1, del1, del2);
  }
  VECT_CPY(param->c1, pt1->p);
  VECT_CPY(param->c2, pt2->p);
  the = point_suivant_surface(pt1->p, pt2->p,
			      pt_cur, param,
			      nbMax, np, param_s, NULL);

  if (fabs(the) <= param_s->tol_angle && the > -10){
    if (lcur->vois == NULL){
      /* on calcul la 2eme normal
       * et on v�rifie qu'il n'y a pas eu de rebroussement */
      VECT_DEL(del1, np->p, pt1->p, i);
      VECT_DEL(del2, np->p, pt2->p, i);
      prod_vect(N2, del1, del2);
    }
    if (lcur->vois != NULL || prod_scal(N1, N2) <= EPS_TOL){
      ajouterPoint_comp(m, param_s, np, ind_constr, 
			lcur, ind_surf, ind_aj,
			boo_sing);
      cur->p->the = the;
      return 0;
    }

  }

  //on a une singularit� ou on n'a pas converg�
  cur->p->the = 1;
  boo_sing = 1;
  cur->p->the = 1;
  seg1->b_new = 0;
  seg2->b_new = 0;
  free(np);
  return 1;
  /* !! si on veut ajouter le point et s'arreter apr�s,
   *    il faut bien v�rifier dans point_suivant_surface
   *     que celui-ci a bien �t� calcul�....*/
}

/*
 * Utilis� pour la cr�ation d'une rosace :
 * copie les segments pour le point de la rosace qui est dupliqu�
 */
void copier_segment(Ptmaill *p_init, Ptmaill *p_n,
		    int ind_aj_ros, int sens){
  p_init->ind_seg[(ind_aj_ros + 3 + sens) % 4]
    = p_n->ind_seg[(ind_aj_ros + 2 - sens) % 4];
  p_n->ind_seg[(ind_aj_ros + 1 + sens) % 4]
    = p_n->ind_seg[(ind_aj_ros + 2 - sens) % 4];
}


/* Ajoute une diagonale :
 * or : 0 -> vers le haut ... puis on tourne dans le sens +
 * or_vir : la direction du virage : 0, sens n�gatif et 1, sens +
 * or_voistopt : utilis� dans le cas o� le premier point
 *               est une singularit�
 * or_base : l'orientation de la base d�finie par la direction
 * de la courbe (de pts[i] vers pts[i+1]) et son orthogonal
 *   (ce bool�en dit donc si on doit prendre l'orthogonal [or_base==1]
 *    ou son oppos� [or_base==0])
 */
int ajoute_diag_maill(Maillage *m,
		      PPPt *pts, int nb_dual, int* nb,
		      ListeD *init, int or, int or_vir,
		      int or_voistopt,
		      Param_maillage* param_s,
		      int or_base, int type_maill,
		      void(*callback_stop)(Maillage*, int,
					   int*, void*),
		      void *param_call, int fermer_sing, int ind_constr){

  int i, j, is_term = 0;
  PListeD suiv[NB_BRANCHES_MAX];
  int nb_part;
  Quadr *q;
  int nb_part_init = m->quadr->nb_part;
  int del = 3 + 2 * or_vir;
  int nb_max = nb[0];
  int bool_term[NB_BRANCHES_MAX];
  int or_cur[NB_BRANCHES_MAX];
  int or_voistopt_cur[NB_BRANCHES_MAX];
  ListeD *tmp_init;
  int bool_tmp = init->vois != NULL && init->vois->nb_vois == 0;
  Ptmaill *ptm1, *ptm2;
  Point *cp;

  for (i = 0; i < nb_dual; i++){
    bool_term[i] = 0;
    or_cur[i] = 2 * i + or;
    or_voistopt_cur[i] = (or_voistopt + 2 * i) % 4;
    nb_max = _max(nb_max, nb[i]);
    suiv[i] = init;
  }

  i = 1;
  while (i < nb_max && !is_term){
    for (j = 0; j < nb_dual; j++){
      if (!bool_term[j]){
	if (i == 1 && j == 1 && bool_tmp){
	  //on v�rifie que l'on a pas ajout� � l'envers 
	  //le 1er parall�logramme
	  ptm1 = get_elementL2D(init->vois->vois[or]);
	  ptm2 = get_elementL2D(init->vois->vois[or + 1]);
	  if (distance(ptm1->p->pt->p, pts[j][i]->p)
	      < distance(ptm2->p->pt->p, pts[j][i]->p)){
	    //on intervertit les 2 points
	    cp = ptm1->p;
	    ptm1->p = ptm2->p;
	    ptm2->p = cp;
	    //		PRINT_WAR_LOG(param_s, "2 pts interverti\n");
	  }
	}

	if (ajouter_parall_diagonal(m, pts[j][i], NULL, param_s,
				    suiv[j], or_cur[j], or_vir,
				    or_voistopt_cur[j],
				    or_base, type_maill, ind_constr) != 1){
#ifdef _DBG
	  Pt *tmp_ = getvect(suiv[j]);
#endif
	  PRINT_ERR_LOG(param_s, "probl�me ajout %d i�me point diagonal\n",i);
	  return i;
	}
	nb_part = m->quadr->nb_part;
	if (nb_part == 1 && m->quadr->l[0] == NULL){
	  PRINT_ERR_LOG(param_s, "probl�me ajout %d i�me point diagonal (2)\n", i);
	  return i;
	}
	else {
	  //		PRINT_WAR_LOG(param_s, "pt %d ajout�\n", i);
	}
	q = get_elementL2D(m->quadr->l[nb_part - 1]);
	q->est_diag = 1;
	tmp_init = suiv[j];
	suiv[j] = liste_getVois(suiv[j], or_cur[j]);
	if (suiv[j] == NULL)
	  return i;
	else{
	  if (i == 1 && init->vois != NULL)
	    or_cur[j] = (or_voistopt_cur[j] + 2) % 4;

	  suiv[j] = liste_getVois(suiv[j], or_cur[j] + del);
	  if (suiv[j] == NULL)
	    return i;
	}
	if (m->quadr->mailles[1] != NULL){
	  if (m->quadr->mailles[1]->nb_tot > 1)
	    return i;
	}
	maillage_che(m, param_s, 1, tmp_init, 0, ind_constr);
	if (m->quadr->nb_part > nb_part_init){
	  PRINT_WAR_LOG(param_s, "||gramme du point %d de la courbe %d pas ajout� => ajout diagonal termin�\nNB_PART : %d\n",
			i, j, m->quadr->nb_part);
	  suppr_point(suiv[j], m, param_s,1);
	  suppr_autres_composantes(m->quadr, param_s, 0,
				   callback_supprQuadr, m);
	  bool_term[j] = 1;
	}
      }
    }
    if (i == 1 && init->vois != NULL && fermer_sing){
      maillage_fermer_sing(m, init);
    }
    maillage_che(m, param_s, 0, NULL, 0, ind_constr);
    if (callback_stop != NULL)
      callback_stop(m, nb_dual, bool_term, param_call);

    is_term = 1;
    for (j = 0; j < nb_dual; j++){
      if (bool_term[j] == 0)
	bool_term[j] = (nb[j] <= i - 1);
      if (bool_term[j] == 0){
	is_term = 0;
	break;
      }
    }
    i++;
  }
  return i;
}

/*
 * On ajoute une courbe du dual partant d'un point init du bord du maillage.
 * On a ainsi, avec la courbe du primal du bord du maillage une condition
 * primale-duale
 */
int ajoute_courbe_dual_maillage(Maillage *m, 
				Courbe *c, ListeD *init,
				int or_primal,
				int or_vir, Param_maillage* param_s,
				int type_maill, int ind_constr){
  ListeD *cur = init, *suiv, *prec = NULL;
  double dist_courb = 0.0;
  P3D pos, param_tmp;
  Pt *pt_init = getvect(init);
  Pt *pt_inter, *pt_suiv, *newpt, *pt_prec = NULL;
  double psi = 0.0;
  double res_param, init_p;
  CourbeP *c_surf = NULL, *c_plan = NULL;
  Param_new3 *param_new3 = NULL;
  Param_new2 *param_new2 = NULL;
  P3D vect_dir, vect_tan;
  int i, j;
  int dir, ind_pts = 0;
  int del_ind;
  int ind_courb_geod = 0;
  double pas_tot = 0.0;
  int ind_tmp = 0;
  int or2; // = (or_primal + 3 + 2 * or_vir +_del) % 4;
  if (c->type == 0){
    c_plan = c->c_param;
    c_surf = surf_get_courbe_from_courbeP(m->s, c_plan);
    param_new3 = create_param_new3(c_surf, m->h, pos);
    if (c_plan->param == NULL){
      dir = c_plan->dir;
      init_p = pt_init->param[dir % 2];
    }
    else
      init_p = c->c_param->in;
  }
  else{
    param_new2 = create_param_new2(pos, NULL, pos, 0.0, 
				   0.0, m->s);
  }

  suiv = liste_getVois(cur, or_primal);
  if(suiv->saut != NULL){
    free(suiv->saut);
    suiv->saut = NULL;
  }
  i = 0;
  if (init->vois != NULL)
    or2 = (suiv->el->dir_sing + 3) % 4;
  while (suiv != NULL){
    newpt = createPtN();
    pt_inter = getvect(cur);
    pt_suiv = getvect(suiv);
    if (prec != NULL)
      pt_prec = getvect(prec);

    //calcul de psi
    VECT_DEL(vect_dir, pt_inter->p, pt_suiv->p, j);
    if (c->type == 2)
      ind_tmp = ind_pts;
    maill_dir_tan(m->s, c, init_p, ind_tmp, 
		  pt_inter, pt_prec, vect_tan);

    psi = angle(vect_dir, vect_tan);
    if (psi >= PI / 2.0 - EPS_TOL){
      PRINT_WAR_LOG(param_s, "angle trop grand entre les 2 courbes => maillage termin�\n");
      return i;
    }
    dist_courb = 2.0*m->h*cos(psi);
    if (c->type == 0){
      VECT_CPY(param_new3->pos, pt_suiv->p);
      if (!point_suivant_courbeP(m->s, init_p,
				 dist_courb, param_new3,
				 param_s, &res_param))
	return 0;
      courbeP_get_image(c_plan, res_param, param_tmp);
      if (!surf_setpt(m->s, newpt, param_tmp)){
	PRINT_WAR_LOG(param_s, "point %d : (%lf;%lf) sortie des bornes de la surface\n",
		      i, param_tmp[0], param_tmp[1]);
	return i + 1;
      }
    }
    else if (c->type == 1){
      param_new2->R = m->h;
      param_new2->the = 0.0;
      VECT_CPY(param_new2->pos, pt_inter->p);
      VECT_CPY(param_new2->centre, pt_suiv->p);
      if (!surf_calc_bon_dir(m->s, pt_inter, vect_tan,
			     param_new2->base, 1)){
	PRINT_ERR_LOG(param_s, "erreur calcul base\n");
	return i;
      }
      if (point_suivant_courbeG(pt_inter, NULL, param_new2,
				param_s->nbMax_new,
				newpt, param_s, NULL,
				dist_courb, NULL) != 1){
	return i;
      }
      pas_tot += distance(newpt->p, pt_inter->p);
      ind_tmp = -1;
      while (pas_tot > c->c_geod->pas){
	pas_tot -= c->c_geod->pas;
	ind_courb_geod++;
	ind_tmp = ind_courb_geod;
      }
      if (ind_courb_geod >= c->c_geod->size)
	break;
    }
    else {
      param_new2->R = m->h;
      del_ind = calc_nouv_pt_courbe(m->s, c->c_pts->nb - ind_pts,
				    pt_suiv, pt_inter,
				    c->c_pts->pts + ind_pts,
				    dist_courb, m->h, param_new2,
				    newpt, param_s, 1);
      ind_pts += del_ind + 1;
    }
    ajoute_pt(m, suiv, or2, -1, createPtmaill(newpt, ind_constr), param_s);
    maillage_che(m, param_s, 0, suiv, 0, ind_constr);

    init_p = res_param;
    prec = cur;
    cur = liste_getVois(cur, or_primal);
    if (init->vois != NULL && i == 0)
      or_primal = (cur->el->dir_sing + 2) % 4;

    cur = liste_getVois(cur, or2);
    suiv = liste_getVois(cur, or_primal);
    i++;
  }
  if (c_surf != NULL){
    free_fonc_vect(c_surf->param);
    free_courbeP(c_surf);
  }
  return i;
}

/*
 * Ajoute une diagonale ferm� au maillage. On appelle ajoute_diag_maill
 * puis on ajoute le parall�logramme permettant de fermer la courbe du dual
 */
void ajoute_diag_ferme_maill(Maillage *m, 
			     PPt *pts, int nb,
			     ListeD *init, int or, int or_vir,
			     Param_maillage* param_s,
			     int or_base, int type_maill, int ind_constr){
  ListeD *tmp = init, *prec = NULL;
  int or2 = (or + 3 + 2 * or_vir) % 4;

  int nb_aj = ajoute_diag_maill(m, &pts, 1, &nb, init,
				or, or_vir, -1, param_s,
				or_base, type_maill,
				NULL, NULL, 0, ind_constr);
  if (nb_aj >= 1){
    PRINT_WAR_LOG(param_s, "courbe ajout� (reste fermeture)\n");
    while (tmp != NULL){
      prec = tmp;
      tmp = liste_getVois(tmp, or);
      if (tmp != NULL){
	tmp = liste_getVois(tmp, or2);
      }
      /*   else{
	   PRINT_WAR_LOG(param_s, "2�me d�placement impossible...\n");
	   return;
	   }*/
    }
    if (!ajouter_parall_diagonal(m, pts[0], m->lignes->l[0],
				 param_s, prec, or, or_vir, -1,
				 or_base, type_maill, ind_constr)){
      PRINT_WAR_LOG(param_s, "impossible de fermer le maillage\n");
      return;
    }
    maillage_che(m, param_s, 0, NULL, 0, ind_constr);
  }
  /*if(nb_aj < nb){
    int i;
    for (i = nb_aj; i < nb; i++)
    free(pts[i]);
    }*/
}

/* calcul la diagonal pour une singularit�
 * Contrairement � l'algorithme mis en place
 * dans le cas o� on n'a pas de singularit�,
 * ici, on calcul un point de la
 * diagonal puis on maille le reste
 * On nomme ici c1 la diagonal dont on donne les angles
 * et c2 la 2eme diagonale
 * m : une initialisation du maillage avec le 1er ||gramme
 *   contenant les 2 premier points de la diagonale
 * s : la surface
 * nbptr  : le nombre de points de la diagonale
 * pt_suppr : le 1er point de la 2eme diagonale (on ne le met
 *       pas dans le maillage
 * npc2 : le 1er point de la 2eme diagonale
 * the1 : la courbure g�od�sique de la 1ere diagonale
 * the2 : la courbure g�od�sique de la 2eme diagonale
 * h_m : le pas du maillage
 * h_d : les pas du dual sur la 1ere diagonale
 * dir_init : vecteur indiquant la direction initiale
 *     de la 1ere diagonale
 * dir_init2 : direction orthogonale � dir_init
 *        (utilis� dans le cas d'un parm�trage donn� avec singularit�,
 *         ex : coord. polaire)
 * p_maill : les param�tres du calcul
 * or : l'orientation de la 1ere diagonale
 * linit : le 1er point de la 1ere diagonale
 */
int calc_courbe_sing(Maillage *m, 
		     int nbptr, Pt* pt_suppr,
		     Pt* npc2, double* the1,
		     double *the2, double h_m,
		     double* h_d, P3D dir_init,
		     P3D dir_init2,
		     Param_maillage *p_maill, int or,
		     ListeD *linit){
  int i, j;
  Param_new2 *param, *param2;
  P3D metr[2];
  P3D der[2];
  P3D pc2_prec, v_tmp, dir1;
  double *tmp = NULL;
  double n, ps;
  int boo;
  int nbMax = p_maill->nbMax_new;
  ListeD *lsuiv, *lprec = linit;
  ListeD *lsuiv_c2 = liste_getVois(linit, or);
  Pt* pc2calc;
  Pt *pc1, *pc1_prec, *pc2 = NULL;
  int ind_constr = 0;

  Newton_f fonc = f_newton_param_courbe;
  Newton_f_p fonc_p = f_newton_param_courbe_p;
  int dim = 2;
  int or_vir = 0;

  P3D dummy, p_res;
  P3D del, X0;
  Pt* np, *np_calc;

  ajoute_pt(m, lsuiv_c2, (or + 1) % 4, -1,
	    createPtmaill(npc2, ind_constr), p_maill);
  // VECT_CPY(pc2, pt_suppr->p); 
  /* passe directement � prec donc inutilis�
   * en l'�tat*/
  /* if (m->s->type == 2){
    dim = 3;
    fonc = f_newton_contr_courbe;
    fonc_p = f_newton_contr_courbe_p;
    }*/

  lsuiv = liste_getVois(linit, or);
  lsuiv = liste_getVois(lsuiv, (or + 3) % 4);

  param = create_param_new2(dummy, der, dummy,
			    h_d[1], the1[0], m->s);
  param2 = create_param_new2(dummy, der, dummy,
			     h_d[1], the2[0], m->s);

  for (i = 1; i < nbptr; i++){
    np_calc = createPtN();
    np = createPtN();

    pc1_prec = getvect(lprec);
    pc1 = getvect(lsuiv);

    if (i == 1)
      VECT_CPY(dir1, dir_init);
    else {
      VECT_DEL(dir1, pc1_prec->p, pc1->p, j);
    }
    if (i == 1 && dir_init2 != NULL){
      /* init. de la direction pour la diag.1 */
      VECT_CPY(param->base[0], dir_init);
      VECT_CPY(param->base[1], dir_init2);
    }
    else {
      surf_calc_bon_dir(m->s, pc1, dir1, param->base, 1);
    }

    /* newton de la diag.1 */
    tmp = NULL;
    param->R = h_d[i];
    param->the = the1[i];
    VECT_CPY(param->pos, pc1->p);
    VECT_CPY(param->centre, param->pos);
    boo = point_suivant_courbeG(pc1, tmp, param,
				nbMax, np_calc,
				p_maill, metr, 0.0, NULL);

    if (!boo){
      PRINT_WAR_LOG(p_maill, "Rebroussement courbe %d\n", i);
      i--;
      break;
    }

    /* on ajoute le ||gramme associ� � ce morceau de
     * diagonale et on maille
     */
    if (ajouter_parall_diagonal(m, np_calc, NULL, p_maill,
				lsuiv, or, or_vir, -1, 1, 3, ind_constr) <= -10)
      return -10;
    if (!maillage_che(m, p_maill, 0, NULL, 0, ind_constr))
      return i;

    lsuiv_c2 = liste_getVois(lsuiv_c2, or);
    if (lsuiv_c2 == NULL)
      return i;
    lsuiv_c2 = liste_getVois(lsuiv_c2, or + 1);
    if (lsuiv_c2 == NULL)
      return i;

    /* on calcul le racordement � la 2eme diagonale */
    param->the = the2[i];
    VECT_CPY(pc2_prec, pc2->p);
    pc2 = getvect(liste_getVois(lsuiv_c2, or + 2));
    pc2calc = getvect(lsuiv_c2);

    param2->R = h_m;
    VECT_CPY(param2->pos, pc2);
    VECT_CPY(param2->centre, pc2calc->p);
    VECT_DEL(dir1, pc2_prec, pc2->p, j);
    VECT_DEL(v_tmp, pc2->p, pc2calc->p, j);
    n = pow(norme(dir1), 2);
    ps = prod_scal(dir1, v_tmp);
    for (j = 0; j < 3; j++)
      del[j] = 2 * dir1[j] * ps / n;
    deplacement_infinitesimal(m->s, p_maill, pc2, del, X0);

    surf_calc_bon_dir(m->s, pc2, dir1, param2->base, 1);
    if (!surf_newton(m->s, X0, fonc, fonc_p,
		     (void*)param2, param->R, param->R,
		     p_res, p_maill, dim))
      return -10;

    // if (m->s->type == 1){
      surf_setpt(m->s, np, p_res);
      /* }
    else
    VECT_CPY(np->p, p_res);*/

    ajoute_pt(m, lsuiv_c2, (or + 1) % 4, -1,
	      createPtmaill(np, ind_constr), p_maill);

    lsuiv = liste_getVois(lsuiv, or);
    lsuiv = liste_getVois(lsuiv, or + 3);
  }
  return i;
}

/*
 * On calcul gr�ce aux courbes de Bezier les points d'une courbe
 * sur la surface s passant par les points pts avec les d�riv�es
 * der. Le premier point de la courbe doit �tre pts[0] et le dernier
 * pts[1] (les longueurs entre 2 points succesifs sont h)
 */
int calc_bezier(Surf_param *s, P3D pts[2], P3D der[2],
		Pt *res, double h, double tol,
		Param_maillage *p_maill, int or_vir){
  double H[SIZE_TAB];
  P3D dummy = {0.,0.,0.};
  double param_bez = 0.3;
  Fonc_vect *fonc_bez = create_fonction_bezier(pts, der, param_bez);
  CourbeP *courbe_bez = create_courbeP(fonc_bez, 0.0, 1.0, 0, 0.0, 0, dummy);
  double dern_param = 2.0;
  int boo_in = 1, is_term = 0;
  double aj = (-1 + 2 * or_vir)*0.1, dist = dern_param - 1.0;
  double dist_p = 0;
  double aj_p = 1.0;
  double delta_dist = 1.0;
  int i, si;
  int nbptr = NB_PTS_SING_MAX;
  Pt tmp;
  NewMult_courbes *new_mult;

#ifdef DBG_BEZIER
  int it = 1, itmax = 20;
  char name[1000] = "courbes/courbe";
  char n_file[1000];
#endif
  /* double sig;
     int si_in = 0;*/

  VECT_CPY(tmp.p, pts[0]);
  VECT_CPY(tmp.param, pts[0]);

  for (i = 0; i < nbptr; i++)
    H[i] = h;

  si = nbptr;
  while (!is_term){
    si = calc_courbeP_surf(si,
			   courbe_bez, NULL,
			   H, s,
			   p_maill, res, &dern_param, -1);
#ifdef DBG_BEZIER
    sprintf(n_file, "%s%d.plt", name, it);
    export_courbe(res, si, n_file);
    it++;
    if(it>itmax)
      break;
#endif
    dist_p = dist;
    dist = dern_param - 1.0;

    delta_dist = fabs(dist - dist_p);
    is_term = (fabs(dist) < tol);
    if (!is_term){
      if (boo_in){
	courbe_bez = split_courbe_bezier(courbe_bez, -1,
					 0.2, 0.5);
	dist = 1.0;
	boo_in = 0;
      }
      else {
	aj_p = aj;
	if (dist*dist_p < 0){
	  aj = -aj_p*fabs(dist) / delta_dist;
	}
	else {
	  aj = aj_p*dist / delta_dist;
	}
	new_mult = (NewMult_courbes*)courbe_bez->param->param;
	deplacer_pt_mult_bezier(new_mult->courbes, 1, aj);
      }
    }
  }
  return si;
}

/*
 * Calcul les points d'intersections entre
 * une surface et une sph�re (calcul� par la donn�e de 2 points
 * et une d�riv�e)
 * !!! Ne fonctionne que pour les surfaces param�tr�es (TODO)
 */
int calc_cercle(Surf_param *s, Pt* init,
		Pt* c1, Pt* p1,
		P3D base_p1[2],
		P3D dir_c, double h, Pt *pts,
		Param_maillage* param_s,
		double tol){
  P3D dir_c_tan, dir_p1, dir_p1_tan, nor_plan;
  int i, j;
  Param_new5 *param;
  Param_new1 *param2;
  Param_new2 *param3;
  double the;
  Pt* c2 = createPtN(), *pt_tmp = createPtN();
  int term;
  double R = 0.0, ps1, ps2;
  P3D vect_tmp, _dir;
  P3D v_tmp1, v_tmp2, _init;
  P3D X0;
  double tol2 = 100 * tol;
  int sig;
  double dist, dist_p, del_the = 0.4, del_dist;

  ptcopy(pts, c1);

  VECT_DEL(dir_p1, p1->p, c1->p, i);
  surf_projeter_tangent(s, p1, dir_p1, dir_p1_tan);
  normer(dir_p1_tan);

  surf_projeter_tangent(s, c1, dir_c, dir_c_tan);
  normer(dir_c_tan);

  the = 2 * acos(prod_scal(dir_c_tan, dir_p1_tan));

  rotation_vect(dir_c_tan, base_p1, the, nor_plan);

  param = create_param_new5(p1->p, c1->p, nor_plan, dir_c_tan, s);
  param2 = create_param_new1(init->p, init->p, h, R, s, param_s);
  param3 = create_param_new2(init->p, base_p1, init->p, R, 0, s);

  dist = tol + 1.0;
  while (fabs(dist) > tol){
    inters_double_plan_surface(p1, c1, dir_c_tan,
			       param->nor2, base_p1, param,
			       param_s->nbMax_new, c2,
			       param_s);
    dist_p = dist;
    dist = distance(c1->p, c2->p) - distance(p1->p, c2->p);
    del_dist = dist - dist_p;
    if (fabs(dist) > tol){    /* on actualise theta */
      if (signe(dist_p) != signe(dist))
	del_the = -del_the*fabs(dist) / fabs(del_dist);
      else
	del_the = dist*fabs(del_the) / fabs(del_dist);
      the += del_the;
      rotation_vect(dir_c_tan, base_p1, the, param->nor1);
    }
  }

  ps2 = 1 + tol;
  while (fabs(ps2) > tol){
    VECT_DEL(vect_tmp, c1->p, c2->p, i);
    R = norme(vect_tmp);

    param2->R2 = R;
    VECT_CPY(_dir, dir_c);
    for (j = 0; j < 3; j++)
      _dir[j] *= -h;
    i = 1;
    term = 0;

    /* on calcul tous les points du cercle */
    while (!term){
      VECT_CPY(param2->c1, pts[i - 1].p);
      VECT_CPY(param2->c2, c2->p);
      param2->R2 = R;
      deplacement_infinitesimal(s, param_s, &pts[i - 1],
				_dir, _init);
      if (point_suivant_surface(NULL, NULL, NULL, param2,
				param_s->nbMax_new, &pts[i],
				param_s, _init) <= -10)
	return -1;
      VECT_DEL(_dir, pts[i - 1].p, pts[i].p, j);

      VECT_DEL(v_tmp1, p1->p, pts[i - 1].p, j);
      VECT_DEL(v_tmp2, p1->p, pts[i].p, j);
      ps1 = prod_scal(v_tmp1, base_p1[1]);
      ps2 = prod_scal(v_tmp2, base_p1[1]);
      i++;
      if (ps1*ps2 < 0 || fabs(ps2) < tol)
	term = 1;
    }

    /* on d�cale le cercle dans le cas o� tous les points de
     * celui-ci ne rentrent pas
     */
    if (fabs(ps2) > tol){
      sig = -1;
      if (fabs(ps1) < fabs(ps2) && fabs(ps1) < tol2){
	i--;
	sig = 1;
      }
      VECT_CPY(param3->pos, p1->p);
      VECT_CPY(param3->centre, c1->p);
      param3->R = distance(c1->p, pts[i - 1].p);
      param3->the = 0.0;

      surf_calc_bon_dir(s, &pts[i - 1], base_p1[0],
			param3->base, 1);
      /*      surf_get_base_opt(s->s_param, pts[i-1].param,
	      der, metr);
	      calc_base_dir(der, metr, base_p1[0],
	      param3->base);*/

      init_inter_droite_sphere(s, param_s, init->p, p1->p, base_p1[0],
			       c1->p, param3->R,
			       &pts[i - 1], X0);

      if (point_suivant_courbeG(&pts[i - 1], NULL, param3,
				param_s->nbMax_new, pt_tmp,
				param_s, NULL, 0.0, X0) <= -10)
	return -1;
      VECT_DEL(v_tmp1, c1->p, pts[i - 1].p, j);
      VECT_DEL(v_tmp2, c1->p, pt_tmp->p, j);
      the = acos(prod_scal(v_tmp1, v_tmp2)
		 / (norme(v_tmp1)*norme(v_tmp2)));

      VECT_CPY(param3->pos, c1->p);

      surf_calc_bon_dir(s, c2, vect_tmp,
			param3->base, 1);
      /*  surf_get_base_opt(s->s_param, c2->param, der, metr);
	  calc_base_dir(der, metr, vect_tmp, param3->base);*/
      VECT_CPY(param3->centre, c1->p);
      param3->R = R;
      param3->the = sig*the;

      if (point_suivant_courbeG(c1, NULL, param3,
				param_s->nbMax_new, c2,
				param_s, NULL, 0.0, NULL) <= -10)
	return -1;
    }
  }
  return i;
}

/*
 * Cas o� on maille par une ligne du dual. On ajoute un point
 * dans le dual ainsi que les parall�logrammes que celui-ci d�fini
 * m : le maillage
 * s : la surface
 * the : l'angle du ||gramme au niveau du dual
 * h : le pas du maillage
 * dir : la direction du dual
 * p_maill : les param�tres du maillage
 * or : l'orientation de l'ajout
 * linit : le dernier point du maillage sur la courbe dual
 */
int calc_diag(Maillage *m,
	      double the, double h, P3D dir,
	      Param_maillage *p_maill, int or, int or_vir,
	      int or_base,
	      ListeD *linit, int ind_constr){
  Param_new2 *param;
  int nbMax = p_maill->nbMax_new;
  Pt* pc1;
  Pt* np_calc = createPtN();
  P3D base[2];

  if (the > PI / 2.0){
    PRINT_WAR_LOG(p_maill, "erreur : rebroussement sur la diagonale (angle > PI/2!!)\n");
    return 0;
  }
  pc1 = getvect(linit);
  surf_calc_bon_dir(m->s, pc1, dir, base, 1);

  param = create_param_new2(pc1->p, base, pc1->p,
			    2 * h*cos(the), 0.0, m->s);

  /* newton de la diag.1 */
  param->R = 2 * h*cos(the);
  param->the = 0.0;
  VECT_CPY(param->pos, pc1->p);
  VECT_CPY(param->centre, param->pos);
  if (!point_suivant_courbeG(pc1, NULL, param,
			     nbMax, np_calc,
			     p_maill, NULL, 0.0, NULL))
    return 0;

  /* on ajoute le ||gramme associ� � ce morceau de
   * diagonale et on maille
   */
  if (ajouter_parall_diagonal(m, np_calc, NULL, p_maill,
			      linit, or, or_vir, -1, or_base,
			      3, ind_constr) <= -10)
    return 0;
  if (!maillage_che(m, p_maill, 0, NULL, linit->el->ind_part, ind_constr))
    return 0;
  return 1;
}

void init_inter_droite_sphere(Surf_param *s, Param_maillage *param,
			      P3D init, P3D p1,
			      P3D vect, P3D c1, double R,
			      Pt* np, P3D res){
  P3D inter[2];
  P3D del;
  int nb_inter;
  int i, ind;

  nb_inter = intersection_droite_cercle(p1, vect,
					c1, R, inter);
  if (nb_inter == 1)
    ind = 0;
  else{
    if (distance(init, inter[0]) < distance(init, inter[1]))
      ind = 0;
    else
      ind = 1;
  }
  VECT_DEL(del, np->p, inter[ind], i);
  deplacement_infinitesimal(s, param, np, del, res);
}

void deplacer_courbe(Surf_param *s, Pt *pts, int nbpts, Pt *p,
		     Pt *res, Param_maillage *p_maill){
  int i;
  P3D dir;
  Param_new2 *p_new2;
  double l_tot = 0.0;
  double l[NB_PTS_SING_MAX];
  double R;
  for (i = 0; i < nbpts - 1; i++){
    l[i] = distance(pts[i].p, pts[i + 1].p);
    l_tot += l[i];
  }
  VECT_DEL(dir, pts[nbpts - 1].p, p->p, i);
  R = norme(dir);
  homothetie(dir, 1.0 / R);
  p_new2 = create_param_new2(pts[0].p, NULL,
			     pts[0].p, 0.0, 0.0, s);
  ptcopy(res, p);
  for (i = 1; i < nbpts; i++){
    surf_calc_bon_dir(s, &pts[i], dir, p_new2->base, 1);
    p_new2->R += l[i - 1] / l_tot;
    VECT_CPY(p_new2->pos, pts[i].p);
    VECT_CPY(p_new2->centre, pts[i].p);
    point_suivant_courbeG(&pts[i], NULL,
			  p_new2, p_maill->nbMax_new, res + i,
			  p_maill, NULL, 0.0, NULL);

  }
}

/*
 * on cr�� ici une singularit� (ou plut�t un secteur),
 * jonction de 3 courbes du dual : on se donne 2 courbes
 * ainsi que les angles autour de celles-ci et le reste du maillage
 * est bien d�fini
 * m : le maillage d�fini par une courbe du dual
 *     ainsi que les angles le long de celle-ci
 * pts : les points de la 2eme courbe du dual
 * h : le pas de la discr�tisation de cette 2eme courbes
 *     (les angles autour de cette courbe)
 */
void ajout_courbe_dual(Maillage *m, Surf *s, PPt *pts, int nbpts,
		       Param_maillage* param_s, int or,
		       int or_vir, int bool_fermer, 
		       int ind_constr){
  ListeD *init = m->lignes->l[0];
  int or_base = 1;
  int or_voistopt = or;
  if (init->vois == NULL)
    maillage_set_sing(m, init);
  ajoute_diag_maill(m, &pts, 1, &nbpts, init, 2,
		    or_vir, or_voistopt, param_s,
		    or_base, 3, NULL, NULL, 0, ind_constr);
  if (bool_fermer)
    maillage_fermer_sing(m, init);
  maillage_che(m, param_s, 0, NULL, 0, ind_constr);
}

/*
 * pts : les points d�finissant les 2 courbes
 * npts : de la place disponible pour ajouter des points
 */
void jonction_dual_pts(Maillage *m, int nb_dual,
		       PPt *pts, PPPt *npts, int *nbpts,
		       Param_maillage* param_s,
		       int or, int or_vir, double eps,
		       int fermer_sing, int ind_constr){
  ListeD *init = m->lignes->l[0];
  int or_base = 1;
  int or_voistopt = (or + 2) % 4;
  Param_call_sing *param;
  TabInt ind_npts[NB_BRANCHES_MAX];
  Pint _ind_npts[NB_BRANCHES_MAX];
  int pos_sing = 0;
  int i;

  for (i = 0; i < nb_dual; i++){
    if (nbpts[i] < 2)
      return;
    _ind_npts[i] = ind_npts[i];
  }
  param = create_param_call_sing(init, eps, or,
				 pos_sing, nb_dual, nbpts,
				 pts, npts, _ind_npts,
				 param_s, m->s, m->h);
  if (param == NULL)
    return;
  maillage_set_sing(m, init);
  ajoute_diag_maill(m, npts, nb_dual, nbpts, init, 0,
		    or_vir, or_voistopt, param_s,
		    or_base, 3, callback_sing, param, 
		    fermer_sing, ind_constr);
}

/* 
 * Subdivise la courbe c d�finie par une liste de points
 * dist_min : la distance minimale requise entre 2 points
 * nbpt : le nombre de points sur lequel on fait la subdivision
 */
int subd_courbe(Surf_param *s, Param_new2 *param_new, 
		SListe *c, Liste *pt, 
		double R, double dist_min, int nbpt, 
		Param_maillage *param_m){
  Pt pos[2], pt_prec;
  int ind = 0;
  int ind_suiv;
  Pt *new;
  int nb = 2;
  
  while(pt != NULL && pt->n != NULL && ind < nbpt){
    ind_suiv = -1;
    ptcopy(pos, (Pt*) pt->n->el);
    nb = 1;
    if(pt->n->n != NULL){
      ptcopy(pos+1, (Pt*) pt->n->n->el);
      nb = 2;
    }
    while(ind_suiv == -1){
      ptcopy(&pt_prec, (Pt*) pt->el);
      new = createPtN();
      ind_suiv = calc_nouv_pt_courbe(s, nb, &pt_prec, &pt_prec, 
				     pos, R, R, param_new, 
				     new, param_m, 1);
      if(ind_suiv > -1){
	// free(pt->n->el);
	pt->n->el = new;
      } else if(ind_suiv != -10){
	ajouter_apres_liste(c, pt, new);
      } else
	return 1;
      pt = pt->n;
      if(ind_suiv>0 || ind_suiv < -1){
	/* on a pass� 2 points d'un coup 
	   (�tonnant vu le but de la fonction) */
	return 1;
      }
    }
    ind++;
  }
  return 1;
}

/*
 * Calcul une nouvelle discretisation de la courbe sur la surface
 * p1 : le 1er point de la courbe
 * pts : les points suivants
 * p_prec : le centre de la sph�re (inutilis� si bool_p1_centre=1)
 * Retourne l'indice du segment sur lequel se situe le point
 *  et -10 si l'on a d�pass� tous les segments
 */
int calc_nouv_pt_courbe(Surf_param *s, int nb, Pt *p1, Pt* p_prec,
			Pt *pts, double pas_pos, double pas_centre,
			Param_new2 *param_new, Pt *res,
			Param_maillage *param_m,
			int bool_p1_centre){
  P3D del;
  Pt* pos = NULL;
  int i, j = 0;
  double dist;
  int term = 0;
  int ret;
  if(!bool_p1_centre)
    p_prec = p1;
  while (!term){
    if (j == 0) {
      VECT_DEL(del, p_prec->p, pts[0].p, i);
    }
    else {
      VECT_DEL(del, pts[j - 1].p, pts[j].p, i);
    }
    dist = norme(del);
    term = (pas_pos <= dist - EPS_TOL);
    if (j >= nb)
      return -10;
    if (!term){
      if (pas_pos <= (dist + EPS_TOL)){
	ptcopy(res, &pts[j]);
	return j - 1;
      }
      pas_pos -= dist;
      j++;
    }
    else{
      if (j == 0)
	pos = p_prec;
      else
	pos = &pts[j - 1];
    }
  }
  ret = nouv_pt_courbe(s, p1->p, pos, del, pas_centre,
		       param_new, res, param_m);
  if(ret != 1)
    return -10;
  return j - 1;
  }


/*
 * calcul le point suivant sur la courbe pos 
 * d�finit par une r�union de segments
 * pas_centre : la distance de centre au nouveau point sur la courbe
 */
int nouv_pt_courbe(Surf_param *s, double *centre, Pt *pos,
		   P3D dir, double pas_centre,
		   Param_new2 *param_new, Pt *res,
		   Param_maillage *param_m){
  int i, nbMax = param_m->nbMax_new;
  double ang = 0.0;
  double tmp = pas_centre;
  if (distance(centre, pos->p) > EPS_TOL){
    P3D _v1, _v2;
    double ps;
    VECT_DEL(_v1, pos->p, centre, i);
    VECT_DEL(_v2, pos->p, pos[1].p, i);
    ps = prod_scal(_v1, _v2) / (norme(_v1)*norme(_v2));
    if (fabs(fabs(ps) - 1) > EPS_TOL){
      tmp = 2.0*ang*pas_centre;
    }
  }
  param_new->R = pas_centre;
  surf_calc_bon_dir(s, pos, dir, param_new->base, 1);
  VECT_CPY(param_new->pos, pos->p);
  VECT_CPY(param_new->centre, centre);
  param_new->the = 0.0;

  return point_suivant_courbeG(pos, NULL, param_new, nbMax, res,
			       param_m, NULL, tmp, NULL);
}

void maill_dir_tan(Surf_param *s, Courbe *c, double param, int ind,
		   Pt *pos, Pt *prec, double *dir_tan){
  P3D tmp;
  int i;
  if (c->type == 0){
    CourbeP *cp = c->c_param;
    P3D par, tmp2[3];
    image_fvect(cp->param, param, par);
    image_cvect_p(s->f, par, tmp2);
    image_fvect_p(cp->param, param, tmp);
    prod_mat_vect(tmp2, 3, 2, tmp, dir_tan);
  }
  else if (c->type == 1){
    double the = 0.0;
    if (ind >= 0)
      the = c->c_geod->courbure[ind];
    if (prec == NULL){
      VECT_CPY(tmp, c->c_geod->dir_init);
      pos = c->c_geod->init;
    }
    else {
      VECT_DEL(tmp, prec->p, pos->p, i);
    }
    surf_rotation_vect(s, pos, tmp, the, dir_tan);
  }
  else{
    VECT_DEL(tmp, c->c_pts->pts[ind + 1].p, 
	     c->c_pts->pts[ind + 1].p, i);
    surf_projeter_tangent(s, c->c_pts->pts, tmp, dir_tan);
  }
}

void maill_dir_tan_init(Surf_param *s, Courbe *c, double *dir_tan){
  double param_init = 0.0;
  if (c->type == 0){
    CourbeP *c_plan = c->c_param;
    int dir;
    if (c_plan->param == NULL){
      dir = c_plan->dir;
      param_init = c_plan->param_init[dir%2];
    }
    else
      param_init = c->c_param->in;
  }
  maill_dir_tan(s, c, param_init, 0, NULL, NULL, dir_tan);
}


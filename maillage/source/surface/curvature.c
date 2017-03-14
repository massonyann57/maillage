#include "curvature.h"

Mesh_curv* create_mesh_curv(Surf_param *s,
			    double dist_bord_constr,
			    double dist_sing_min,
			    Param_maillage *param_s, 
			    int nbx, int nby){
  int i, j, k;
  int ind[4];
  P3D p1, p2, ptmp, mid;
  SListeD *sl = createSListeD(NULL, NULL, print_el_quadr, NULL,
			      NULL, NULL, NULL, 0, 0, -1, 0);
  Mesh_curv *res = malloc(sizeof(Mesh_curv));
  double curv;
  ListeD *tmp = NULL, *prem = NULL, *vois;
  ListeD *prem_x = NULL;
  Quadr_mesh *quadr;
  int or = 3;
  int ind_prec;
  P3D par_tmp;
  P3D base[2], metr[2];
  double *init = res->min_surf;
  double *fin = res->max_surf;
  double *tmp1, *tmp2;
  double _del = dist_sing_min;
  int dec_borne[2][2];
  int bool_util;
  double _del_[2];
  int quo_x = 0, quo_y = 0;
  if (s->ind_quotient % 2 == 1)
    quo_x = 1;
  if (s->ind_quotient / 2 == 1)
    quo_y = 1;

  init[0] = s->init[0]; init[1] = s->init[1];
  fin[0] = s->fin[0]; fin[1] = s->fin[1];

  //  PRINT_WAR_LOG(param_s, "debut creation mesh : quotient : %d\n", s->ind_quotient);
  for (i = 0; i < 2; i++){
    if (i == 0){
      tmp1 = init;
      tmp2 = fin;
    }
    else {
      tmp2 = init;
      tmp1 = fin;
      _del *= -1.;
    }
    for (j = 0; j < 2; j++){
      par_tmp[j] = tmp1[j];
      par_tmp[(j + 1) % 2] = (tmp1[(j+1)%2] + tmp2[(j+1)%2])/2.;
      surf_calc_base(s, par_tmp, base, metr);
      if (estNulle(deter(metr, 2))){
	tmp1[j] += _del;
	dec_borne[j][i] = 1;
      }
      else
	dec_borne[j][i] = 0;
    }
  }
  _del_[0] = (fin[0] - init[0]) / nbx;
  _del_[1] = (fin[1] - init[1]) / nby;
  res->mesh = sl;
  res->pts = create_gtab(NULL, NULL);

  p1[0] = init[0];
  p1[1] = init[1];
  p2[0] = p1[0]; p2[1] = p1[1] + _del_[1];
  ind_prec = curv_aj_pt(s, p1, res->pts);
  ptmp[0] = p1[0];
  ptmp[1] = p1[1];
  i = 0;
  //  PRINT_WAR_LOG(param_s, "avant boucle creation mesh\n");
  while (p1[0] - fin[0] < -EPS_TOL){
    p1[0] += _del_[0];
    p2[0] += _del_[0];
    p1[1] = init[1];
    p2[1] = init[1];

    ind[1] = ind_prec;
    ind[0] = curv_aj_pt(s, p1, res->pts);
    ind_prec = ind[0];
    j = 0;
    ind[2] = ind[1];

    while (p2[1] - fin[1] < -EPS_TOL){
      if (i == 0){
	ptmp[1] += _del_[1];
	ind[2] = curv_aj_pt(s, ptmp, res->pts);
      }
      else {
	if (i == 1)
	  ind[2] += 2;
	else
	  ind[2]++;
      }
      p2[1] = p1[1] + _del_[1];

      ind[3] = curv_aj_pt(s, p2, res->pts);

      mid[0] = p1[0] - _del_[0] / 2.;
      mid[1] = p1[1] + _del_[1] / 2.;
      curv = surf_get_curv(s, mid);
      bool_util = 1;
      for (k = 0; k < 2; k++){
	if(dec_borne[k][1] && (fin[k] - p2[k] 
			       < dist_bord_constr - EPS_TOL 
			       || fin[k] - p2[k] <= _del_[k] + EPS_TOL))
	  bool_util = 0;
	else if(dec_borne[k][0] && (p2[k] - init[k] 
				    < dist_bord_constr - EPS_TOL
				    || p2[k] - init[k] <= _del_[k] + EPS_TOL))
	  bool_util = 0;
      }
      quadr = create_quadr_mesh(ind, res->pts, curv, bool_util);
      or = 3;
      vois = tmp;
      if (j == 0){
	or = 0;
	vois = prem;
      }
      tmp = ajouter_listeD(sl, vois,
			   quadr, NULL, or, NULL, 0, 0,
			   0, -1, param_s);
      //	PRINT_WAR_LOG(param_s, "i : %d   j : %d\n", i, j);
      if (prem_x == NULL){
	prem_x = tmp;
	//	PRINT_WAR_LOG(param_s, " !!!!!!!!!!!!!! new prem_x : i : %d   j : %d\n", i, j);
      }
      if (quo_x && i == nbx - 1 && j == 0){
	//		PRINT_WAR_LOG(param_s, "prem_x->pH : %p      prem_x->nL->pH : %p  tmp->nH : %p\n", prem_x->pH, prem_x->nL->pH, tmp->nH);
	ajouter_vois(sl, tmp, prem_x, 0, 2, param_s);
	prem_x = prem_x->nL;
      }
      ind[1] = ind[2]; ind[0] = ind[3];
      p1[1] = p2[1];
      if (j == 0){
	prem = tmp;
      }
      j++;
    }
    if (quo_y){
      ajouter_vois(sl, tmp, prem, 3, 1, param_s);
    }
    i++;
  }
  return res;
}

int curv_aj_pt(Surf_param *s, P3D param,
	       GTab *pts){
  Pt *new = createPtN();
  surf_setpt(s, new, param);
  return aj_gtab(pts, new);
}

void free_mesh_curv(Mesh_curv *m){
  free_gtab(m->pts, free);
  free_slisteD(m->mesh);
  free(m);
}

Quadr_mesh* create_quadr_mesh(int ind[4], GTab *tab,
			      double curv, int bool_util){
  int i, j;
  PPt pts[4];
  P3D pv, v1, v2;
  double *par;
  Quadr_mesh *res = malloc(sizeof(Quadr_mesh));
  res->bool_util = bool_util;
  res->aire = 0.;
  for (j = 0; j < 2; j++){
    res->borne[j][0] = (double)INT_MAX;
    res->borne[j][1] = -res->borne[j][0];
  }
  for (i = 0; i < 4; i++){
    res->pts[i] = ind[i];
    pts[i] = get_element_gtab(tab, ind[i]);
    par = pts[i]->param;
    for (j = 0; j < 2; j++){
      if (par[j] < res->borne[j][0])
	res->borne[j][0] = par[j];
      if (par[j] > res->borne[j][1])
	res->borne[j][1] = par[j];
    }
  }
  for (i = 1; i < 4; i += 2){
    VECT_DEL(v1, pts[i]->p, pts[i - 1]->p, j);
    VECT_DEL(v2, pts[i]->p, pts[(i + 1) % 4]->p, j);
    prod_vect(pv, v1, v2);
    res->aire += norme(pv) / 2.;
  }
  res->curv = curv;
  res->passage = 0;
  res->passage2 = -1;
  res->aire_inter = 0.;
  res->dir_prop = -1;
  res->dirs = 0;
  res->pt_dir = NULL;
 
  /*DBG*/ res->curv_aj = 0.;
  return res;
}

/*
 * On cherche le quadr contenant p (on suppose que le maillage suit les
 *  lignes de param de la surface)
 */
ListeD* curv_trouver_quadr(Mesh_curv *mesh, Pt *p, ListeD *guess){
  ListeD *res = guess, *prec;
  SListeD *sl = mesh->mesh;
  Quadr_mesh *m;

  if (guess == NULL)
    res = sl->l[0];

  while (res != NULL){
    prec = res;
    m = res->el->el;
    if (p->param[0] < m->borne[0][0] - EPS_TOL)
      res = res->pH;
    else if (p->param[0] > m->borne[0][1] + EPS_TOL)
      res = res->nH;
    if (res != NULL){
      if (p->param[1] < m->borne[1][0] - EPS_TOL)
	res = res->pL;
      else if (p->param[1] > m->borne[1][1] + EPS_TOL)
	res = res->nL;
    }
    if (res == prec){
      return res;
    }
  }
  return NULL;
}


int calcul_secteur_courbe_geod(Surf_param *s, Mesh_curv *mesh, 
			       SListe *c, int ind_c, 
			       Param_maillage *param_s,
			       ListeD **inits, int *nb_init){
  ListeD *cur, *_prec;
  Quadr_mesh *quadr;
  int inter[4];
  PPt ptquadr[4];
  int term, i, j;
  double prop[4];
  int nb_inter = 0, diff_pos;
  int or = -10, or_prec = -10;
  P3D pv, v1, v2;
  double _prop, prop_prec = -1.;
  int est_int = 0;
  int nb_quadr = 0;
  int app_quadr;
  int pos_prec = -1;
  int pos_min = -1;
  int nb_rec, nb_suiv, nb_max = 2;
 
  int term2 = 0;
  double borne_tmp[2][2];
  int prec_coin = 0, est_coin;
  double suppr_prop[2];
  int suppr_inter[2];
  int nb_suppr_inter;
  int suiv_null = 0, ind_dec = -1;
  double _tol_petit = 0.60;
  int or_prec2 = -10;
  int bool_pr_parall, bool_dej_passe;
  int prec_dec = 0, _tmp;
  int choix_ret = -1;
  Pt pt_mesh1, pt_mesh2;
  P3D param_mesh;
  double metr_m, metr_c;
  double rapp_metr_max = 2.1;
  double rapp_metr_min = 0.99; //TODO : 0.8
  double Rsubd;
  int nbsubd = 5;
  int ind_dec_prec;

  int dbg_nb_pass = 0;
  int aj_or, term_dec;

  Liste *ptcur = c->l, *ptprec, *pttmp=NULL, *ptcur2;
  Param_new2 *param_new = create_param_new2(param_mesh, NULL, 
					    param_mesh, 0., 
					    0., s);
  int aj_passage[4];
  int aj_tot = 0;
  for(i = 0;i<4;i++){
    aj_passage[i] = (int)pow(2,i);
    aj_tot += aj_passage[i];
  }

  param_mesh[2] = 0.0;

  for (i = 0; i < 2; i++){
    borne_tmp[i][0] = mesh->min_surf[i];
    borne_tmp[i][1] = mesh->max_surf[i];
  }

  nb_init[0] = 1;
  inits[0] = curv_trouver_quadr(mesh, (Pt*) ptcur->el, NULL);
  cur = inits[0];
  ptcur = ptcur->n;
  while (!term2){
    if (cur == NULL){
      ptprec = ptcur;
      if (inits[0] == NULL){
	ptprec = ptprec->p;
	nb_init[0] = 0;
      }
      while (ptcur != NULL 
	     && !est_dans_borne(ptcur->el, borne_tmp))
	ptcur = ptcur->n;
      if (ptcur != NULL && ptprec != ptcur){
	/* le point était dans un trou
	 * (une singularité du paramétrage) */
	term = 0;
	ptprec = ptcur->p;
	while(!term){
	  while (ptcur != NULL 
	     && !est_dans_borne(ptcur->el, borne_tmp))
	    ptcur = ptcur->n;
	  ptprec = ptcur->p;
	  cur = curv_trouver_quadr(mesh, (Pt*) ptcur->el, NULL);
	  quadr = cur->el->el;
	  if(quadr->bool_util){
	    /* on n'est pas dans la zone inutilisé => on subdivise */
	    if(ptprec == NULL){
	      PRINT_WAR_LOG(param_s, "erreur ptprec null pour la subdivision (singularité du paramétrage)\n");
	    }
	    metr_c = distance(((Pt*)ptcur->el)->p, 
			      ((Pt*)ptprec->el)->p);
	    Rsubd = metr_c/nbsubd;
	    subd_courbe(s, param_new, c, ptprec, Rsubd, 
			Rsubd/10., 1, param_s);	
	    term = 0;
	  } else term = 1;
	  ptcur = ptprec->n;
	}
	inits[nb_init[0]] = cur;
	nb_init[0]++;
	or = -10;
      }
      else
	break;
    }
    quadr = cur->el->el;
    if(quadr->passage != ind_c){
      quadr->dirs = 0;
    }

    bool_dej_passe = 0;
    for (i = 0; i < 4; i++)
      ptquadr[i] = get_element_gtab(mesh->pts,
				    quadr->pts[i]);
    if (or_prec >= 0 && or == (or_prec + 2) % 4)
      est_coin = 1;
    if (quadr->passage != 0 || (or_prec >= 0 && or == (or_prec+2)%4)){
      if (quadr->passage == ind_c
	  || (or == (or_prec + 2) % 4 && or_prec >= 0)){
	ptprec = ptcur;
	while (appartient_quadr(ptquadr, (Pt*)ptcur->el, 
				quadr->borne)){
	  pttmp = ptcur;
	  ptcur = ptcur->n;
	  quadr->dirs = 0;
	  if (ptcur == NULL){
	    if(calc_inter_fin_courbe(s, cur, param_new, 
				     c, param_s)){
	      if(quadr->passage == 0)
		quadr->passage = ind_c;
	      PRINT_WAR_LOG(param_s, "calc secteur : nb_passage : %d\n", 
			    dbg_nb_pass);
	      return 1;
	    }
	    else ptcur = pttmp->n;
	  }
	}
	if (!suiv_null || ind_dec >= 0){
	  ind_dec_prec = ind_dec;
	  ind_dec = ajuste_borne(ptprec->el, ptcur->el,
				 quadr->borne);
	  if(ind_dec >= 0){
	    or_prec2 = or_prec;
	    or_prec = or;
	    prec_coin = 0;
	    or = ind_dec;
	    quadr->dirs = aj_tot-aj_passage[ind_dec];
	  } else if (prec_coin){
	    /* on vient d'un coin */
	    _tmp = or_prec2;
	    or_prec2 = or_prec;
	    or_prec = or;
	    if (ind_dec_prec != -1 && (ptprec != ptcur || suiv_null)){
	      if (choix_ret == -1){
		or = _tmp;
		choix_ret = or;
	      }
	      else {
		or = (choix_ret + 2) % 4;
		choix_ret = -1;
	      }
	    }
	    else if (ind_dec_prec != -1){
	      if (choix_ret == -1){
		if (or != (or_prec2 + 3) % 4)
		  or = (or_prec2 + 1) % 4;
		else {
		  or = (or_prec2 + 3) % 4;
		}
		choix_ret = or;
	      }
	      else{
		or = (choix_ret + 2) % 4;
		choix_ret = -1;
	      }
	    }
	    else
	      or = (or + 1) % 4;
	    or_prec2 = _tmp;
	    if(quadr->pt_dir != ptcur){
	      quadr->pt_dir = ptcur;
	      quadr->dirs = 0;
	    }
	    aj_or = 0;
	    while(aj_or < 4 && (quadr->dirs 
		  % (2*aj_passage[(or+aj_or)%4]))
		  / aj_passage[(or+aj_or)%4] > 0)
	      aj_or++;
	    if(aj_or == 4){
	      PRINT_WAR_LOG(param_s, "on a fait toutes les directions \
à partir de ce point => erreur du calcul \
des bords du secteur\n");
	      return 0;
	    }
	    or = (or+aj_or)%4;
	    quadr->dirs += aj_passage[or];
	  } else {
	    PRINT_WAR_LOG(param_s, "ERREUR cas inconnu ?\n");
	  }
	  cur = liste_getVois(cur, or);
	  if (cur != NULL){
	    quadr = cur->el->el;
	    /* dans le cas où ind_dec est >=0, 
	       on indique la direction qu'on a prise*/
	    if(ind_dec >= 0){
	      if((quadr->dirs 
		  % (2*aj_passage[(ind_dec+2)%4]))
		 /aj_passage[(ind_dec+2)%4] == 0)
		quadr->dirs += aj_passage[(ind_dec+2)%4];
	    }

	    for (i = 0; i < 4; i++)
	      ptquadr[i] = get_element_gtab(mesh->pts,
					    quadr->pts[i]);
	    if (quadr->passage == ind_c){
	      /*	PRINT_WAR_LOG(param_s, 
			"erreur0 limite secteur %d\n",
			nb_quadr);*/
	      bool_dej_passe = 1;
	      //return 0;
	    }
	    else if (quadr->passage == 0)
	      quadr->passage = ind_c;
	    else
	      quadr->passage2 = ind_c;
	  }
	  else
	    break;
	  suiv_null = 0;
	}
	else {
	  break;
	}
      }
      else{
	quadr->passage2 = ind_c;
	choix_ret = -1;
      }
    }
    else {
      quadr->passage = ind_c;
      choix_ret = -1;
    }
    term = 0;
    est_int = 0;
    ptprec = ptcur;
    while (!term){
      app_quadr = appartient_quadr(ptquadr, (Pt*)ptcur->el, 
				   quadr->borne);
      term = (app_quadr == 0);
      if (!term){
	pttmp = ptcur;
	ptcur = ptcur->n;
	if (ptcur == NULL){
	  if(calc_inter_fin_courbe(s, cur, param_new, 
				   c, param_s)){
	    /*   PRINT_WAR_LOG(param_s, "calc secteur : nb_passage : %d\n", 
		 dbg_nb_pass);*/
	    ptcur = pttmp;
	    term = 1;
	    term2 = 1;
	  }
	  else {
	    ptcur = pttmp->n;
	  }
	}
	est_int = 1;
	_prec = liste_getVois(cur, or+2);
	((Quadr_mesh*)_prec->el->el)->dirs -= aj_passage[or];
      }
    }  
    if (ptcur == NULL)
      ptcur = c->fin;
  
    if (app_quadr < 0)
      app_quadr = abs(app_quadr + 1);
    else
      app_quadr = -1;
    pos_prec = -1;
    nb_inter = -1;
    ptprec = ptcur->p;

    /* on adapte la métrique de la courbe */
    metr_c = distance(((Pt*)ptcur->el)->p, ((Pt*)ptprec->el)->p);
    param_mesh[0] = quadr->borne[0][0];  param_mesh[1] = quadr->borne[1][0];
    surf_setpt(s, &pt_mesh1, param_mesh);
    param_mesh[0] = quadr->borne[0][1];  param_mesh[1] = quadr->borne[1][1];
    surf_setpt(s, &pt_mesh2, param_mesh);
    metr_m = distance(pt_mesh1.p, pt_mesh2.p);
    while(metr_c/rapp_metr_min < metr_m){
      /* on augmente la metrique de la courbe (on saute des points) */
      ptcur = ptcur->n;
      quadr->dirs = 0;
      if(ptcur == NULL){
	ptcur = c->fin;
	break;
      } else
	suppr_el_liste(c, ptcur->p);
      ptprec = ptcur->p;
      metr_c = distance(((Pt*)ptcur->el)->p, ((Pt*)ptprec->el)->p);
    }
    if(metr_c/rapp_metr_max > metr_m){
      /* on baisse la métrique de la courbe (on subdivise) */
      Rsubd = metr_m*rapp_metr_min;
      quadr->dirs = 0;
      subd_courbe(s, param_new, c, ptprec, Rsubd, 
		  Rsubd/10., nbsubd, param_s);	  
    }

    ptcur = ptprec->n;
    est_int = 1;
    while(ptprec != NULL && est_int){
      est_int = appartient_quadr(ptquadr, (Pt*)ptprec->el, 
				 quadr->borne);
      if(est_int){
	pttmp = ptprec;
	ptprec = ptprec->p;
      }
    }
    if(ptprec == NULL){
      ptprec = pttmp;
      est_int = 0;
    }

    term = 0;
    nb_rec = 0;
    nb_suiv = 0;
    pos_min = -1;
    ind_dec = -1;
    bool_pr_parall = 0;
    ptcur2 = ptcur;
    while (!term){
      est_coin = 0;
      nb_inter = intersection_segm_quadr(ptquadr, 
					 ((Pt*)ptprec->el)->p,
					 ((Pt*)ptcur->el)->p, inter, prop,
					 &pos_prec, &pos_min);
      nb_suppr_inter = 0;
      for (i = 0; i < nb_inter; i++){
	if (estNulle(prop[i])){
	  suppr_inter[nb_suppr_inter] = inter[i];
	  suppr_prop[nb_suppr_inter] = prop[i];
	  nb_suppr_inter++;
	  for (j = i; j < nb_inter - 1; j++){
	    inter[j] = inter[j + 1];
	    prop[j] = prop[j + 1];
	  }
	  nb_inter--;
	  i--;
	}
	else if (estNulle(1. - prop[i]) || prop[i] > 1.){
	  prop[i] = 1.;
	  est_coin = 1;
	}
      }
      if (nb_inter == 0){
	for (i = 0; i < nb_suppr_inter; i++){
	  inter[i] = suppr_inter[i];
	  prop[i] = suppr_prop[i];
	  est_coin = 1;
	}
	nb_inter = nb_suppr_inter;
      }
      else if (nb_inter < 2 && nb_suppr_inter > 0){
	for (i = 0; i < nb_suppr_inter; i++){
	  if (inter[0] != suppr_inter[i]
	      && ((inter[0] + 1) % 4 != suppr_inter[i]
		  || prop[0] < 1.)){
	    /* on a supprimé un point qu'on n'aurait pas du...*/
	    est_coin = 1;
	    nb_inter++;
	    if (suppr_inter[i] > inter[0]){
	      inter[1] = suppr_inter[i];
	      prop[1] = suppr_prop[i];
	    }
	    else {
	      inter[1] = inter[0];
	      prop[1] = prop[0];
	      inter[0] = suppr_inter[i];
	      prop[0] = suppr_prop[i];
	    }
	  }
	}
      }


      if ((or == -10 || est_int) && nb_inter == 2){
	/* 2 cas : - cas où on est près d'un segment
	 *         - cas pourri où on est parallèle
	 */
	inter[0] = inter[(pos_min + 1) % 2];
	prop[0] = prop[(pos_min + 1) % 2];
	if (prop[0] == 0.){
	  inter[0] = (inter[0] + 3) % 4;
	  prop[0] = 1.;
	}
	nb_inter--;
      }
      if (nb_inter == 0
	  || (or != -10 && !est_int
	      && nb_inter == 1 && !est_coin)
	  || (!est_coin && inter[0] == (or + 1) % 4
	      && nb_inter == 1)
	  || (nb_inter > 0 && est_coin)){
	/*	if (nb_inter_prec > 0 && nb_inter == 0){
		PRINT_WAR_LOG(param_s, "ERREUR perte inter... !!!!!!!!!!\n");
		}*/

	/* on regarde si on peut ajuster
	 * (erreur possible : la tolerance s'accumule
	 *  et la courbe sort du ||gramme)
	 * => si aucun des points ne respecte les bornes,
	 *    on décale et fixe le or
	 */
	if (suiv_null)
	  break;
	ind_dec = ajuste_borne(ptprec->el, ptcur->el,
			       quadr->borne);
	if (ind_dec >= 0){
	  est_coin = 1;
	  nb_inter = 0;
	  break;
	}
	if (nb_inter > 0 && est_coin)
	  break;
	if(ptcur == c->fin)
	  app_quadr = 0;
	else
	  app_quadr = appartient_quadr(NULL, ptcur->n->el, 
				       quadr->borne);
	term_dec = 1;
	if (app_quadr != 0 || ((nb_suiv <= nb_rec 
				|| ptprec == c->l) 
			       && nb_suiv < nb_max)){
	  term_dec = 0;
	  if (!app_quadr)
	    nb_suiv++;
	  pttmp = ptcur;
	  ptcur = ptcur->n;

	  if(ptcur == NULL){
	    term_dec = 1;
	    /*  if(calc_inter_fin_courbe(s, cur, param_new, 
				     c, param_s)){
	      PRINT_WAR_LOG(param_s, "calc secteur : nb_passage : %d\n", 
			    dbg_nb_pass);
	      return 1;
	    }
	    else ptcur = pttmp->n;*/
	  }
	} else if (ptprec != c->l && nb_rec < nb_max){
	  ptprec = ptprec->p;
	  nb_rec++;
	  term_dec = 0;
	}
	if(term_dec) {
	  if (nb_inter == 0){
	    nb_inter = 1;
	    inter[0] = (or + 2)%4;
	    prop[0] = -1.;
	    est_coin = 1;
	  }
	  if ((prop[0] > _tol_petit && 1. - prop[0] > _tol_petit)){
	    PRINT_WAR_LOG(param_s, "erreur1 limite secteur %d\n", 
			  nb_quadr);
	    PRINT_WAR_LOG(param_s, "calc secteur : nb_passage : %d\n", 
			  dbg_nb_pass);
	    return 0;
	  }
	  else {
	    /* on est dans le cas pourri où on n'est pas
	     * tout à fait || mais presque
	     */
	    bool_pr_parall = 1;
	    break;
	  }
	}
      }
      else term = 1;
    }
    ptcur = ptcur2;
    
    if (nb_inter == 0 && suiv_null){
      quadr->passage = 0;
      break;
    }
    if (nb_inter > 1 && (inter[0] == (or + 1) % 4 ||
				(inter[1] != (or + 1) % 4
				 /*&& prop_prec == 1.*/ 
				 && inter[0] == or)
				|| pos_prec == inter[0])){
      or_prec2 = or_prec;
      or_prec = or;
      or = (inter[1] + 1) % 4;
      if (inter[0] != (or_prec + 1) % 4)
	est_coin = 1;
    }
    else if (nb_inter > 0){
      or_prec2 = or_prec;
      or_prec = or;
      or = (inter[0] + 1) % 4;
      if (nb_inter > 1 && inter[1] != (or_prec + 1) % 4)
	est_coin = 1;
    }
    /*
     * cas pourri où on n'est pas tout à fait || mais presque
     */
    if (nb_inter == 1 && !est_coin && bool_pr_parall
	/* && or == (or_prec+2)%4 */
	&& (prop[0] < _tol_petit || 1. - prop[0] < _tol_petit)){
      if(!prec_coin){
	nb_inter = 2;
	prop[1] = 0.5;
	if (prop[0] < 1. - prop[0]){
	  inter[1] = (inter[0] + 3) % 4;
	  or = (or_prec + 3) % 4;
	}
	else {
	  inter[1] = (inter[0] + 1) % 4;
	  or = (or_prec + 1) % 4;
	}
      } else {
	or_prec2 = or_prec;
	or_prec = or;
	or = (or+2)%2;
	nb_inter = 0;
      }
      prec_coin = 1;
    }
    if (ind_dec < 0)
      prec_dec = 0;
    if (nb_inter == 0){
      if(bool_pr_parall){
	quadr->dir_prop = or;
	quadr->aire_inter = 0.;
      } else if (ind_dec >= 0){
	or_prec2 = or_prec;
	or_prec = or;
	or = ind_dec;
	if (!bool_dej_passe){
	  if (est_int){
	    quadr->dir_prop = or;
	    quadr->aire_inter = 0.;
	  }
	  else{
	    if (quadr->passage == ind_c && (prec_dec)){
	      quadr->passage = 0;
	    }
	    else if (quadr->passage2 == ind_c)
	      quadr->passage2 = -1;
	  }
	}
	prec_dec = 1;
      }
      else {
	PRINT_WAR_LOG(param_s, "erreur2 limite secteur %d\n", nb_quadr);
	PRINT_WAR_LOG(param_s, "calc secteur : nb_passage : %d\n", dbg_nb_pass);
	return 0;
      }
    }
    else if (nb_inter == 1){
      if (or_prec == -10){
	/* il s'agit du 1er quadr*/
	quadr->aire_inter = 0.;
      }
      else if (est_int && ((prop_prec != 1. && prop_prec != 0.)
			   || or != (or_prec + 2) % 4)){
	/* le précédent est à l'intérieur */
	nb_inter++;
	est_coin = 0;
	if ((or_prec + 1) % 4 > inter[0]){
	  inter[1] = (or_prec + 1) % 4;
	  prop[1] = 1. - prop_prec;
	}
	else {
	  inter[1] = inter[0];
	  prop[1] = prop[0];
	  prop[0] = 1. - prop_prec;
	  inter[0] = (or_prec + 1) % 4;
	}
      }
      else {
	/* on viens d'un coin et on n'a pas pris la bonne direction */
	if (or_prec != inter[0] && prop[0] == 1.)
	  or = (inter[0] + 2) % 4;
	else {
	  if (or_prec != (inter[0] + 3) % 4)
	    or = (inter[0] + 1) % 4;
	  else
	    or = inter[0];
	}
	quadr->dir_prop = (inter[0] + 1) % 4;
	quadr->aire_inter = 0.;
	//quadr->passage = 0;
	prop_prec = 1.;
      }
    }

    if (est_coin)
      prec_coin = 1;
    else if (nb_inter > 0)
      prec_coin = 0;
    if (nb_inter == 2){
      if (or == (inter[0] + 1) % 4)
	prop_prec = prop[0];
      else
	prop_prec = prop[1];

      diff_pos = inter[1] - inter[0];
      if (diff_pos == 1 || diff_pos == 3){
	/* on est dans le cas d'un triangle */
	if (diff_pos == 1){
	  VECT_DEL(v1, ptquadr[inter[1]]->p,
		   ptquadr[(inter[1] + 1) % 4]->p, j);
	  VECT_DEL(v2, ptquadr[inter[1]]->p,
		   ptquadr[inter[0]]->p, j);
	  quadr->dir_prop = inter[1];
	  _prop = (1. - prop[0])*prop[1];
	}
	else {
	  VECT_DEL(v1, ptquadr[inter[0]]->p,
		   ptquadr[(inter[0] + 1) % 4]->p, j);
	  VECT_DEL(v2, ptquadr[inter[0]]->p,
		   ptquadr[inter[1]]->p, j);
	  quadr->dir_prop = inter[0];
	  _prop = (1. - prop[1])*prop[0];
	}
	prod_vect(pv, v1, v2);
	quadr->aire_inter = norme(pv)*_prop / 2.;
      }
      else {
	/* on a 2 quadrangles */
	quadr->dir_prop = _max(inter[0], inter[1]);
	quadr->aire_inter = quadr->aire*(1. - prop[0] + prop[1]) / 2.;
      }
      nb_quadr++;
    }

    if(est_coin && nb_inter == 2){
      /* on a une courbe qui est parallèle à un bord 
       * => on prend la direction qui est la plus proche du point suivant
       */
      int ind_tmp;
      if((inter[1]-inter[0]+4)%4 == 1 
	 || (inter[1]-inter[0]+4)%4 == 3){
	/* prop == 0 => on retourne le min
	 * prop == 1 => on retourne le max
	 */
	if(prop[0]<1.-prop[0]){
	  ind_tmp = _min(inter[0], inter[1]);
	  if(inter[1] == 3)
	    ind_tmp = 3;
	} else {
	  ind_tmp = _max(inter[0], inter[1]);
	  if(inter[0] == 0)
	    ind_tmp = 0;
	}
	
      } else ind_tmp = (inter[0]+1)%4;
      or = dir_suiv_mauv_inter(ptcur->el, quadr->borne, ind_tmp);
    }

    _prec = cur;
    if(ind_dec < 0){ 
      if(quadr->pt_dir != ptcur){
	quadr->pt_dir = ptcur;
	quadr->dirs = 0;
      }

      aj_or = 0;
      while(aj_or < 4 && (quadr->dirs 
			  % (2*aj_passage[(or+aj_or)%4]))
	    / aj_passage[(or+aj_or)%4] > 0)
	aj_or++;
      if(aj_or == 4){
	PRINT_WAR_LOG(param_s, "on a fait toutes les directions \
à partir de ce point => erreur du calcul \
des bords du secteur\n");
	return 0;
      } else
	or = (or+aj_or)%4;
      if(ind_dec < 0)
	quadr->dirs += aj_passage[or];
    } else {
      quadr->dirs = aj_tot-aj_passage[ind_dec];
    }
    cur = liste_getVois(cur, or);
    if(ind_dec >= 0 && cur != NULL){
      quadr = cur->el->el;
      if(quadr != NULL){
	if((quadr->dirs 
	    % (2*aj_passage[(ind_dec+2)%4]))
	 /aj_passage[(ind_dec+2)%4] == 0)
	  quadr->dirs += aj_passage[(ind_dec+2)%4];
      }
    }
    suiv_null = 0;
    if (cur == NULL){
      or_prec2 = or_prec;
      or_prec = or;
      ind_dec = ajuste_borne_dir(ptcur->p->el, ptcur->el,
				 quadr->borne, or);
      if (ind_dec == 0){
	or = (or + 2) % 4;
	cur = _prec;
	prec_coin = 1;
	suiv_null = 1;
	ind_dec = (or + 2) % 4;
      }
      else {
	ind_dec = -1;
	if (est_coin){
	  or = (or + 1) % 4;
	  ind_dec = 0;
	  cur = liste_getVois(_prec, or);
	  suiv_null = 1;
	}
      }
    }
    dbg_nb_pass++;
  }
  PRINT_WAR_LOG(param_s, "calc secteur : nb_passage : %d\n", dbg_nb_pass);
  return 1;
}

/* 
 * On arrive au dernier point 
 * => 2 cas : - on n'a pas atteint le dernier
 *              losange => on subdivise
 *            - on a atteint le dernier losange => terminé
 * retourne : 0 pour le 1er cas (pas terminé) 
 *         et 1 pour le 2ème cas (terminé)
 */
int calc_inter_fin_courbe(Surf_param *s, ListeD *mesh_cur,
			  Param_new2 *param_new, 
			  SListe *pts, Param_maillage *param_s){
  if(mesh_cur->nH == NULL || mesh_cur->pH == NULL
     || mesh_cur->pL == NULL || mesh_cur->nL == NULL){
    return 1;
  } else {
    double R;
    double H[100];
    int i, nb = 100;
    CourbeG cg;
    Pt new_pt[100];
    Pt *tmp;
    int nb_sub = 5;
    int nbcur = 0;

    VECT_DEL(cg.dir_init, ((Pt*)pts->fin->p->el)->p,
	     ((Pt*)pts->fin->el)->p,i);
    normer(cg.dir_init);
    cg.size = nb;
    cg.init = (Pt*) pts->fin->el;
    memset(cg.courbure, 0, SIZE_TAB*sizeof(double));
    R = norme(cg.dir_init)/((double)nb_sub);
    while(nbcur <= 1){
      for(i = 0;i<nb;i++)
	H[i] = R;
      cg.pas = R;
      nbcur = calc_courbeG(&cg, s, H, param_s, new_pt, nb);
      R /= (double)nb_sub;
    }
    for(i = 0;i<nbcur-1;i++){
      tmp = createPtN();
      ptcopy(tmp, new_pt+i+1);
      ajouter_fin_liste(pts, tmp);
    }
    return 0;
  }
}

/*
 * Calcul la courbure de Gauss d'un secteur délimité par les
 * 2 courbes c1 et c2
 * res : 2 valeurs : K^- et K^+ resp.
 * retourne 1 si le calcul est bon, 
 *          -1 si l'erreur vient de la 1ère courbe
 *          -2 si l'erreur vient de la 2ème courbe
 */
int calc_curv(Surf_param *s, Courbe *c1, Courbe *c2, 
	      SListe *lc1, SListe *lc2,	      
	      double *res, Mesh_curv **mesh,
	      Param_maillage *param, double *H,
	      PListeD init[2][10], int *nb_init,
	      int nbx, int nby){
  int i, j;
  Courbe *c;
  SListe *lc;
  Pt pts[SIZE_TAB];
  P3D _pts[SIZE_TAB], _params[SIZE_TAB];
  int nbptr = SIZE_TAB; /* on construit la géodésique jusqu'au bord */
  int nbp, dummy = 0, size_prec;
  Pt *addr_pt;
  //  double nb_add = 4.;
  double dist_min_sing = 0.02;
  double dist_sing_constr = 0.05;
  //  int bool, bool_aj[2][2];
  // int term;
  Liste* ltmp, *ltmp2, *lprec;
  int ind;

  if (mesh[0] == NULL)
    mesh[0] = create_mesh_curv(s, dist_sing_constr,
			       dist_min_sing, param, nbx, nby);
  for (i = 0; i < 2; i++){
    if (i == 0){
      c = c1;
      lc = lc1;
    }
    else{
      c = c2;
      lc = lc2;
    }
    size_prec = c->c_geod->size;
    c->c_geod->size = SIZE_TAB;
    for (j = size_prec; j < SIZE_TAB; j++)
      c->c_geod->courbure[j] = 0.;
    if (c->c_pts == NULL || c->c_pts->nb == -1){
      memset(init[i], 0, 10*sizeof(PListeD));
      //  term = 0;
      /*      PRINT_WAR_LOG(param, "avant while : %d    borne : %lf-->%lf     %lf-->%lf\n", 
	      i, s->s_param->init[0], s->s_param->fin[0], s->s_param->init[1], s->s_param->fin[1]);*/
      //   while (!term){
      /* si la surface n'est pas fermé, on repousse un peu le bord */
      /*	for (k = 0; k < 2; k++){
		if (k == 0)
		bool = (s->ind_quotient % 2 == 0);
		else
		bool = (s->ind_quotient / 2 == 0);
		bool_aj[k][0] = 0; bool_aj[k][1] = 0;
		if (bool){
		if (estNulle(mesh[0]->min_surf[k] - s->init[k])){
		s->init[k] -= nb_add* h[0];
		bool_aj[k][0] = 1;
		}
		if (estNulle(mesh[0]->max_surf[k] - s->fin[k])){
		s->fin[k] += nb_add*h[0];
		bool_aj[k][1] = 1;
		}
		}
		}*/
      nbp = discretiser_courbe(s, c, H, nbptr,
			       param, pts, &dummy);
      
      if (nbp == SIZE_TAB){
	PRINT_WAR_LOG(param, "ERREUR construction secteur : pas assez de points pour aller jusqu'au bord   h : %lf\n",H[0]);
	return -10;
      }
      if (c->c_pts == NULL){
	for (j = 0; j < nbp; j++){
	  VECT_CPY(_params[j], pts[j].param);
	  VECT_CPY(_pts[j], pts[j].p);
	}
	init_courbePts(c, _params, _pts, nbp);
	addr_pt = c->c_pts->pts;
      }
      else {
	c->type = 2;
	addr_pt = c->c_pts->pts;
	c->c_pts->nb = nbp;
	for (j = 0; j < nbp; j++){
	  ptcopy(addr_pt+j,pts+j);
	}
      }
      ltmp = lc->l;
      ind = 0;
      while(ltmp != NULL && ind < nbp){
	ltmp->el = addr_pt + ind;
	ind++;
	ltmp = ltmp->n;
      }
      if(ltmp != NULL){
	ltmp2 = ltmp->p;
	while(ltmp != NULL){
	  lprec = ltmp;
	  ltmp = ltmp->n;
	  free(lprec);
	}
	lc->fin = ltmp2;
	ltmp2->n = NULL;
      }
      for(j = ind;j<nbp;j++){
	ajouter_fin_liste(lc, addr_pt+j);
      }
   //      PRINT_WAR_LOG(param, "avant calc secteur i : %d\n", i);
      if (!calcul_secteur_courbe_geod(s, mesh[0], lc,
				      i+1, param, init[i], 
				      nb_init+i)){
	
	return -i;
      }
      /* TODO : décommenter */
      /*  ltmp = lc->l;
      ind = 0;
      while(ltmp->n != NULL){
	H[ind] = distance(((Pt*)ltmp->el)->p, ((Pt*)ltmp->n->el)->p);
	ind++;
	ltmp = ltmp->n;
	}*/
      //      PRINT_WAR_LOG(param, "après calc secteur i : %d\n", i);
    }
  }
  calc_curv_sub(mesh[0], init, nb_init, res, param);
  return 1;
}

El_constr_curv* create_el_constr_curv(ListeD *l, int dir,
				      int aj_int, int est_connexe){
  El_constr_curv *res = malloc(sizeof(El_constr_curv));
  res->l = l;
  res->dir_prec = dir;
  res->aj_int = aj_int;
  res->est_connexe = est_connexe;
  return res;
}

/*
 * Supprime le découpage réalisé :
 * 1 ---> ind
 * 2 ---> ind2
 */
void mesh_suppr_passage_curv(Mesh_curv *mesh, PListeD init[2][10],
			     int nb_init[2], Param_maillage *param,
			     int ind, int ind2){
  ListeD *cur;
  SListe *pile = NULL;
  Liste *l;
  int i, j;
  Quadr_mesh *quadr;
  ListeD *vois;

  pile = createSListe(init[0][0], NULL, NULL, NULL, NULL);
  for (i = 0; i < 2; i++){
    for (j = 0; j < nb_init[i]; j++){
      ajouter_fin_liste(pile, init[i][j]);
    }
  }
  l = pile->l;
  while (l != NULL){
    cur = l->el;
    quadr = cur->el->el;
    if (fabs(quadr->passage) > 0 
	&& (quadr->passage != ind || quadr->passage2 != -1)
	&& (quadr->passage != ind2 || quadr->passage2 != -1)){
      if (quadr->passage == 3){
	quadr->passage = 0;
	/*DBG*/ quadr->curv_aj = 0.;
      }
      else if ((abs(quadr->passage) == 1 && quadr->passage2 == -1)
	       || (quadr->passage2 != -1 && ind != 0)){
	quadr->passage = ind;
	if (ind == 0)
	  quadr->curv_aj = 0.;
      }
      else if (abs(quadr->passage) == 2 || quadr->passage2 != -1){
	quadr->passage = ind2;
	if (ind2 == 0)
	  quadr->curv_aj = 0.;
      }
      if (ind == 0 || ind2 == 0)
	quadr->passage2 = -1;
      for (i = 0; i < 4; i++){
	vois = liste_getVois(cur, i);
	if (vois != NULL){
	  quadr = vois->el->el;
	  if (fabs(quadr->passage) > 0 
	      && (quadr->passage != ind || quadr->passage2 != -1)
	      && (quadr->passage != ind2 || quadr->passage2 != -1)){
	    ajouter_fin_liste(pile, vois);
	  }
	}
      }
    }
    suppr_el_liste(pile, l);
    l = pile->l;
  }
}


void calc_curv_sub(Mesh_curv *mesh, PListeD init[2][10], int *nb_init,
		   double *res, Param_maillage *param){
  ListeD *cur;
  SListe *pile = NULL;
  Liste *l;
  Quadr_mesh *quadr, *quadr_tmp;
  PQuadr_mesh quadr_vois[3];
  PQuadr_mesh quadr_vois2[3];
  double aire;
  int i, j, k;
  El_constr_curv *tmp;
  El_constr_curv *new;
  PListeD vois[3];
  PListeD vois2[3];
  int dir2 = -1;
  int _dir, nb_dir_prop;
  int passage, nbj;
  int dir_vois[3];
  double aire_min = (double)INT_MAX;
  int bool_aj_int = 0, bool_aj_vois;
  int aj_int;
  ListeD *vois_tmp;

  // int nb_vois_0;
  int nb_aj = 0;

  for (i = 0; i < 2; i++){
    for (j = nb_init[i]; j < 10; j++){
      init[i][j] = NULL;
    }
  }
  for (j = 0; j < 2; j++){
    for (k = 0; k < nb_init[j]; k++){
      quadr_vois[0] = init[j][k]->el->el;
      quadr_vois[0]->passage = -quadr_vois[0]->passage;
      for (i = 0; i < 4; i++){
	cur = liste_getVois(init[j][k], i);
	if (cur != NULL){
	  quadr = cur->el->el;
	  if (quadr->passage != 0){
	    new = create_el_constr_curv(cur, (i + 2) % 4, 1, k == 0);
	    if (pile == NULL)
	      pile = createSListe(new, NULL, NULL, NULL, NULL);
	    else
	      ajouter_fin_liste(pile, new);
	  }
	}
      }
    }
  }
  res[0] = 0.; res[1] = 0.;
  if (pile == NULL)
    return;
  l = pile->l;
  while (l != NULL){
    tmp = l->el;
    cur = tmp->l;
    quadr = cur->el->el;
    dir2 = -1;
    passage = quadr->passage;
    if (passage != 3 && passage >= 0){
      nbj = 1;
      if (quadr->passage2 != -1)
	nbj = 2;
      aire_min = (double)INT_MAX;
      for (j = 0; j < nbj; j++){
	if (j == 0)
	  passage = quadr->passage;
	else
	  passage = quadr->passage2;
	if (quadr->bool_util){
	  if (passage != 0){
	    vois[0] = liste_getVois(cur, tmp->dir_prec);
	    quadr_vois[0] = vois[0]->el->el;
	    if (passage == 1)
	      dir2 = (tmp->dir_prec + 3) % 4;
	    else
	      dir2 = (tmp->dir_prec + 1) % 4;
	    if (abs(quadr_vois[0]->passage) == passage
		|| quadr_vois[0]->passage2 == passage){
	      /* on vient du bord*/
	      vois[1] = vois[0];
	      dir_vois[1] = tmp->dir_prec;
	      vois[0] = liste_getVois(cur, dir2);
	      dir_vois[0] = dir2;
	      vois[2] = liste_getVois(cur, tmp->dir_prec + 2);
	      dir_vois[2] = (tmp->dir_prec + 2) % 4;
	    }
	    else {
	      /* on vient de l'intérieur */
	      dir_vois[0] = tmp->dir_prec;
	      vois[1] = liste_getVois(cur, tmp->dir_prec + 3);
	      dir_vois[1] = (tmp->dir_prec + 3) % 4;
	      vois[2] = liste_getVois(cur, tmp->dir_prec + 1);
	      dir_vois[2] = (tmp->dir_prec + 1) % 4;
	    }
	    nb_dir_prop = 0;
	    for (i = 0; i < 3; i++){
	      if (vois[i] != NULL){
		quadr_vois[i] = vois[i]->el->el;
		if ((i == 0 || abs(quadr_vois[i]->passage) == passage
		     || quadr_vois[i]->passage2 == passage)
		    && (quadr->dir_prop == dir_vois[i]
			|| (quadr->dir_prop + 1) % 4 == dir_vois[i]))
		  nb_dir_prop++;
	      }
	      else if (quadr->dir_prop == dir_vois[i]
		       || (quadr->dir_prop + 1) % 4 == dir_vois[i])
		nb_dir_prop++;
	    }
	    if (nb_dir_prop >= 2)
	      aire = quadr->aire_inter;
	    else
	      aire = quadr->aire - quadr->aire_inter;
	    aire *= quadr->curv;
	  }
	  else {
	    aire = quadr->aire*quadr->curv;
	  }
	  aire_min = _fmin(aire, aire_min);
	  if (aire_min > 0)
	    res[1] += aire_min;
	  else
	    res[0] -= aire_min;
	}
	else aire_min = 0.;
	quadr->curv_aj = aire_min;
      }

      vois[1] = liste_getVois(cur, tmp->dir_prec);
      quadr_vois[1] = vois[1]->el->el;
      for (i = 0; i < 4; i++){
	aj_int = 1;
	vois[0] = liste_getVois(cur, i);
	if (vois[0] != NULL){
	  quadr_vois[0] = vois[0]->el->el;
	  bool_aj_vois = 0;
	  if (quadr_vois[0]->bool_util){
	    if (passage == 0 && quadr_vois[0]->passage == 0)
	      bool_aj_vois = 1;
	    else if (tmp->est_connexe && !bool_aj_int
		     && tmp->aj_int && i == dir2 &&
		     quadr_vois[1]->passage != 3
		     && quadr_vois[1]->bool_util
		     && quadr->passage2 == -1
		     && quadr_vois[0]->passage == 0){
	      /* on verifie que si on fait le même virage,
	       * on ne retombe pas sur la frontière
	       */
	      vois_tmp = liste_getVois(vois[0], tmp->dir_prec);
	      if (vois_tmp != NULL){
		quadr_tmp = vois_tmp->el->el;
		if (abs(quadr_tmp->passage) == 0){
		  vois_tmp = liste_getVois(cur, i+2);
		  if(vois_tmp != NULL){
		    quadr_tmp = vois_tmp->el->el;
		    if(quadr_tmp->passage >= 0){
		      /* on vérifie aussi que le quadrangle 
		       * n'a pas 3 voisins ayant passage == 0
		       */
		      vois2[0] = liste_getVois(cur, i+1);
		      vois2[1] = liste_getVois(vois[0], i+2);
		      vois2[2] = liste_getVois(vois[0], i+3);
		      if(vois2[0] == NULL || vois2[1] == NULL 
			 || vois2[2] == NULL)
			bool_aj_vois = 1;
		      else{
			for(k = 0;k<3;k++){
			  quadr_vois2[k] = vois2[k]->el->el;
			}
			if(quadr_vois2[0]->passage != 0
			   && quadr_vois2[2]->passage != 0)
			  bool_aj_vois = 1;
		      }
		    }
		  }
		}
	      }
	      // bool_aj_int = 1;
	    }
	    else if (passage > 0){
	      if (passage == 1)
		_dir = (i + 1) % 4;
	      else
		_dir = (i + 3) % 4;
	      vois[2] = liste_getVois(cur, _dir);
	      if (vois[2] != NULL){
		quadr_vois[2] = vois[2]->el->el;
		if (quadr_vois[2]->passage == 3)
		  bool_aj_vois = 1;
	      }
	    }
	  }
	  if (passage != 0 && quadr_vois[0]->passage >= 1
	      && quadr_vois[0]->passage < 3){
	    /* on regarde si le point courant n'a pas été
	     * obtenu par un changement de courbe
	     * => on n'ajoute pas le point dans cette direction
	     */
	    if (quadr->passage2 == -1
		&& (abs(quadr_vois[1]->passage) != passage
		    || quadr_vois[1]->passage2 != -1)){
	      aj_int = 0;
	      if (i != (dir2 + 2) % 4)
		aj_int = 1;
	    }
	    else
	      aj_int = 1;
	    bool_aj_vois = 1;
	  }
	  if (bool_aj_vois){
	    new = create_el_constr_curv(vois[0], (i + 2) % 4,
					aj_int, tmp->est_connexe);
	    ajouter_fin_liste(pile, new);
	  }
	}
      }
      if (passage > 0)
	quadr->passage = -quadr->passage;
      else
	quadr->passage = 3;
    }
    suppr_el_liste(pile, l);
    free(tmp);
    l = pile->l;
     nb_aj++;
  }
}

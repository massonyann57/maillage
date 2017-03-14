#include "curvature.h"

Mesh_curv* create_mesh_curv(Surf_param *s, 
			    double dist_bord_constr, 
			    double dist_sing_min,
			    Param_maillage *param_s){
  int i, j, k;
  int nbx = 150, nby = 150;
  int ind[4];
  P3D p1, p2, ptmp, mid;
  SListeD *sl = createSListeD(NULL, NULL, print_el_quadr, NULL,
			      NULL, NULL, NULL, 0, 0, -1);
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
  double del_x;
  double del_y;
  int quo_x = 0, quo_y = 0;
  if(s->ind_quotient%2 == 1)
    quo_x = 1;
  if(s->ind_quotient/2 == 1)
    quo_y = 1;

  init[0] = s->init[0]; init[1] = s->init[1];
  fin[0] = s->fin[0]; fin[1] = s->fin[1];

  for(i = 0;i<2;i++){
    if(i==0){
      tmp1 = init;
      tmp2 = fin;
    } else {
      tmp2 = init;
      tmp1 = fin;
      _del *= -1.;
    }
    for(j = 0;j<2;j++){
      par_tmp[j] = tmp1[j];
      par_tmp[(j+1)%2] = (tmp1[(j+1)%2] + tmp2[(j+1)%2])/2.;
      surf_calc_base(s, par_tmp, base, metr);
      if(estNulle(deter(metr, 2))){
	tmp1[j]+=_del;
	dec_borne[j][i] = 1;
      } else
	dec_borne[j][i] = 0;
    }
  }
  del_x = (fin[0]-init[0])/nbx;
  del_y = (fin[1]-init[1])/nby;
  res->mesh = sl;
  res->pts = create_gtab(NULL, NULL);

  p1[0] = init[0];
  p1[1] = init[1];
  p2[0] = p1[0];p2[1] = p1[1]+del_y;
  ind_prec = curv_aj_pt(s, p1, res->pts);
  ptmp[0] = p1[0];
  ptmp[1] = p1[1];
  i = 0;
  while(p1[0] - fin[0]<-EPS_TOL){
    p1[0] += del_x;
    p2[0] += del_x;
    p1[1] = init[1];
    p2[1] = init[1];

    ind[1] = ind_prec;
    ind[0] = curv_aj_pt(s, p1, res->pts);
    ind_prec = ind[0];
    j = 0;
    ind[2] = ind[1];
    while(p2[1]- fin[1]<-EPS_TOL){
      if(i==0){
	ptmp[1] += del_y;
	ind[2] = curv_aj_pt(s, ptmp, res->pts);
      } else {
	if(i==1)
	  ind[2]+=2;
	else
	  ind[2]++;
      }
      p2[1] = p1[1]+del_y;

      ind[3] = curv_aj_pt(s, p2, res->pts);
      
      mid[0] = p1[0]-del_x/2.;
      mid[1] = p1[1]+del_y/2.;
      curv = surf_get_curv(s, mid);
      bool_util = 1;
      for(k = 0;k<2;k++){
	if(dec_borne[k][1] && fin[k]-p2[k]<dist_bord_constr-EPS_TOL)
	  bool_util = 0;
	else if(dec_borne[k][0] && p2[k]- init[k]<dist_bord_constr-EPS_TOL)
	  bool_util = 0;
      }
      quadr = create_quadr_mesh(ind, res->pts, curv, bool_util);
      or = 3;
      vois = tmp;
      if(j==0){
	or = 0;
	vois = prem;
      }
      tmp = ajouter_listeD(sl, vois,
			   quadr, NULL, or, NULL, 0, 0,
			   0, -1, param_s);
      if(prem_x == NULL)
	prem_x = tmp;
      if(quo_x && i == nbx-1){
	ajouter_vois(sl, tmp, prem_x, 0, 2, param_s);
	prem_x = prem_x->nL;
      }
      ind[1] = ind[2];ind[0] = ind[3];
      p1[1] = p2[1];
      if(j == 0){
	prem = tmp;
      }
      j++;
    }
    if(quo_y){
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

Quadr_mesh* create_quadr_mesh(int ind[4], GTab *tab, 
			      double curv, int bool_util){
  int i, j;
  PPt pts[4];
  P3D pv, v1, v2;
  double *par;
  Quadr_mesh *res = malloc(sizeof(Quadr_mesh));
  res->bool_util = bool_util;
  res->aire = 0.;
  for(j = 0;j<2;j++){
    res->borne[j][0] = (double)INT_MAX;
    res->borne[j][1] = -res->borne[j][0];
  }
  for(i = 0;i<4;i++){
    res->pts[i] = ind[i];
    pts[i] = get_element_gtab(tab, ind[i]);
    par = pts[i]->param;
    for(j = 0;j<2;j++){
      if(par[j] < res->borne[j][0])
	res->borne[j][0] = par[j];
      if(par[j] > res->borne[j][1])
	res->borne[j][1] = par[j];
    }
  }
  for(i = 1;i<4;i+=2){
    VECT_DEL(v1, pts[i]->p, pts[i-1]->p, j);
    VECT_DEL(v2, pts[i]->p, pts[(i+1)%4]->p, j);
    prod_vect(pv, v1, v2);
    res->aire += norme(pv)/2.;
  }
  res->curv = curv;
  res->passage = 0;
  res->passage2 = -1;
  res->aire_inter = 0.;
  res->dir_prop = -1;

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
  
  if(guess == NULL)
    res = sl->l[0];

  while(res != NULL){
    prec = res;
    m = res->el->el;
    if(p->param[0] < m->borne[0][0]-EPS_TOL)
      res = res->pH;
    else if(p->param[0] > m->borne[0][1]+EPS_TOL)
      res = res->nH;
    if(res != NULL){
      if(p->param[1] < m->borne[1][0]-EPS_TOL)
	res = res->pL;
      else if(p->param[1] > m->borne[1][1]+EPS_TOL)
	res = res->nL;
    }
    if(res == prec){
      return res;
    }
  }
  return NULL;
}

int calcul_secteur_courbe_geod(Mesh_curv *mesh, CourbePts *c, 
				int ind_c, Param_maillage *param_s,
			       ListeD **inits, int *nb_init){
  int ind = 0;
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
  int ind_prec, pos_min = -1;
  int nb_rec, nb_suiv, nb_max = 30;
  int term2 = 0;
  double borne_tmp[2][2];
  int prec_coin = 0, est_coin;
  double suppr_prop[2];
  int suppr_inter[2];
  int nb_suppr_inter;
  int nb_inter_prec;

  for(i = 0;i<2;i++){
    borne_tmp[i][0] = mesh->min_surf[i];
    borne_tmp[i][1] = mesh->max_surf[i];
  }

  nb_init[0] = 1;
  inits[0] = curv_trouver_quadr(mesh, c->pts, NULL);
  cur = inits[0];
  ind++;
  while(!term2){
    if(cur == NULL){
      ind_prec = ind;
      if(inits[0] == NULL){
	ind_prec = ind-1;
	nb_init[0] = 0;
      }
      while(ind<c->nb && !est_dans_borne(c->pts+ind, borne_tmp))
	ind++;
      if(ind<c->nb && ind_prec < ind){
	/* le point était dans un trou 
	 * (une singularité du paramétrage */
	inits[nb_init[0]] = curv_trouver_quadr(mesh, c->pts+ind, NULL);
	cur = inits[nb_init[0]];
	nb_init[0]++;
	or = -10;
      } 
      else
	break;
    }
    quadr = cur->el->el;
    if(quadr->passage != 0){
      if(quadr->passage == ind_c){
	PRINT_WAR_LOG(param_s, "erreur0 secteur geod : %d\n", nb_quadr);
	return 0;
      }
      else
	quadr->passage2 = ind_c;
    } else
      quadr->passage = ind_c;
    term = 0;
    est_int = 0;
    while(!term){
      for(i = 0;i<4;i++)
	ptquadr[i] = get_element_gtab(mesh->pts, 
				      quadr->pts[i]);
      app_quadr = appartient_quadr(ptquadr, c->pts+ind, quadr->borne);
      term = (app_quadr == 0);
      if(!term){
	ind++;
	if(ind == c->nb)
	  break;
	est_int = 1;
      }
    }
    if(ind == c->nb)
      break;
    if(app_quadr < 0)
      app_quadr = abs(app_quadr+1);
    else
      app_quadr = -1;
    pos_prec = -1;
    nb_inter = 0;
    ind_prec = ind-1;
    term = 0;
    nb_rec = 0;
    nb_suiv = 0;
    pos_min = -1;
    while(!term){
      est_coin = 0;
      nb_inter_prec = nb_inter;
      nb_inter = intersection_segm_quadr(ptquadr, c->pts[ind_prec].p, 
					 c->pts[ind].p, inter, prop, 
					 &pos_prec, &pos_min);
      nb_suppr_inter = 0;
      for(i = 0;i<nb_inter;i++){
	if(estNulle(prop[i])){
	  suppr_inter[nb_suppr_inter] = inter[i];
	  suppr_prop[nb_suppr_inter] = prop[i];
	  nb_suppr_inter++;
	  for(j = i;j<nb_inter-1;j++){
	    inter[j] = inter[j+1];
	    prop[j] = prop[j+1];
	  }
	  nb_inter--;
	  i--;
	} else if(estNulle(1.-prop[i])){
	  prop[i] = 1.;
	  est_coin = 1;
	}
      }
      if(nb_inter == 0){
	for(i = 0;i<nb_suppr_inter;i++){
	  inter[i] = suppr_inter[i];
	  prop[i] = suppr_prop[i];
	  est_coin = 1;
	}
	nb_inter = nb_suppr_inter;
      }
      else if(nb_inter < 2 && !est_int && nb_suppr_inter > 0){
	for(i = 0;i<nb_suppr_inter;i++){
	  if(inter[0] != suppr_inter[i] 
	     && !((inter[0]+1)%4 == suppr_inter[i]/* && prop[0] == 1.*/)){
	    // on a supprimé un point qu'on n'aurait pas du...
	    est_coin = 1;
	    nb_inter++;
	    if(suppr_inter[i]>inter[0]){
	      inter[1] = suppr_inter[i];
	      prop[1] = suppr_prop[i];
	    } else {
	      inter[1] = inter[0];
	      prop[1] = prop[0];
	      prop[0] = suppr_prop[i];
	      inter[0] = suppr_inter[i];
	    }
	  }
	}
      }
      if(nb_inter_prec != 0 && nb_inter == 0)
	break;

      /*    if((or == -10 || est_int) && nb_inter == 2){
	inter[0] = inter[(pos_min+1)%2];
	prop[0] = prop[(pos_min+1)%2];
	nb_inter--;
	}*/
      if(nb_inter == 0 
	 || (or!=-10 && !est_int 
	     && nb_inter==1 && !est_coin)
	 /*|| (!prec_coin && !est_coin && nb_inter == 2 
	     && inter[0] != (or+1)%4 && inter[1] != (or+1)%4)*/){
	app_quadr = appartient_quadr(NULL, c->pts+ind+1, quadr->borne);
       	if(app_quadr != 0 || ((nb_suiv <= nb_rec || ind_prec==0) && nb_suiv < nb_max)){
	  if(!app_quadr)
	    nb_suiv++;
	  ind++;
	} else {
	  if(ind_prec > 0 && nb_rec < nb_max){
	    ind_prec--;
	    nb_rec++;
	  }
	  else{
	    PRINT_WAR_LOG(param_s, "erreur1 secteur geod : %d\n", nb_quadr);
	    return 0;
	  }
	}
      } else term = 1;
    }
    ind -= nb_suiv;
    if(nb_inter > 1 && (inter[0] == (or+1)%4 || 
			(inter[1] != (or+1)%4 
			 && prop[0] < prop[1] && inter[0] == or)
			|| pos_prec == inter[0])){
      or_prec = or;
      or = (inter[1]+1)%4;
    } else {
      or_prec = or;
      or = (inter[0]+1)%4;
    }
    prec_coin = 0;
    if(nb_inter == 0){	    
      PRINT_WAR_LOG(param_s, "erreur2 secteur geod : %d\n", nb_quadr);
      return 0;
    }
    else if(nb_inter == 1){
      if(or_prec == -10){
	/* il s'agit du 1er quadr*/
	quadr->aire_inter = 0.;
      } else if(est_int){
	/* le précédent est à l'intérieur */
	nb_inter++;
	if((or_prec+1)%4>inter[0]){
	  inter[1] = (or_prec+1)%4;
	  prop[1] = 1.-prop_prec;
	} else {
	  inter[1] = inter[0];
	  prop[1] = prop[0];
	  prop[0] = 1.-prop_prec;
	  inter[0] = (or_prec+1)%4;
	}
      } else {
	/* on viens d'un coin et on n'a pas pris la bonne direction */
	or = (or_prec+1)%4;
	quadr->dir_prop = (inter[0]+1)%4;
	quadr->aire_inter = 0.;
	prec_coin = 1;
	//quadr->passage = 0;
      }
    } 
    if(or == (inter[0]+1)%4)
      prop_prec = prop[0];
    else
      prop_prec = prop[1];

    if(nb_inter == 2){
      diff_pos = inter[1]-inter[0];
      if(diff_pos==1 || diff_pos==3){
	/* on est dans le cas d'un triangle */
	if(diff_pos == 1){
	  VECT_DEL(v1, ptquadr[inter[1]]->p, ptquadr[(inter[1]+1)%4]->p, j);
	  VECT_DEL(v2, ptquadr[inter[1]]->p, ptquadr[inter[0]]->p, j);
	  quadr->dir_prop = inter[1];
	  _prop = (1.-prop[0])*prop[1];
	} else {
	  VECT_DEL(v1, ptquadr[inter[0]]->p, ptquadr[(inter[0]+1)%4]->p, j);
	  VECT_DEL(v2, ptquadr[inter[0]]->p, ptquadr[inter[1]]->p, j);
	  quadr->dir_prop = inter[0];
	  _prop = (1.-prop[1])*prop[0];
	}
	prod_vect(pv, v1, v2);
	quadr->aire_inter = norme(pv)*_prop/2.;
      } else {
	/* on a 2 quadrangles */
	quadr->dir_prop = _max(inter[0],inter[1]);
	quadr->aire_inter = quadr->aire*(1.-prop[0]+prop[1])/2.;
      }
      nb_quadr++;
    }
    _prec = cur;
    cur = liste_getVois(cur, or);
    if(est_coin && cur == NULL){
      or = (or+1)%4;
      cur = liste_getVois(_prec, or);
      prec_coin = 1;
    }
  }
  return 1;
}

/*
 * Calcul la courbure de Gauss d'un secteur délimité par les 
 * 2 courbes c1 et c2
 * res : 2 valeurs : K^- et K^+ resp.
 */
int calc_curv(Surf *s, Courbe *c1, Courbe *c2, 
	      double *res, Mesh_curv **mesh, 
	      Param_maillage *param, double h, 
	      PListeD init[2][10], int *nb_init){
  int i, j, k;
  Courbe *c;
  Pt pts[SIZE_TAB];
  P3D _pts[SIZE_TAB], _params[SIZE_TAB];
  double H[SIZE_TAB];
  int nbptr = SIZE_TAB; /* on construit la géodésique jusqu'au bord */
  int nbp, dummy = 0, size_prec;
  Pt *addr_pt;
  double nb_add = 4.;
  double dist_min_sing = 0.05;
  double dist_sing_constr = 0.035;
  int bool, bool_aj[2][2];
  
  for (j = 0; j < nbptr; j++)
    H[j] = h;

  if(mesh[0] == NULL)
    mesh[0] = create_mesh_curv(s->s_param, dist_sing_constr,
			       dist_min_sing, param);
  for(i = 0;i<2;i++){
    if(i==0)  c = c1;
    else  c = c2;
    size_prec = c->c_geod->size;
    c->c_geod->size = SIZE_TAB;
    for(j = size_prec;j<SIZE_TAB;j++)
      c->c_geod->courbure[j] = 0.;
    if(c->c_pts == NULL || c->c_pts->nb == -1){
      memset(init[i], 0, 10*sizeof(PListeD));
      /* si la surface n'est pas fermé, on repousse un peu le bord */
      for(k = 0;k<2;k++){
	if(k == 0)
	  bool = (s->s_param->ind_quotient%2==0);
	else 
	  bool = (s->s_param->ind_quotient/2==0);
	bool_aj[k][0] = 0;bool_aj[k][1] = 0;
	if(bool){
	  if(mesh[0]->min_surf[k] == s->s_param->init[k]){
	    s->s_param->init[k] -= nb_add* h;
	    bool_aj[k][0] = 1;
	  }
	  if(mesh[0]->max_surf[k] == s->s_param->fin[k]){
	    s->s_param->fin[k] += nb_add*h;
	    bool_aj[k][1] = 1;
	  }
	}
      }
      nbp = discretiser_courbe(s, c, H, nbptr, 
			       param, pts, &dummy);
      for(k = 0;k<2;k++){
	if(bool_aj[k][0])
	  s->s_param->init[k] += nb_add*h;
	if(bool_aj[k][1])
	  s->s_param->fin[k] -= nb_add*h;
      }

      if(nbp == SIZE_TAB){
	PRINT_WAR_LOG(param, "ERREUR construction secteur : pas assez de points pour aller jusqu'au bord\n");
	return 0;
      }
      if(c->c_pts == NULL){
	for (j = 0; j < nbp; j++){
	  VECT_CPY(_params[j], pts[j].param);
	  VECT_CPY(_pts[j], pts[j].p);
	}
	init_courbePts(c, _params, _pts, nbp);
      } else {
	addr_pt = c->c_pts->pts;
	c->c_pts->nb = nbp;
	for (j = 0; j < nbp; j++){
	  VECT_CPY(addr_pt[j].param, pts[j].param);
	  VECT_CPY(addr_pt[j].p, pts[j].p);
	}

      }
      if(!calcul_secteur_courbe_geod(mesh[0], c->c_pts, 
				     i+1, param, init[i], nb_init+i))
	return 0;
    }
  }
  calc_curv_sub(mesh[0], init, nb_init, res, param);
  return 1;
}

El_constr_curv* create_el_constr_curv(ListeD *l, int dir){
  El_constr_curv *res = malloc(sizeof(El_constr_curv));
  res->l = l;
  res->dir_prec = dir;
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
  for(i = 0;i<2;i++){
    for(j = 0;j<nb_init[i];j++){
      ajouter_fin_liste(pile, init[i][j]);
    }
  }
  l = pile->l;
  while(l != NULL){
    cur = l->el;
    quadr = cur->el->el;
    if(quadr->passage == 3 || quadr->passage < 0){
      if(quadr->passage == 3){
	quadr->passage = 0;
	/*DBG*/ quadr->curv_aj = 0.;
      } else if(abs(quadr->passage) == 1){
	quadr->passage = ind;
	if(ind == 0)
	  quadr->curv_aj = 0.;
      } else if(abs(quadr->passage) == 2){
	quadr->passage = ind2;
	if(ind2 == 0)
	  quadr->curv_aj = 0.;
      }
      for(i = 0;i<4;i++){
	vois = liste_getVois(cur, i);
	if(vois != NULL){
	  quadr = vois->el->el;
	  if(quadr->passage != ind && quadr->passage != 0){
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
  Quadr_mesh *quadr;
  PQuadr_mesh quadr_vois[3];
  double aire;
  int i, j;
  El_constr_curv *tmp;
  El_constr_curv *new;
  PListeD vois[3];
  int dir2 = -1;
  int _dir, nb_dir_prop;
  int passage, nbj;
  int dir_vois[3];
  double aire_min = (double)INT_MAX;
  int bool_aj_int = 0, bool_aj_vois;
  int nb_aj_init[2] = {1,0};

  for(i = 0;i<2;i++){
    for(j = nb_init[i];j<10;j++){
      init[i][j] = NULL;
    }
  }
  for(j = 0;j<2;j++){
    for(i = 0;i<4;i++){
      cur = liste_getVois(init[j][0], i);
      if(cur != NULL){
	quadr = cur->el->el;
	if(quadr->passage != 0){
	  new = create_el_constr_curv(cur, (i+2)%4);
	  if(pile == NULL)
	    pile = createSListe(new, NULL, NULL, NULL, NULL);
	  else 
	    ajouter_fin_liste(pile, new);
	}
      }
    }
  }
  quadr = init[0][0]->el->el;
  quadr->passage = -quadr->passage;
  res[0] = 0.;res[1] = 0.;
  if(pile == NULL)
    return;
  l = pile->l;
  while(l != NULL){
    tmp = l->el;
    cur = tmp->l;
    quadr = cur->el->el;
    dir2 = -1;
    passage = quadr->passage;
    if(passage != 3 && passage >= 0){
      nbj = 1;
      if(quadr->passage2 != -1)
	nbj = 2;
      aire_min = (double)INT_MAX;
      for(j = 0;j<nbj;j++){
	if(j == 0)
	  passage = quadr->passage;
	else
	  passage = quadr->passage2;
	if(quadr->bool_util){
	  if(passage != 0){
	    vois[0] = liste_getVois(cur, tmp->dir_prec);
	    quadr_vois[0] = vois[0]->el->el;
	    if(passage == 1)
	      dir2 = (tmp->dir_prec+3)%4;
	    else
	      dir2 = (tmp->dir_prec+1)%4;
	    if(abs(quadr_vois[0]->passage) == passage 
	       || quadr_vois[0]->passage2 == passage){
	      /* on vient du bord*/
	      vois[1] = vois[0];
	      dir_vois[1] = tmp->dir_prec;
	      vois[0] = liste_getVois(cur, dir2);
	      dir_vois[0] = dir2;
	      vois[2] = liste_getVois(cur, tmp->dir_prec+2);
	      dir_vois[2] = (tmp->dir_prec+2)%4;
	    } else {
	      /* on vient de l'intérieur */
	      dir_vois[0] = tmp->dir_prec;
	      vois[1] = liste_getVois(cur, tmp->dir_prec+3);
	      dir_vois[1] = (tmp->dir_prec+3)%4;
	      vois[2] = liste_getVois(cur, tmp->dir_prec+1);
	      dir_vois[2] = (tmp->dir_prec+1)%4;
	    }
	    nb_dir_prop = 0;
	    for(i = 0;i<3;i++){
	      if(vois[i] != NULL){
		quadr_vois[i] = vois[i]->el->el;
		if((i == 0 || abs(quadr_vois[i]->passage) == passage 
		    || quadr_vois[i]->passage2 == passage)
		   && (quadr->dir_prop == dir_vois[i]
		       || (quadr->dir_prop+1)%4 == dir_vois[i]))
		  nb_dir_prop++;
	      } else if(quadr->dir_prop == dir_vois[i]
			|| (quadr->dir_prop+1)%4 == dir_vois[i])
		nb_dir_prop++;
	    } 
	    if(nb_dir_prop >= 2)
	      aire = quadr->aire_inter;
	    else
	      aire = quadr->aire - quadr->aire_inter;
	    aire *= quadr->curv;
	  } else {
	    aire = quadr->aire*quadr->curv;
	  }
	  aire_min = _fmin(aire, aire_min);
	  if(aire_min>0)
	    res[1] += aire_min;
	  else
	    res[0] -= aire_min;
	} else aire_min = 0.;
	quadr->curv_aj = aire_min;
      }

      vois[1] = liste_getVois(cur, tmp->dir_prec);
      quadr_vois[1] = vois[1]->el->el;
      for(i = 0;i<4;i++){
	vois[0] = liste_getVois(cur, i);
	if(vois[0] != NULL){
	  quadr_vois[0] = vois[0]->el->el;
	  bool_aj_vois = 0;
	  if(quadr_vois[0]->bool_util){
	    if(passage == 0 && quadr_vois[0]->passage == 0)
	      bool_aj_vois = 1;
	    else if(!bool_aj_int && i == dir2 && 
		    quadr_vois[1]->passage != 3 && quadr_vois[1]->bool_util
		    && quadr->passage2 == -1
		    && quadr_vois[0]->passage == 0){
	      bool_aj_vois = 1;
	      // bool_aj_int = 1;
	    } else if(passage>0){
	      if(passage == 1)
		_dir = (i+1)%4;
	      else
		_dir = (i+3)%4;
	      vois[2] = liste_getVois(cur, _dir);
	      if(vois[2] != NULL){
		quadr_vois[2] = vois[2]->el->el;
		if(quadr_vois[2]->passage == 3)
		  bool_aj_vois = 1;
	      }
	    }
	  }
	  if(passage != 0 && quadr_vois[0]->passage >= 1 
	     && quadr_vois[0]->passage < 3){
	    bool_aj_vois = 1;	    
	  }
	  if(vois[0] == init[0][nb_aj_init[0]]){
	    nb_aj_init[0]++;
	    bool_aj_vois = 1;
	  } else if(vois[0] == init[1][nb_aj_init[1]]){
	    nb_aj_init[1]++;
	    bool_aj_vois = 1;
	  }
	  if(bool_aj_vois){
	    new = create_el_constr_curv(vois[0], (i+2)%4);
	    ajouter_fin_liste(pile, new);
	  }
	}
      }
      if(passage > 0)
	quadr->passage = -quadr->passage;
      else
	quadr->passage = 3;
    }
    suppr_el_liste(pile, l);
    free(tmp);
    l = pile->l;
  }
}

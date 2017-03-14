#include "init_main.h"

Surf* init_surf(int type_surf, Pt *init, double *v_init, 
		Error_type *err){
  double init_u, fin_u;
  double init_v, fin_v;
  if(type_surf == 0){
    init->p[0] = 1.0; init->p[1] = 0.0; init->p[2] = 0.0;
    v_init[0] = 0.0;  v_init[1] = 1.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0, 0,
		       create_champ_goursat());
  }
  else if(type_surf == 1){
    init->p[0] = 1; init->p[1] = 0; init->p[2] = 0;
    v_init[0] = 0.0;  v_init[1] = 1.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_sphere());
  }
  else if(type_surf == 2){
    init->p[0] = 1;
    init->p[1] = 0/*sqrt((double)TMP_BOULE)*/; init->p[2] = 0;
    v_init[0] = 0;  v_init[1] = 1.0; v_init[2] = 0;
	
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_sphere());
  }
  else if(type_surf == 3){
    init->p[0] = TORE_R2-TORE_R1; init->p[1] = 0; init->p[2] = 0;
    v_init[0] = 0.0;  v_init[1] = 1.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_tore());
  }
  else if(type_surf == 4){
    init->p[0] = 1; init->p[1] = 0; init->p[2] = 0;
    v_init[0] = 0.0;  v_init[1] = 0.0; v_init[2] = 1.0;
  
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_hyperb());
  }
  else if(type_surf == 5){
    init->p[0] = 0; init->p[1] = 0; init->p[2] = 0.1;
    v_init[0] = 1.0;  v_init[1] = 0.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_tore2T());
  }
  else if(type_surf == 6){
    double coeff1 = COEFF_BOULE_ENF1;
	
    init->p[0] = 0; init->p[1] = 0;
    init->p[2] = sqrt(1-coeff1*(1+tanh(3)));
    v_init[0] = 1.0;  v_init[1] = 0.0; v_init[2] = 0.0;  
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_boule_enf());
  }
  else if(type_surf == 7){
    init->p[0] = 0; init->p[1] = 0;init->p[2] = 0;
    v_init[0] = 1.;  v_init[1] = 0.; 
    v_init[2] = 0.0;
  
    init->param[0] = 0.0;
    init->param[1] = 0.;
    init_u = -15.;
    fin_u = 15.;
    init_v = -15.;
    fin_v = 15.;
    return create_surf(1, create_champ_boite_oeuf_param(),
		       init_u, fin_u, init_v, fin_v, 0, NULL);
  }
  else if(type_surf == 8){
    init->p[0] = 0; init->p[1] = 0;init->p[2] = 0;
    v_init[0] = 1.0/sqrt(2.0);  v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_conoide());
  }
  else if(type_surf == 9){
    init->p[0] = 0; init->p[1] = 0;init->p[2] = 0;
    v_init[0] = 1.0;  v_init[1] = 0.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,0,
		       create_champ_plan());
  }
  else if(type_surf == 10){
    P3D R;
    R[0] = 1.0;R[1] = 1.0;R[2] = 1.0;

    init->p[0] = 0.; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 1.0/sqrt(2.0);
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -PI/2.0;
    fin_u = PI/2.;
    init_v = -PI/2.;
    fin_v = PI/2.;
  
    return create_surf(1, create_champ_sphere_param(R),
		       init_u, fin_u, init_v, fin_v, 0, NULL);
  }
#ifndef _WINDOWS
  else if(type_surf == 11){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 1.0/sqrt(2.0); v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 0.0;
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -5.0;
    fin_u = 2.0;
    init_v = -3.0;
    fin_v = 2.0;
  
    return create_surf(1, create_champ_trinoid_param(), 
		       init_u, fin_u, init_v, fin_v, 0, NULL);
  }
#endif
  else if(type_surf == 12){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = -1.0; v_init[1] = 0.0; v_init[2] = 0.0;
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -2.0;
    fin_u = 2.0;
    init_v = -2.0;
    fin_v = 2.0;
  
    return create_surf(1, create_champ_plan_param(), 
		       init_u, fin_u, init_v, fin_v, 0, NULL);
  }
  else if(type_surf == 13){
    int n = 3;
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = -1.0; v_init[2] = 0.0;
    init->param[0] = 0.01;
    init->param[1] = 0.;
    init_u = 0.0;
    fin_u = 1.0;
    init_v = -PI;
    fin_v = PI;
  
    return create_surf(1, create_champ_enneper_param(n), 
		       init_u, fin_u, init_v, fin_v, 2, NULL);
  }
  else if(type_surf == 14){
    int n = 4;
    double _d = 0.2;

    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 1.0; v_init[1] = 0.0; v_init[2] = 0.0;
    init->param[0] = 0.5;
    init->param[1] = 0.0;
    init_u = 0.001;
    fin_u = 1.;
    init_v = 0.0;
    fin_v = 2*PI/n;
  
    return create_surf(1, create_champ_tmp_param(n, _d), 
		       init_u, fin_u, init_v, fin_v, 0, NULL);
  }
  else if(type_surf == 15){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 0.; 
    v_init[2] = 1.0;
    init->param[0] = -0.5;
    init->param[1] = 0.6;
    init_u = -10.0;
    fin_u = 10.0;
    init_v = -PI;
    fin_v = PI;

    return create_surf(1, create_champ_catenoid_param(), 
		       init_u, fin_u, init_v, fin_v, 2, NULL);
  }
  else if(type_surf == 16){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 1.0/sqrt(2.0);
    init->param[0] = -PI/2.+0.01;
    init->param[1] = 0.0;
    init_u = -PI/2.;
    fin_u = 2.;
    init_v = -PI;
    fin_v = PI;

    return create_surf(1, create_champ_catenoid2_param(),
		       init_u, fin_u, init_v, fin_v, 2, NULL);
  }
  else if(type_surf == 17){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0; 
    v_init[2] = 0.0;
    init->param[0] = 1.0;
    init->param[1] = 0.0;
    init_u = 0.0000;
    fin_u = 1000;
    init_v = -2.0*PI;
    fin_v = 2.0*PI;

    return create_surf(1, create_champ_plan_param2(), 
		       init_u, fin_u, init_v, fin_v, 1, NULL);
  }
  else if(type_surf == 18){
    double c1 = 1.0;
    double c2 = 0.0; // pour avoir une surface de goursat : c2 = 2

    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0; 
    v_init[2] = 0.0;
    init->param[0] = 0.;
    init->param[1] = 0.;
    init_u = -PI/2.0;
    fin_u = PI/2.0;
    init_v = -PI;
    fin_v = PI;

    return create_surf(1, create_champ_cube_param(c1, c2), 
		       init_u, fin_u, init_v, fin_v, 1, NULL);
  }
  else if(type_surf == 19){
    double* c1 = malloc(sizeof(double));
    Fonc_r *f_tore;
    c1[0] = 1.5;
    f_tore = create_fonc_r(r_ext_tore, r_ext_tore_p, c1);

    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0; 
    v_init[2] = 0.0;
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -PI;
    fin_u = PI;
    init_v = -0.4;
    fin_v = 2*PI;

    return create_surf(1, create_champ_tore_param(c1[0], f_tore),
		       init_u, fin_u, init_v, fin_v, 0, NULL);
  }
  else if(type_surf == 20){
    double R1 = 0.5, R2 = 0.5; 
    int n = 7;
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 1.0;v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 1.0/sqrt(2.0);
    init->param[0] = -PI/2.+0.01;
    init->param[1] = 0.0;
    init_u = -PI/2.;
    fin_u = 3.;
    init_v = -PI;
    fin_v = PI;
    return create_surf(1, create_champ_enneper2_param(R1, R2, n),
		       init_u, fin_u, init_v, fin_v, 2, NULL);
  }
  else{
    if(err != NULL)
      err[0] = WRONG_PARAM_USER;
    return NULL;
  }
}

int discretiser_courbe(Surf_param *s, Courbe *c, double *h, int nb,
		       Param_maillage *param, Pt *res, 
		       int *bool_fermer){
  if(c->type == 0){
    //paramétrée
    if (bool_fermer[0]){
      int tmp = calc_courbeP_ferme(nb, c->c_param, NULL,
				   h, s, param, res,
				   NULL, -1);
      if(tmp<0)
	bool_fermer[0] = 0;
      return abs(tmp);
    }
    else{
      return calc_courbeP_surf(nb, c->c_param, NULL,
			       h, s, param, res,
			       NULL, -1);
    }
  } else if(c->type == 1){
    //géod
    return calc_courbeG(c->c_geod, s, h, param, res, nb);
  } else if(c->type == 2){
    //donnée par des pts
    //   nb = _min(nb, c->c_pts->nb);
    return calc_courbePts(s, c->c_pts, param, h, nb, res);
  }
  return -1;
}

/*
 * Crée un maillage à partir de 2 courbes du primal
 * bool_courbe : 0 -> courbe géodésique
 *               1 -> courbe entrée (par Rhino)
 */
Maillage* init_courbes_primales(Surf_param *s, Pt *c1, Pt *c2, 
				int nbptr1, int nbptr2, double h, 
				Param_maillage *param){
  int i;
  PPt _c1[SIZE_TAB], _c2[SIZE_TAB];
  Maillage *m;
  int or = 3;
  int ind_constr = 0;

  for(i = 0;i<nbptr1;i++){
    _c1[i] = createPtN();
    ptcopy(_c1[i], c1+i);
  }
  for(i = 0;i<nbptr2;i++){
    _c2[i] = createPtN();
    ptcopy(_c2[i], c2+i);
  }
  m = createMaillage(_c1[0], h, s);
  ajoute_courbe_maill(m, _c1, nbptr1, 1,
		      m->lignes->l[0], or%4, -1,
		      param->tol_pas, param, ind_constr);
  ajoute_courbe_maill(m, _c2, nbptr2, 1,
		      m->lignes->l[0], (or+1)%4, -1,
			  param->tol_pas, param, ind_constr);
  maillage_che(m, param, 0, NULL, 0, ind_constr);
  return m;
}

Maillage* ajouter_courbe_primal_maillage(Surf_param *s, Pt *c1, 
					 Pt *c2, int nbptr1, 
					 int nbptr2, 
					 double h, 
					 Param_maillage *param, int ind_constr){
  int i;
  PPt _c1[SIZE_TAB], _c2[SIZE_TAB];
  Maillage *m;
  int or = 3;

  m = createMaillage(c1, h, s);

  for(i = 0;i<nbptr1;i++){
    _c1[i] = createPtN();
    ptcopy(_c1[i], c1+i);
  }
  for(i = 0;i<nbptr2;i++){
    _c2[i] = createPtN();
    ptcopy(_c2[i], c2+i);
  }
  ajoute_courbe_maill(m, _c1, nbptr1, 1,
		      m->lignes->l[0], or%4, -1,
		      param->tol_pas,param, ind_constr);
  ajoute_courbe_maill(m, _c2, nbptr2, 1,
		      m->lignes->l[0], (or+1)%4, -1,
		      param->tol_pas,param, ind_constr);
  maillage_che(m, param, 0, NULL, 0, ind_constr);
  return m;
}


Maillage* create_sing_dual(Surf_param *s, TabPT *c, 
			   int *nbpts, int nbc,
			   double pas, Param_maillage *param,
			   int fermer_sing, double _eps, int bool_algo){
  int i, j;
  static TabPPT c_new[NB_BRANCHES_MAX];
  Maillage *m;
  int or = 2;
  int or_vir = 1;
  int ind_constr = 0;
  Pt *init = createPtN();
  PPPt _c_new[NB_BRANCHES_MAX];

  ptcopy(init, &c[0][0]);
  m = createMaillage(init, pas, s);
  if (nbc > 1){
    if (bool_algo){
      PPt _c[NB_BRANCHES_MAX];

      double eps = _eps*PI/180.0;
      PRINT_WAR_LOG(param, "test algo : nbc : %d\n", nbc);
      for (i = 0; i < nbc; i++){
	_c[i] = c[i];
	_c_new[i] = c_new[i];
      }
      jonction_dual_pts(m, nbc, _c, _c_new,
			nbpts, param, or, or_vir, 
			eps, fermer_sing, ind_constr);
    }
    else{
      int or_base = 1;
      int or_voistopt = (or + 2) % 4;
      ListeD *linit = m->lignes->l[0];

      maillage_set_sing(m, linit);
      for (i = 0; i < nbc; i++){
	_c_new[i] = c_new[i];
	for (j = 0; j < nbpts[i]; j++){
	  c_new[i][j] = createPtN();
	  ptcopy(c_new[i][j], &c[i][j]);
	}
      }
      ajoute_diag_maill(m, _c_new, nbc, nbpts, linit, 0,
			or_vir, or_voistopt, param,
			or_base, 3, NULL, NULL, fermer_sing, ind_constr);
    }
  }
  return m;
}

/* On supprime du maillage toute les construction 
 * faite après ind
 */
void free_constr(Maillage *m, int ind, Param_maillage *param){
  GTab *tab = m->lignes->mailles[0];
  int nb = tab->nb_tot;
  int i;
  ListeD *l;
  Ptmaill *ptm;
  Segment *seg;
  void *dum = m->seg->bool_suppr[0];
  for(i = 0;i<nb;i++){
    l = get_element_gtab(tab, i);
    if(l != NULL){
      ptm = l->el->el;
      if(ptm->p->ind_constr>=ind){
	suppr_point(l, m, param,0);
	if(dum != m->seg->bool_suppr[0])
	  printf("pb!!!! i : %d\n", i);
      }
    }
  }
  tab = m->seg;
  nb = tab->nb_tot;
  for (i = 0; i<nb; i++){
	  seg = get_element_gtab(tab, i);
    if(seg != NULL){
      seg->b_new = 1;
    }
  }
  update_sing(m);
  PRINT_WAR_LOG(param, "après ajustement sing\n");
  /* TODO : supprimer tous les sauts devenus inutile... */
  if(m->pts_dual != NULL)
    free_dual(m);
}

void update_sing(Maillage *m){
  int i, j;
  ListeD *sing;
  Ptmaill *ptm;
  SListeD *pts = m->lignes;
  int ind_seg;
  Segment *seg;
  for(i = 0;i<pts->nb_sing[0];i++){
    /* on ajuste le nombre de voisins */
    sing = pts->sing[0][i];
    ptm = sing->el->el;
    j = 0;
    while(j<sing->vois->nb_vois && sing->vois->vois[j] != NULL)
      j++;
    
    ind_seg = ptmaill_get_ind_seg(ptm, 0);
    seg = get_element_gtab(m->seg, ind_seg);
    if((j<sing->vois->nb_vois || seg->q2 == NULL) && sing->vois->est_ferme){
      sing->vois->est_ferme = 0;
      ptm->sing->est_ferme = 0;
    }
      
    if(!sing->vois->est_ferme){
      if(sing->vois->vois[0]->saut != NULL)
	free_saut_sing(m, sing, 0);
      else if(sing->vois->vois[j-1]->saut != NULL)
	free_saut_sing(m, sing, j-1);
    }
    if(j<4 && !sing->vois->est_ferme){
      /* on supprime la singularité */
      maillage_suppr_sing(m, sing);
    } else {
      sing->vois->nb_vois = j;
      ptm->sing->nb_seg = j;
    }
  }
}

/*__inline */  double r_ext_tore(double t, void *param){
  double r_int = ((double*)param)[0];
  return r_int / 2 + cos(t)*r_int / 6;
  //return 1;
}
/*__inline */  double r_ext_tore_p(double t, void *param){
  double r_int = ((double*)param)[0];
  return -sin(t)*r_int / 6;
  // return 0;
}

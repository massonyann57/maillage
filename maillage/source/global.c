#include "global.h"

PMaillage save[NB_SAVE_MAX];
int nb_maillage_saved = 0;

DLLEXP void _CDECL exp_free_memory(P3D *pts, int *quadr,
				   int *nbpts_quadr){
  free(pts);
  free(quadr);
  free(nbpts_quadr);
}

DLLEXP void _CDECL exp_free_maillage(Maillage *m){
  free_surface_param(m->s);
  free_maillage(m, NULL);
}

/* Cas où il s'agit des lignes paramétrage : f=NULL et f_p=NULL*/
DLLEXP CourbeP*
_CDECL exp_createCourbe_param(void(*f)(double, void*, P3D),
			      void(*f_p)(double, void*, P3D),
			      void *param_f, double init,
			      double fin, int est_per,
			      double pos_init, int dir){
  P3D tmp = {0.,0.};
  Fonc_vect *fonc = create_fonction_vect(f, f_p, param_f);
  CourbeP *c = create_courbeP(fonc, init, fin, est_per, 
			      pos_init, dir, tmp);
  return c;
}

DLLEXP Courbe*
_CDECL exp_createCourbe(P3D *param, P3D *pts, int nbpts,
			CourbeP *c_param, double p_init,
			double *param_init, double *dir_init,
			double *the, double pas){
  Courbe *res = create_courbe(-1, NULL, NULL, NULL);
  res->type = -1;

  if (pts != NULL){
    /* on est dans le cas où l'utilisateur rentre des points */
    init_courbePts(res, param, pts, nbpts);
  }
  else if (param_init != NULL && dir_init != NULL){
    /* on est dans le cas où l'utilisateur rentre une géodésique */
    Pt *init = createPtN();
    VECT_CPY(init->param, param_init);
    init_courbe_geod(res, dir_init, init, pas, nbpts, the);
  }
  else if (c_param != NULL){
    /* on est dans le cas où l'utilisateur rentre
     * une courbe paramétrée */
    res->c_param = c_param;
    res->type = 0;
  }
  else {
    return NULL;
  }
  return res;
}


DLLEXP Surf*
_CDECL exp_createSurface(void(*f)(P3D, void*, P3D),
			 void(*f_p)(P3D, void*, P3D*), double (*f_gauss)(P3D, void*),
			 void *param_f, double init1, double fin1,
			 double init2, double fin2, int ind_quotient){
  Champ_vect *champ = create_champ_vect(f, f_p, NULL, NULL, param_f);
  Surf *s = create_surf(1, champ, init1, fin1, init2, fin2,
			ind_quotient, NULL);
  s->s_param->gauss = create_champ_r(f_gauss, NULL, param_f);
  return s;
}

DLLEXP Surf*
_CDECL exp_createSurface_exemple(int type, P3D param_init, P3D vect){
  Pt init;
  Surf *res = init_surf(type, &init, vect, NULL);
  VECT_CPY(param_init, init.param);
  return res;
}

DLLEXP Maillage* _CDECL
exp_maillage_courbes_primales(Surf *s, Courbe *c1, Courbe *c2,
			      double h, int nb1, int nb2,
			      double tol, char *path_filelog,
			      int *ind_maill){
  Pt pts1[SIZE_TAB], pts2[SIZE_TAB];
  double H[SIZE_TAB];
  int nbm = _max(nb1, nb2);
  int i;
  Maillage *m = NULL;
  int dummy = 0;
  ListeD *init;
  Ptmaill *ptm;

  Param_maillage *param;
#ifdef _DBG
  char name[1000] = "courbes_prim.log";
  char _path_filelog[SIZE_TAB];
  if (path_filelog == NULL){
    get_path_filelog(_path_filelog, name);
    path_filelog = _path_filelog;
  }
#endif
  param = create_param_maillage(0.0, path_filelog,
				tol);
  if (ind_maill != NULL)
    ind_maill[0] = -1;
  for (i = 0; i < nbm; i++){
    H[i] = h;
  }
  nb1 = discretiser_courbe(s->s_param, c1, H, nb1, 
			   param, pts1, &dummy);
  if (nb1 > 0){
    nb2 = discretiser_courbe(s->s_param, c2, H, nb2, 
			     param, pts2, &dummy);
    if (nb2 > 0){
      m = init_courbes_primales(s->s_param, pts1, pts2, 
				nb1, nb2, h, param);
      init = m->lignes->l[0];
      if (init != NULL){
	ptm = init->el->el;
	ptm->p->est_bord_sector = 1;
      }
      if (ind_maill != NULL)
	ind_maill[0] = ajouter_maillage(m, param);
    }
  }
  if (param != NULL)
    free_param_maillage(param);
  return m;
}

DLLEXP Maillage* _CDECL
exp_maillage_sing(Surf *s, PCourbe *c, int *nbpts, double **H,
		  int nbc, double pas,
		  double tol, char *path_filelog, 
		  double eps, int *ind_maill){
  TabPT pts[NB_BRANCHES_MAX];
  double _H[SIZE_TAB];
  int i, j, nbm = 0;
  Maillage *m = NULL;
  double pas_discr = pas;
  int fermer_sing = 1;
  Param_maillage *param;
  int dummy = 0;
  double *p_H = _H;

#ifdef _DBG
  char name[1000] = "sing.log";
  char _path_filelog[SIZE_TAB];
  if (path_filelog == NULL){
    get_path_filelog(_path_filelog, name);
    path_filelog = _path_filelog;
  }
#endif

  param = create_param_maillage(0.0, path_filelog,
				tol);
  if (ind_maill != NULL)
    ind_maill[0] = -1;
  for (i = 0; i < nbc; i++){
    nbm = _max(nbpts[i], nbm);
  }
  if (H == NULL){
    for (i = 0; i < nbm; i++){
      _H[i] = pas_discr;
    }
  }
  for (i = 0; i < nbc; i++){
    if (c[i]->type != 2){
      if (H != NULL)
	p_H = H[i];
      nbpts[i] = discretiser_courbe(s->s_param, c[i], p_H, nbpts[i],
				    param, pts[i], &dummy);
      if (nbpts[i] <= 0){
	if (ind_maill != NULL)
	  ind_maill[0] = -1;
	return NULL;
      }
    }
    else {
      nbpts[i] = _min(nbpts[i], c[i]->c_pts->nb);
      for (j = 0; j < nbpts[i]; j++){
	pts[i][j] = c[i]->c_pts->pts[j];
      }
    }
  }
  m = create_sing_dual(s->s_param, pts, nbpts, nbc, pas,
		       param, fermer_sing, eps, H == NULL);
  if (ind_maill != NULL){
    ind_maill[0] = ajouter_maillage(m, param);
  }
  free_param_maillage(param);
  return m;
}

DLLEXP Maillage* _CDECL
exp_maillage_primal_dual(Surf *s, Courbe *c1, 
			 Courbe *c2, int bool_swi,
			 double h, int nb1, double tol,
			 char *path_filelog, int *ind_maill){
  Pt pts[SIZE_TAB];
  PPt _pts[SIZE_TAB];
  double H[SIZE_TAB];
  int i, or = 3;
  Maillage *m = NULL;
  int or_vir = 1, type_maill = 3;
  Param_maillage *param;
  int dummy = 0;

  if (!bool_swi)
    or_vir = 0;

#ifdef _DBG
  char name[1000] = "primal_dual.log";
  char _path_filelog[SIZE_TAB];
  if (path_filelog == NULL){
    get_path_filelog(_path_filelog, name);
    path_filelog = _path_filelog;
  }
#endif

  param = create_param_maillage(0.0, path_filelog,
				tol);
  for (i = 0; i < nb1; i++)
    H[i] = h;
  nb1 = discretiser_courbe(s->s_param, c1, H, nb1, 
			   param, pts, &dummy);
  if (nb1 <= 0){
    if (ind_maill != NULL)
      ind_maill[0] = -1;
    return NULL;
  }
  for (i = 0; i < nb1; i++){
    _pts[i] = createPtN();
    ptcopy(_pts[i], pts + i);
  }
  m = createMaillage(_pts[0], h, s->s_param);
  if (ind_maill != NULL)
    ind_maill[0] = ajouter_maillage(m, param);
  ajoute_courbe_maill(m, _pts, nb1, 1,
		      m->lignes->l[0], or, -1,
		      param->tol_pas, NULL, 0);
  ajoute_courbe_dual_maillage(m, c2, m->lignes->l[0],
			      or, or_vir, param,
			      type_maill, 0);
  free_param_maillage(param);
  return m;
}

DLLEXP void _CDECL
exp_exporter_maillage(Maillage *m, int bool_pull, int bool_dual,
		      int bool_curv, char *path, char *name_file,
		      P3D **res_pts,
		      int **res_quadr, int **res_nbpts_quadr,
		      int *size_pts, int *size_quadr){
  if (m != NULL){
    Param_maillage *param;
    int size[2];
    //   int bool_curv = 0;
    char *path_filelog = NULL;
    Mapping map = NULL;
    /*  if(bool_dual)
	map = mapping_homothetie;*/

#ifdef _DBG
    char name[1000] = "affichage.log";
    char _path_filelog[SIZE_TAB];
    if (path_filelog == NULL){
      get_path_filelog(_path_filelog, name);
      path_filelog = _path_filelog;
    }
#endif
    param = create_param_maillage(0.0, path_filelog, 0.1);
    if (bool_dual){
      if (m->quadr_dual == NULL)
	constr_dual(m, param);
    }
    maill_export_paraview(m, param, bool_pull, bool_dual,
			  bool_curv, path, name_file,
			  0, map, NULL, res_pts,
			  res_quadr, res_nbpts_quadr, size);
    if (size_pts != NULL){
      size_pts[0] = size[0];
      size_quadr[0] = size[1];
    }
  }
}

/*
 * Ajoute une courbe au maillage
 * est_primal : booléen (ajouter la courbe en primal ou en dual)
 * h : le pas (si == NULL on ajoute en condition primal dual)
 * pas : utilisé uniquement si m==NULL
 */
DLLEXP Maillage* _CDECL
exp_ajouter_courbe(Maillage *m, Surf *s, double *pt,
		   Courbe *c, int est_primal,
		   double *h, double pas,
		   int nb, double tol, int bool_fermer,
		   int bool_fermer_courbe,
		   char *path_filelog, int *ind_maill,
		   int ind_constr){
  Param_maillage *param;
  double H[SIZE_TAB];
  double *_h = NULL;
  int i, j;
  int or_init = 3;
  int type_maill = 3;
  int or_vir_init = 0;
  ListeD *linit = NULL;
  double d = -1.0;
  Ptmaill *ptm = NULL;
  P3D dir_init;
  ListeD *vois = NULL;
  int nb_vois;
  int bool_sing = 0;

#ifdef _DBG
  char name[1000] = "aj_courbe.log";
  char _path_filelog[SIZE_TAB];
  if (path_filelog == NULL){
    get_path_filelog(_path_filelog, name);
    path_filelog = _path_filelog;
  }
#endif
  bool_fermer_courbe = (m == NULL) && bool_fermer_courbe;

  param = create_param_maillage(0.0, path_filelog, tol);
  PRINT_WAR_LOG(param, "ajouter courbe 1\n");
  if (m != NULL && ind_constr <= m->ind_constr_max){
	  PRINT_WAR_LOG(param, "ajouter courbe 1bis : max : %d   new : %d\n", m->ind_constr_max, 
		  ind_constr);
    free_constr(m, ind_constr, param);
	  m->ind_constr_max = ind_constr;
  }
  else if (m != NULL) m->ind_constr_max++;
  PRINT_WAR_LOG(param, "ajouter courbe 2\n");

  if (est_primal && m != NULL){
    for (i = 0; i<nb; i++)
      H[i] = m->h;
    _h = H;
  }
  else if (h != NULL && !est_primal){
    _h = h;
  }
  else if (m == NULL){
    ind_maill[0] = -1;
    return NULL;
  }
  PRINT_WAR_LOG(param, "ajouter courbe 3\n");
  if (m != NULL){
    d = maillage_projection_bord(m, pt, &linit);
    if (d > param->dist_bord_max*m->h || linit == NULL)
      return m;
	ptm = linit->el->el;
	PRINT_WAR_LOG(param, "ajouter courbe 4\n");
  }
  if (m != NULL){
    if (s != NULL){
      if (m->s != NULL && s->s_param != m->s){
	free_surface_param(m->s);
      }
      m->s = s->s_param;
    }
    else {
      s = malloc(sizeof(Surf));
      s->type = 1;
	  s->s_param = m->s;
	  PRINT_WAR_LOG(param, "ajouter courbe 5\n");
    }

    if (linit->vois != NULL){
      bool_sing = 1;
      if (linit->vois->est_ferme){
	return NULL;
      }
    }
    else {
		maillage_set_sing(m, linit);
		PRINT_WAR_LOG(param, "ajouter courbe 6\n");
    }
	nb_vois = listeD_get_nb_max_vois(linit);
	PRINT_WAR_LOG(param, "ajouter courbe 7 nb vois : %d\n", nb_vois);
    if (!bool_sing && nb_vois == 4){
      return NULL;
    }
  }
  if (c != NULL){
    if(c->type != 0 || c->c_param->param != NULL)
      maill_dir_tan_init(s->s_param, c, dir_init);
    if (_h != NULL){
      Pt pts[SIZE_TAB];
      PPt _pts[SIZE_TAB];
      PPt *_pts2 = _pts;
      Pt *tmp = pts;
      int or_voistopt;
	  int bool = 0;
	  PRINT_WAR_LOG(param, "ajouter courbe 8\n");

      if (m != NULL){// && distance(pt, ptm->p->pt->p) > tol;

	if (bool)
	  tmp++;
	else {
	  if (c->type == 0){
	    P3D _tmp, _del, _dir, _dir_tmp, mat[3];
	    Pt _pt;
	    double ps;
	    courbeP_get_image(c->c_param, c->c_param->in, _tmp);
	    surf_setpt(s->s_param, &_pt, _tmp);
	    image_cvect_p(s->s_param->f, _tmp, mat);

		PRINT_WAR_LOG(param, "ajouter courbe 9\n");
	    VECT_DEL(_del, ptm->p->pt->p, _pt.p, j);
	    courbeP_get_image_p(c->c_param, c->c_param->in, _dir_tmp);
	    prod_mat_vect(mat, 3, 2, _dir_tmp, _dir);
	    normer(_dir);
	    ps = prod_scal(_del, _dir);
		_h[0] -= ps;
		PRINT_WAR_LOG(param, "ajouter courbe 10\n");
	  }
	}
	  }
	  PRINT_WAR_LOG(param, "ajouter courbe 11\n");
      nb = discretiser_courbe(s->s_param, c, _h, nb, param,
		  tmp, &bool_fermer_courbe);
	  PRINT_WAR_LOG(param, "ajouter courbe 12\n");
      if (nb <= 0){
	if (ind_maill != NULL){
	  ind_maill[0] = -1;
	}
	return NULL;
      }
      if (m != NULL){
	if (bool){
	  ptcopy(pts, ptm->p->pt);
	  nb++;
	}
      }
      for (i = 0; i < nb; i++){
	_pts[i] = createPtN();
	ptcopy(_pts[i], pts + i);
      }
      if (est_primal){
	// ajout d'une courbe du primal 
	// => on regarde la direction de l'ajout

	if (linit->vois == NULL && bool_fermer){
	  int _ind;
	  P3D del;
	  Ptmaill *ptm_vois = NULL;
	  int nb_tot = 0;
	  double angle_tmp, angle_min = PI;

	  for (i = 0; i < nb_vois; i++){
	    vois = liste_getVois(linit, i);
	    if (vois != NULL){
	      nb_tot++;
	      ptm_vois = vois->el->el;
	      VECT_DEL(del, ptm->p->pt->p, ptm_vois->p->pt->p, j);
	      angle_tmp = fabs(angle(dir_init, del) - PI);
	      if (angle_min > angle_tmp){
		_ind = i;
		angle_min = angle_tmp;
	      }
	    }
	  }
	  or_init = (_ind + 2) % 4;
	  if (nb_tot != 2){
	    return NULL;
	  }
	}
	else {
	  bool_sing = 0;
	  or_init = nb_vois;
	  vois = liste_getVois(linit, nb_vois - 1);
	  or_voistopt = (vois->el->dir_sing + 1) % 4;
	}
	PRINT_WAR_LOG(param, "ajouter courbe 13\n");

	ajoute_courbe_maill(m, _pts, nb, 1,
			    linit, or_init, or_voistopt,
				param->tol_pas, param, ind_constr);
	PRINT_WAR_LOG(param, "ajouter courbe 14\n");
	maillage_che(m, param, 0, linit, 0, ind_constr);
      }
      else {
	or_voistopt = 0;
	if (m == NULL){
	  m = createMaillage(_pts[0], pas, s->s_param);

	  if (ind_maill != NULL)
	    ind_maill[0] = ajouter_maillage(m, param);
	  linit = m->lignes->l[0];
	}
	else {
		PRINT_WAR_LOG(param, "ajouter courbe 15\n");
	  or_vir_init = 1;

	  vois = liste_getVois(linit, nb_vois - 1);
	  or_voistopt = (vois->el->dir_sing + 1) % 4;
	  or_init = nb_vois;
	}
	if (bool_fermer_courbe){
	  ajoute_diag_ferme_maill(m, _pts2, nb, linit,
				  or_init, or_vir_init, param, 1,
				  type_maill, ind_constr);
	}
	else{
		PRINT_WAR_LOG(param, "ajouter courbe 16\n");
	  ajoute_diag_maill(m, &_pts2, 1, &nb, linit,
			    or_init, or_vir_init, or_voistopt, param, 1,
			    type_maill, NULL, NULL, 0, ind_constr);
	}
      }
    }
    else if (m != NULL){
      // on est dans le cas d'une condition primale-duale
      or_vir_init = 1;
	  or_init = nb_vois - 1;
	  PRINT_WAR_LOG(param, "ajouter courbe 17\n");
      ajoute_courbe_dual_maillage(m, c, linit, or_init,
				  or_vir_init, param,
				  type_maill, ind_constr);
    }
  }

  if (linit != NULL){
	  PRINT_WAR_LOG(param, "ajouter courbe 18\n");
    int bool_free = free_branches(m, param, linit);
    if (!bool_free && bool_fermer){
      if (maillage_fermer_sing(m, linit)){
	maillage_che(m, param, 0, linit, 0, ind_constr);
      }
    }
  }
  update_sing(m);
  return m;
}

DLLEXP Maillage* _CDECL exp_get_maill(int indice){
  if (indice < 0 || indice >= nb_maillage_saved)
    return NULL;
  return save[indice];
}

int ajouter_maillage(Maillage *m, Param_maillage *param){
  if (nb_maillage_saved >= NB_SAVE_MAX){
    int i;
    for (i = 0; i < NB_SAVE_MAX; i++){
      free_maillage(save[i], param);
    }
    nb_maillage_saved = 0;
  }
  save[nb_maillage_saved] = m;
  nb_maillage_saved++;
  return nb_maillage_saved - 1;
}


DLLEXP Maillage* _CDECL
exp_generer_sing_neg(Surf *s, P3D param_init, P3D dir,
		     double h, int nbptr, double tol_maill,
		     double _tol, int *ind_maill){
  Maillage *m;
  Pt *pt_init = createPtN();
  int term = 0;
  double the, the_prec = -10.;
  double _the[SIZE_TAB];
  double curv[2], curv_prec[2];
  Courbe *_c1, *_c2, *c1, *c2, *cp;
  PListeD init[2][10];
  double tol = _tol*PI/180.;
  double del_inf_sup = tol / 2.;
  double min_the = tol + del_inf_sup / 2.;
  double max_the = PI - tol - del_inf_sup / 2.;
  Mesh_curv *mesh = NULL;
  char path[SIZE_TAB] = "";
  char name_curv[SIZE_TAB] = "test_curv.vtk";
  int i = 0, j = 0, k;
  double borne_the[2] = {0.,PI};
  int nb_init[2];
  double _h = 0.2;
  int fin_boucle = 0;
  int ind1, ind2;
  int b_fermer;
  double tot_the = 0.;
  char *path_filelog = NULL;
  double H[SIZE_TAB];
  int ind_pert, nb_pert, ret, nb_pert_max = 15;
  double del_pert = 0.03, cur_del;
  P3D dir_tmp;
  int angle_croissant = 1;

#ifdef _DBG
  char name[1000] = "sing_neg.log";
  char _path_filelog[SIZE_TAB];
  if (path_filelog == NULL){
    get_path_filelog(_path_filelog, name);
    path_filelog = _path_filelog;
  }
#endif
  Param_maillage *param = create_param_maillage(0.0, path_filelog,
						tol_maill);
  PRINT_WAR_LOG(param, "début fonction\n");
  double depl_min = tol*pow(10., -9);
  SListe *lc1 = createSListe(NULL,NULL,NULL,exp_pt_liste,NULL);
  SListe *lc2 = createSListe(NULL,NULL,NULL,exp_pt_liste,NULL);
  SListe *lctmp = NULL;
  int nbx = param->discr_nbx, nby = param->discr_nby;
  int bool_modif_discr = 0;
  int nb_modif = 0, nb_modif_max = 5;
	      
  for(i = 0;i<SIZE_TAB;i++)
    H[i] = _h;

  if (s->s_param->gauss == NULL){
    PRINT_WAR_LOG(param, "la surface n'a pas de définition\
 de la courbure de Gauss...\n");
    //    return NULL;
  }
  PRINT_WAR_LOG(param, "tol angle losange : %lf    \
tol angle constr : %lf\n", param->tol_angle, tol);

  surf_setpt(s->s_param, pt_init, param_init);
  m = createMaillage(pt_init, h, s->s_param);

  the = (max_the + min_the) / 2.;
  memset(_the, 0, nbptr*sizeof(double));
  _c1 = exp_createCourbe(NULL, NULL, nbptr, NULL,
			 0.0, param_init, dir,
			 _the, _h);
  _c2 = exp_createCourbe(NULL, NULL, nbptr, NULL,
			 0.0, param_init, dir,
			 _the, _h);
  c1 = _c1; c2 = _c2;

  curv[0] = 0.;curv[1] = 0.;
  PRINT_WAR_LOG(param, "début boucle\n");
  while (!fin_boucle){
    term = 0;
    i = 0;
    borne_the[0] = 0.;
    borne_the[1] = PI;
    curv[0] = 0.;
    curv[1] = 0.;
    angle_croissant = 1;

    the = _fmin(the, 2.*PI - tot_the);
    nb_pert = 0;
    nb_modif = 0;
    while (!term){
      //   PRINT_WAR_LOG(param, "courbe %d   it %d\n", j, i);
      c2->c_geod->courbure[0] = tot_the + the;
      bool_modif_discr = 0;

      curv_prec[0] = curv[0];curv_prec[1] = curv[1];
      ret = calc_curv(s->s_param, c2, c1, lc2, lc1, curv, 
		      &mesh, param, H, init, nb_init, nbx, nby);
      if (ret < 1){
	if(ret == -10){
	  PRINT_WAR_LOG(param, "ERREUR\n");
	  return NULL;
	}
	m->mesh_curv = mesh;
	/*	PRINT_WAR_LOG(param, "ERREUR\n");
		return NULL;*/
	if(nb_pert < nb_pert_max){
	  /* on perturbe l'angle */
	  if(nb_pert == 0 || ind_pert != abs(ret)){
	    nb_pert = 0;
	    ind_pert = abs(ret);
	  }
	  cur_del = del_pert*(borne_the[1] - borne_the[0]);
	  if(ret == -1){
	    c1->c_pts->nb = -1;
	    c1->type = 1;
	    surf_rotation_vect(s->s_param, pt_init, 
			       c1->c_geod->dir_init, 
			       cur_del, dir_tmp);
	    VECT_CPY(c1->c_geod->dir_init, dir_tmp);
	  } else {
	    the += cur_del;
	    c2->c_pts->nb = -1;
	    c2->type = 1;
	  }
	  nb_pert++;
	} else {
	  PRINT_WAR_LOG(param, "ERREUR lors du calcul de \
l'intégral de la courbure\n");
	  return m;
	}
      } else {
	/* si on augmente l'angle et que la courbure diminue, 
	 * c'est qu'on a un problème de continuité lors du calcul de la courbure
	 * => on doit augmenter la discrétisation
	 */
	if((angle_croissant == 1
	    && (curv[0]+EPS_TOL  < curv_prec[0] 
		|| curv[1]+EPS_TOL  < curv_prec[1]))
	   || (angle_croissant == 0 
	       && (curv[0] > curv_prec[0]+EPS_TOL 
		   || curv[1] > curv_prec[1]+EPS_TOL))){
	    bool_modif_discr = 1;
	} else {
	  nb_pert = 0;
	  the_prec = the;
	  /*   PRINT_WAR_LOG(param, "courbe %d    curv : %lf;%lf   the_prec : %lf\n", 
	       j, curv[0], curv[1], the);*/
	  if (curv[0] > PI - the - tol){
	    if (the == min_the){
	      if(min_the-EPS_TOL < param->tol_angle_rad){
		PRINT_WAR_LOG(param, "l'angle necessaire est inférieur \
à la tolérance => fin de l'algorithme\n");
		return m;
	      }
	      min_the = _fmax(min_the/2., param->tol_angle_rad);

	      PRINT_WAR_LOG(param, "changement angle min : %lf\n", 
			    min_the*180. / PI);
	      //return m;
	    }
	    borne_the[1] = the;
	    the = _fmax((borne_the[0] + the) / 2., min_the);
	    angle_croissant = 0;
	  }
	  else if (curv[0] < PI - the - tol - del_inf_sup){
	    if (the == max_the || the >= 2.*PI - tot_the){
	      term = 1;
	    }
	    else {
	      borne_the[0] = the;
	      the = _fmin(_fmin((borne_the[1] + the) / 2., max_the),
			  2.*PI - tot_the);
	      angle_croissant = 1;
	    }
	  }
	  else
	    term = 1;
	  //  printf("the : %lf\n", the);

	  m->mesh_curv = mesh;
	  if (!term && fabs(the - the_prec) < depl_min){
	    bool_modif_discr = 1;
	    /*  PRINT_WAR_LOG(param, "on ne se déplace plus : il y a une erreur : courbe : %d   it : %d\n", j, i);
		return m;*/
	  } else {
	    ind1 = 0;
	    ind2 = 2;
	    //  term = (i>=2);
	    if (j < 0){
	      sprintf(name_curv, "test_curv%d_%d.vtk", j, i);
	      exp_exporter_maillage(m, 0, 0, 1, path, name_curv,
				    NULL, NULL, NULL, NULL, NULL);
	    }
	    if (term){
	      ind1 = 2;
	      ind2 = 0;
	      for (k = 0; k < nb_init[0]; k++)
		init[1][k] = init[0][k];
	      nb_init[1] = nb_init[0];
	    }
	    else{
	      c2->c_pts->nb = -1;
	      c2->type = 1;
	    }
	  }
	}
      }
      if(ret == -1) {
	ind1 = 1;
	ind2 = 0;
      } else if(ret == 0){
	ind1 = 0;
	ind2 = 2;
      }
      if(!bool_modif_discr){
	mesh_suppr_passage_curv(mesh, init, nb_init, 
				param, ind1, ind2);
	
	if (j < 0){
	  sprintf(name_curv, "test_suppr%d_%d.vtk", j, i);
	  exp_exporter_maillage(m, 0, 0, 1, path, name_curv,
				NULL, NULL, NULL, NULL, NULL);
	}
	i++;
      } else {
	if(nb_modif < nb_modif_max){
	  nb_modif++;
	  nbx += param->del_discr_nbx;
	  nby += param->del_discr_nby;
	  free_mesh_curv(mesh);
	  mesh = NULL;
	  borne_the[0] = 0.;
	  borne_the[1] = PI;
	  curv[0] = 0.;curv[1] = 0.;
	  angle_croissant = 1;

	  the = _fmin(the, 2.*PI - tot_the);
	  nb_pert = 0;
	  c1->c_pts->nb = -1;
	  c1->type = 1;
	  c2->c_pts->nb = -1;
	  c2->type = 1;
	  i = 0;
	  for (k = 0; k < nb_init[0]; k++)
	    init[0][k] = NULL;
	  for (k = 0; k < nb_init[1]; k++)
	    init[1][k] = NULL;
	  nb_init[0] = 0;nb_init[1] = 0;
	} else {
	  PRINT_WAR_LOG(param, "ERREUR lors du calcul de \
l'intégral de la courbure : 2 rediscretisation\n");
	  return m;
	}
      } 
    }
    PRINT_WAR_LOG(param, "courbe %d, nb iter %d\n", j, i);
    if (2.*PI - tot_the - the < tol / 4.){
      the = 2.*PI - tot_the;
      tot_the = 2.*PI;
      fin_boucle = 1;
    }
    else  tot_the += the;
    b_fermer = 0;
    if (fin_boucle){
      b_fermer = 1;
      c2 = NULL;
    }
    //   s->s_param->fin[0] = 1.2;
    if (j == 0){
      m = exp_maillage_courbes_primales(s, c1, c2, h,
					nbptr, nbptr,
					tol_maill, NULL, NULL);
    }
    else {
      exp_ajouter_courbe(m, s, pt_init->p, c2, 1, NULL, m->h, nbptr,
			 tol_maill, b_fermer, 0, NULL, ind_maill, j);
    }
    //  s->s_param->fin[0] = 1.4;
    /*   sprintf(name_curv, "maill%d.vtk", j);
	 exp_exporter_maillage(m, 0, 0, 0, path, name_curv,
	 NULL, NULL, NULL, NULL, NULL);*/
    //   s->s_param->fin[0] = 1.3;

    if (!fin_boucle){
      cp = c1;
      lctmp = lc1;
      c1 = c2;
      c2 = cp;
      c2->c_pts->nb = -1;
      c2->type = 1;
      lc1 = lc2;
      lc2 = lctmp;
    }
    j++;
  }
  //  s->s_param->fin[0] = 1.45;
  if (ind_maill != NULL){
    ind_maill[0] = ajouter_maillage(m, param);
  }
  free_param_maillage(param);
  return m;
}

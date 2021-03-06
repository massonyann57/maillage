#include "maillage.h"

/*
 * constructeur Maillage
 */
Maillage* createMaillage(Pt* init, double h,
			 Surf_param *s){
  Maillage *res = malloc(sizeof(Maillage));
  Ptmaill *new = NULL;
  int(*cmp)(void*, void*, void*) = NULL;

  if (init != NULL)
    new = createPtmaill(init, 0);
  res->lignes = createSListeD((void*) new,
			      cmp, print_ptMaill,
			      est_bord_pt, (void*)res,
			      callback_aj_vois,
			      res, 0, 0, -1, 1);
  res->quadr = createSListeD(NULL, NULL, print_el_quadr, NULL,
			     NULL, NULL, NULL, 0, 0, -1, 0);
  res->seg = create_gtab(NULL, NULL);
  res->h = h;

  res->ind_max = INT_MAX;
  res->s = s;

  res->pts_dual = NULL;
  res->quadr_dual = NULL;
  res->ind_constr_max = 0;
  res->mesh_curv = NULL;
  return res;
}

Param_call1* create_param_call1(int or, double dist,
				Pt *pt, ListeD *prec){
  Param_call1 *res = malloc(sizeof(Param_call1));
  res->or = or;
  res->dist_p = dist;
  res->pt = pt;
  res->prec = prec;
  return res;
}

/*
 * Ajoute un point et un ||gramme
 * ind_surf : pas utilis� pour le moment
 * p_maill : le point pr�c�dent
 * new_p : le nouveau point
 * or : la direction de l'ajout du ||gramme
 * boo_sing : est une singularit�e ou non
 * (si oui, on ajoute le point mais les segments cr��s ne pourront
 * plus �tre utilis�s)
 */
void ajouterPoint_comp(Maillage *m, Param_maillage *param_s, Pt* new_p,
		       int ind_constr,
		       ListeD *p_maill, int ind_surf, int or,
		       int boo_sing){
  ListeD *newl;
  Ptmaill *newp = createPtmaill(new_p, ind_constr);
  ListeD *p2;
  ListeD *p3;
  int or_aj_pt = or;
  int or_voistopt = (or + 2) % 4;
  int est_etoile = 0;

  int or_p2 = or + 3;
  int _del;

  if (p_maill->vois != NULL)
    or_p2 = or - 1;
  _del = listeD_get_saut(p_maill, or_p2);

  or_aj_pt += _del;
  p2 = liste_getVois(p_maill, or_p2);
  p3 = liste_getVois(p_maill, or);

  if (p2->vois != NULL){
    or_voistopt = (p_maill->el->dir_sing + 1) % 4;
    or_aj_pt = p_maill->el->pos_sing + 1;
    est_etoile = 1;
  }
  if (p_maill->vois != NULL){
    or_aj_pt = (p2->el->dir_sing + 3) % 4;
    or_voistopt = (p2->el->dir_sing + 1) % 4;
    est_etoile = 1;
  }
  if (p3->vois != NULL)
    est_etoile = 1;

  /* on ajoute � la liste de points */
  newl = ajouter_listeD(m->lignes, p2, (void*)newp,
			NULL, or_aj_pt, NULL, or_voistopt, 0, 0, -1, param_s);
  ajouterQuadr(m, param_s, newl, p_maill, ind_surf, or,
	       est_etoile, _del);
}

/*
 * Ajoute un point et un ||gramme
 * ind_surf : pas utilis� pour le moment
 * p_maill : le point pr�c�dent
 * new_p : le nouveau point
 * dir_ros : l'indice du point qui est une rosace (0 si aucun des 2)
 * or : la direction de l'ajout du ||gramme
 * boo_sing : est une singularit�e ou non
 * (si oui, on ajoute le point mais les segments cr��s ne pourront
 *    plus �tre utilis�s)
 */
int ajouterQuadr(Maillage *m, Param_maillage *param_s,
		 ListeD *newl, ListeD *p_maill,
		 int ind_surf, int or, int est_etoile, int del){
  ListeD *p2;
  ListeD *p3;
  Ptmaill *pt_cour = get_elementL2D(p_maill);
  Ptmaill *pt_new = get_elementL2D(newl);

  Quadr *newq;
  Segment *seg_vois1 = NULL, *seg_vois2 = NULL;
  PListeD pquadr[4];
  int ind_seg[4];
  ListeD *nlquadr = NULL;
  int aj_vois1 = 0;

  int or2a, or2b;
  int or_voistopt = (or + 2) % 4;
  int saut_new = 0;
  int dir_d, dir_g, dir_b;
  int ind_tmp;

  Segment *seg1;
  Segment *seg2;
  int _del1 = 0;
  int _del2 = 0;
  if (p_maill->vois == NULL){
    dir_b = (or + 2) % 4;
    dir_g = (or + 1) % 4;
    dir_d = (or + 3) % 4;
  }
  else{
    dir_b = or - 2;
    dir_g = or + 1;
    dir_d = or - 1;
    if (p_maill->vois->est_ferme){
      if (dir_d == -1)
	dir_d = p_maill->vois->nb_vois - 1;
      dir_b = (dir_b + p_maill->vois->nb_vois) %
	p_maill->vois->nb_vois;
      dir_g = dir_g%p_maill->vois->nb_vois;
    }
    else if (dir_g == p_maill->vois->nb_vois)
      dir_g = -1;
  }
  if (p_maill->saut != NULL && !p_maill->saut->dessus){
    _del1 = listeD_get_saut(p_maill, dir_b);
    _del2 = listeD_get_saut(p_maill, dir_g);
  }

  p3 = liste_getVois(p_maill, or);
  ind_seg[0] = ptmaill_get_ind_seg(pt_cour, or);
  seg_vois2 = get_element_gtab(m->seg, ind_seg[0]);

  p2 = liste_getVois(p_maill, dir_d);
  ind_seg[1] = ptmaill_get_ind_seg(pt_cour, dir_d);
  seg_vois1 = get_element_gtab(m->seg, ind_seg[1]);

  if (p2->vois != NULL)
    or_voistopt = (p_maill->el->dir_sing + 1) % 4;
  else if (p_maill->vois != NULL)
    or_voistopt = (p2->el->dir_sing + 1) % 4;
  if (p_maill->vois == NULL)
    saut_new = listeD_get_saut(p2, or + del);
  else {
    saut_new = listeD_get_saut(p2, or_voistopt + 2 + del);
  }
  ind_seg[2] = ptmaill_get_ind_seg(pt_new, (or_voistopt
					    + del + 3 + saut_new));
  ind_seg[3] = ptmaill_get_ind_seg(pt_new, or_voistopt
				   + del + saut_new);
  seg1 = get_element_gtab(m->seg, ind_seg[2]);
  seg2 = get_element_gtab(m->seg, ind_seg[3]);

  pquadr[0] = p_maill;  pquadr[1] = p2;
  pquadr[2] = newl;  pquadr[3] = p3;

  newq = createQuadr(pquadr, ind_seg, ind_surf, 0);

  if (seg_vois1->q1 != NULL){
    int ind_el_par = 0;
    if (appartientQuadr(get_elementL2D(seg_vois1->q1), p3))
      ind_el_par = 1;
    /* on v�rifie s'il y a un saut entre seg_vois1->q1 et nlquadr...*/
    if (ind_el_par){
      or2a = (or_voistopt + 1 + del) % 4;
      if (listeD_get_saut(seg_vois1->q1, (or2a + _del2) % 4) > 0){
	or2a = (or2a + _del2) % 4;
      }
    }
    else
      or2a = (or_voistopt + 2 + del + _del1) % 4;
    nlquadr = ajouter_listeD(m->quadr, seg_vois1->q1,
			     newq, NULL, or2a, NULL, -1, 0,
			     est_etoile, -1, param_s);
    aj_vois1 = 1;
  }
  if (seg_vois2->q1 != NULL){
    int ind_el_par = 0;
    if (appartientQuadr(get_elementL2D(seg_vois2->q1), p3))
      ind_el_par = 1;
    if (ind_el_par)
      or2b = (or_voistopt + 1 + del + _del2) % 4;
    else
      or2b = (or_voistopt + 2 + del + _del1) % 4;
    if (!aj_vois1){
      nlquadr = ajouter_listeD(m->quadr, seg_vois2->q1,
			       newq, NULL, or2b, NULL, 0, 0,
			       est_etoile, -1, param_s);
    }
    else if (nlquadr->el->ind_part != seg_vois2->q1->el->ind_part){
      // le voisin �tait jusque l� isol�
      // => on cherche � la main un autre voisin
      //   permettant de se raccrocher � la composante connexe
      //   principale
      //
      int part_iso, part_gr;

      ajouter_vois(m->quadr, seg_vois2->q1, nlquadr,
		   or2b, 0, param_s);
      if (nlquadr->el->ind_part > seg_vois2->q1->el->ind_part){
	part_gr = seg_vois2->q1->el->ind_part;
	part_iso = nlquadr->el->ind_part;
	merger_composante_isole(m->quadr, part_gr, part_iso);
	if (nlquadr->el->ind_part != part_gr)
	  listeD_changer_partition(m->quadr, nlquadr, part_gr);
	if (seg_vois1->q1->el->ind_part != part_gr){
	  PRINT_WAR_LOG(param_s, "ERREUR : on a une composante isol�e ne contenant pas qu'un seul ||gramme!!!\n");
	}
	ajouter_bord(m->quadr, seg_vois1->q1,
		     m->quadr->bord[part_gr]->fin);
	ajouter_bord(m->quadr, nlquadr,
		     m->quadr->bord[part_gr]->fin);
      }
      else{
	part_gr = nlquadr->el->ind_part;
	part_iso = seg_vois2->q1->el->ind_part;

	merger_composante_isole(m->quadr, part_gr, part_iso);
	ajouter_bord(m->quadr, seg_vois2->q1,
		     m->quadr->bord[part_gr]->fin);
      }
    }
    else if (nlquadr->el->est_etoile && seg_vois1->q1->el->est_etoile
	     && seg_vois2->q1->el->est_etoile){
      if (nlquadr->saut != NULL){
	ind_tmp = (or2b + del + 2) % 4;
	ind_tmp = (ind_tmp + 4 - listeD_get_saut(nlquadr, ind_tmp)) % 4;
	ajouter_vois(m->quadr, nlquadr, seg_vois2->q1,
		     ind_tmp, 0, param_s);
      }
      else{
	int _del = 0;
	if (seg_vois2->q1->saut != NULL)
	  _del = 4 - seg_vois2->q1->saut->delta;
	ajouter_vois(m->quadr, seg_vois2->q1, nlquadr,
		     or2b + del + _del, 0, param_s);
      }
    }
  }
  else if (!aj_vois1){
    nlquadr = ajouter_listeD(m->quadr, NULL, newq, NULL,
			     or, NULL, -1, 0, est_etoile, -1, param_s);
  }

  seg_ajouter_quadr(seg_vois1, nlquadr);
  seg_ajouter_quadr(seg_vois2, nlquadr);
  if (seg1 != NULL)
    seg_ajouter_quadr(seg1, nlquadr);
  else
    return 0;
  seg_ajouter_quadr(seg2, nlquadr);

  actualiser_bord(m->lignes, p2, or, newl->el->ind_part);
  return 1;
}

/* Ajoute le segment pour un nouveau point
 * init : le point pr�c�dent
 * or : la direction de l'ajout
 * lnewp : le nouveau point
 */
void ajoute_vois_seg(Maillage *m, ListeD *init, int or,
		     int or_voistopt, ListeD *lnewp){
  Ptmaill *pt_cour, *newp = get_elementL2D(lnewp);
  Segment *segm;
  int pos;

  pt_cour = get_elementL2D(init);

  segm = createSegment(pt_cour->p, newp->p);
  segm->b_new = 1;

  pos = aj_gtab(m->seg, segm);

  ptmaill_set_ind_seg(pt_cour, or, pos);
  ptmaill_set_ind_seg(newp, or_voistopt, pos);
}

/* Ajoute un point (sans ajouter de ||grammes)
 * ajoute le point � la liste puis cr�� les segments
 * or : la direction dans laquelle on ajoute newp
 */
void ajoute_pt(Maillage *m, ListeD *init, int or,
	       int or_voistonew, Ptmaill *newp, Param_maillage *param_s){
  ajouter_listeD(m->lignes,
		 init, (void*)newp, NULL,
		 or, NULL, or_voistonew, 0, 0, -1, param_s);
  /*  if(init->vois == NULL)
      or_voistonew = (or+2)%4;*/
  /*  ajoute_vois_seg(m, init, or, or_voistonew, lnewp);*/
}

/* Ajoute un point (sans ajouter de ||grammes)
 * ajoute le point � la liste puis cr�� les segments
 */
void ajoute_pt_exis(Maillage *m, ListeD *init, int or,
		    Ptmaill *newp, ListeD *nl, Param_maillage *param){
  /*ListeD* lnewp = */ajouter_listeD(m->lignes,
				     init, (void*)newp, nl,
				     or, NULL, -1, 0, 0, -1, param);
  /*  ajoute_vois_seg(m, init, or, (or+2)%4, lnewp);*/
}

/* Ajoute une courbe : /!\ on n'ajoute pas le premier point
 * or : 0 -> vers le haut ... puis on tourne dans le sens +
 */
void ajoute_courbe_maill(Maillage *m, PPt *pts,
			 int nb, int ordre,
			 ListeD *init, int or,
			 int or_voistonew,
			 double tol_pas,
			 Param_maillage *param,
			 int ind_constr){
  int i;
  ListeD *pt_cour = init;
  ListeD *suiv;
  Pt *pt;
  Ptmaill *ptm;

  if (init->vois == NULL)
    or_voistonew = -1;

  for (i = 1; i < nb; i++){
    if (!ordre)
      pt = pts[nb - i - 1];
    else
      pt = pts[i];
    ptm = createPtmaill(pt, ind_constr);
    ptm->p->est_bord_sector = 1;
    ajoute_pt(m, pt_cour, or, or_voistonew,
	      ptm, param);
    suiv = liste_getVois(pt_cour, or);
    //   if(listeD_get_saut(
    if (i == 1 && init->vois != NULL){
      or = (or_voistonew + 2) % 4;
      or_voistonew = -1;
    }
    pt_cour = suiv;
  }
}


Point* ligne_get_pt(ListeD *l){
  return l->el->el;
}

void ligne_set_pt(ListeD *l, Ptmaill *p){
  l->el->el = p;
}

/*
 * Exporte le maillage vers paraview
 */
void maill_export_paraview(Maillage *m, Param_maillage *param,
			   int bool_pull, int bool_dual, int bool_curv,
			   char *path, char *nom, int ind_part,
			   Mapping map, void *param_map,
			   P3D **exp_pts, int **exp_quadr,
			   int **exp_nbpts_quadr,
			   int *res_size){
  char namestat[1000];
  FILE *fvtk = NULL;
  int i, k, nbpts, nbpar, nbpts_suppr, nbpar_suppr;
  Pt* ptmp;
  Ptmaill *tmp;
  int _ind;
  Quadr *el_quadr;
  Dual_param *param_dual;
  ListeD *el_cur;
  P3D pt_aff;
  int exp_file = path != NULL && nom != NULL;
  int exp_rhino = (exp_pts != NULL);
  GTab *pts;
  GTab *quadr;
  int ind_quadr[2 * NB_BRANCHES_MAX];
  int ind_dec = 0, nbpts_par;
  int nbpts_tot, _ind2 = 0;
  PListeD *_pts;
  Quadr_mesh *quadr_curv;

  if (bool_dual){
    pts = m->pts_dual->mailles[0];
    quadr = m->quadr_dual;
  }
  else if (!bool_curv){
    pts = m->lignes->mailles[ind_part];
    quadr = m->quadr->mailles[ind_part];
  }
  else {
    if (m->mesh_curv == NULL)
      return;
    pts = m->mesh_curv->pts;
    quadr = m->mesh_curv->mesh->mailles[0];
  }
  nbpts = pts->nb_tot;
  nbpar = quadr->nb_tot;
  nbpts_suppr = pts->nb_suppr;
  nbpar_suppr = quadr->nb_suppr;

  if (m->quadr->nb_part != m->lignes->nb_part && param != NULL){
    PRINT_WAR_LOG(param, "attention, erreur possible : les 2 graphes n'ont pas le m�me nombre de composantes connexes\n");
  }

  if (exp_rhino){
    res_size[0] = nbpts - nbpts_suppr;
    res_size[1] = nbpar - nbpar_suppr;
    exp_pts[0] = malloc(res_size[0] * sizeof(P3D));
    if (bool_dual)
      exp_quadr[0] = malloc(m->tot_pts_dual * sizeof(int));
    else
      exp_quadr[0] = malloc(4 * res_size[1] * sizeof(int));
    exp_nbpts_quadr[0] = malloc(res_size[1] * sizeof(int));
  }

  if (exp_file){
    sprintf(namestat, "%s%s", path, nom);
    fvtk = fopen(namestat, "w+");
    if (fvtk == NULL){
      param->err = IO_ERROR;
      if (param != NULL){
	PRINT_ERR_LOG(param, "impossible d'ouvrir le fichier : %s\n", namestat);
      }
      return;
    }
    fprintf(fvtk, "# vtk DataFile Version 2.0\n");
    fprintf(fvtk, "t = 0\n");
    fprintf(fvtk, "ASCII\nDATASET POLYDATA\n");

    fprintf(fvtk, "POINTS %d float\n", nbpts - nbpts_suppr);
  }

  _ind = 0;
  for (i = 0; i < nbpts; i++){
    el_cur = NULL;
    if (!bool_curv){
      el_cur = get_element_gtab(pts, i);
    }
    if (el_cur != NULL || bool_curv){
      if (bool_curv){
	ptmp = get_element_gtab(pts, i);
      }
      else {
	el_cur->el->pos_tab_exp = el_cur->el->pos_tab - ind_dec;
	if (bool_dual)
	  el_cur = el_cur->el->el;
	tmp = el_cur->el->el;
	ptmp = tmp->p->pt;
      }
      if (map != NULL)
	map(ptmp->p, pt_aff, param_map);
      else{
	if (bool_pull){
	  VECT_CPY(pt_aff, ptmp->param);
	}
	else {
	  VECT_CPY(pt_aff, ptmp->p);
	}
      }
      if (exp_file){
	fprintf(fvtk, "%lf %lf %lf\n", pt_aff[0],
		pt_aff[1], pt_aff[2]);
      }
      if (exp_rhino){
	VECT_CPY(exp_pts[0][_ind], pt_aff);
      }
      _ind++;
    }
    else
      ind_dec++;
  }
  if (exp_file)
    fprintf(fvtk, "\n");

  if (nbpar > 0){
    if (exp_file){
      if (bool_dual)
	nbpts_tot = m->tot_pts_dual + 2 * (nbpar - nbpar_suppr);
      else
	nbpts_tot = 6 * (nbpar - nbpar_suppr);
      fprintf(fvtk, "POLYGONS %d %d\n",
	      nbpar - nbpar_suppr, nbpts_tot);
    }
    _ind = 0;
    for (i = 0; i < nbpar; i++){
      el_cur = NULL;
      if (bool_dual){
	param_dual = get_element_gtab(quadr, i);
	_pts = NULL;
	if (param_dual != NULL){
	  _pts = param_dual->pts;
	  nbpts_par = param_dual->nbpts;
	}
      }
      else {
	_pts = NULL;
	el_cur = get_element_gtab(quadr, i);
	if (el_cur != NULL && !bool_curv){
	  el_quadr = el_cur->el->el;
	  _pts = el_quadr->pts;
	  nbpts_par = 4;
	}
      }
      if (_pts != NULL || bool_curv){
	if (bool_curv){
	  quadr_curv = el_cur->el->el;
	  nbpts_par = 4;
	  for (k = 0; k < nbpts_par; k++)
	    ind_quadr[k] = quadr_curv->pts[k];
	}
	else{
	  for (k = 0; k < nbpts_par; k++)
	    ind_quadr[k] = _pts[k]->el->pos_tab_exp;
	}
	if (exp_file){
	  fprintf(fvtk, "%d ", nbpts_par + 1);
	  for (k = 0; k < nbpts_par; k++){
	    fprintf(fvtk, "%d ", ind_quadr[k]);
	  }
	  fprintf(fvtk, "%d\n", ind_quadr[0]);
	}
	if (exp_rhino){
	  nbpts_par = 4;
	  exp_nbpts_quadr[0][_ind] = nbpts_par;
	  if (nbpts_par == 4){
	    for (k = 0; k < nbpts_par; k++)
	      exp_quadr[0][_ind2 + k] = ind_quadr[k];
	    _ind2 += nbpts_par;
	  }
	}
	_ind++;
      }
    }
    if (exp_file)
      fprintf(fvtk, "\n");
  }
  if (exp_file && !bool_dual && !bool_curv){
    Ptmaill *ptm;
    fprintf(fvtk, "POINT_DATA %d\n", nbpts - nbpts_suppr);
    fprintf(fvtk, "SCALARS the float\n");
    fprintf(fvtk, "LOOKUP_TABLE default\n");
    for (i = 0; i < nbpts; i++){
      el_cur = get_element_gtab(pts, i);
      if (el_cur != NULL){
	ptm = el_cur->el->el;
	fprintf(fvtk, "%lf\n", ptm->p->the);
      }
    }
    fprintf(fvtk, "\n");
    fprintf(fvtk, "SCALARS bord int\n");
    fprintf(fvtk, "LOOKUP_TABLE default\n");
    for (i = 0; i < nbpts; i++){
      el_cur = get_element_gtab(pts, i);
      if (el_cur != NULL){
	ptm = el_cur->el->el;
	fprintf(fvtk, "%d\n", ptm->p->est_bord_sector);
      }
    }
    fprintf(fvtk, "\n");
  }
  if (exp_file && bool_curv){
    // double val;
    fprintf(fvtk, "CELL_DATA %d\n", nbpar - nbpar_suppr);
    fprintf(fvtk, "SCALARS gauss float\n");
    fprintf(fvtk, "LOOKUP_TABLE default\n");
    for (i = 0; i < nbpar; i++){
      el_cur = get_element_gtab(quadr, i);
      if (el_cur != NULL){
	quadr_curv = el_cur->el->el;
	fprintf(fvtk, "%lf\n", quadr_curv->curv*quadr_curv->aire);
      }
    }
    fprintf(fvtk, "\n");
    fprintf(fvtk, "SCALARS passage_inter float\n");
    fprintf(fvtk, "LOOKUP_TABLE default\n");
    for (i = 0; i < nbpar; i++){
      el_cur = get_element_gtab(quadr, i);
      if (el_cur != NULL){
	quadr_curv = el_cur->el->el;
	/*	val = quadr_curv->aire;
		if(quadr_curv->passage == 0)
		val = 0.;*/
	if(quadr_curv->bool_util)
	  fprintf(fvtk, "%lf\n", (double)quadr_curv->passage);
	else
	  fprintf(fvtk, "%lf\n", -5.);
      }
    }
    fprintf(fvtk, "\n");
  }
  if (exp_file)
    fclose(fvtk);
}

/*
 * Met tous les couples de segments que l'on peut encore utiliser
 * � inutilisable
 */
void maillage_stop_bord(Maillage *m){
  Liste *tmp;
  ListeD *el;
  Ptmaill *pt;
  int i, j;
  int ind_seg;
  Segment *seg_tmp;

  for (j = 0; j < m->lignes->nb_part; j++){
    tmp = m->lignes->bord[j]->l;
    while (tmp != NULL){
      el = tmp->el;
      pt = get_elementL2D(el);
      for (i = 0; i < 4; i++){
	ind_seg = pt->ind_seg[i];
	seg_tmp = get_element_gtab(m->seg, ind_seg);
	if (ind_seg != -1 && seg_tmp == NULL){
	  seg_tmp->b_new = 0;
	}
      }
      tmp = tmp->n;
    }
  }
}

/*
 * On met le meme nombre de lignes et de colonnes dans le maillage
 */
/*void maillage_suppr_der_ligne(Maillage *m, Param_maillage *param,
  int bool_bord,
  int ind_part,
  int or, int or_vir){
  int ind_max = m->ind_max;
  if(bool_bord){
  suppr_unused_el_bord(m->lignes, param, m, is_used_pt,
  suppr_point, m, ind_part);
  suppr_unused_el_bord(m->lignes, param, &ind_max,
  callback_suppr_ind_max,
  suppr_point, m, ind_part);
  }
  suppr_dern_ligne(m->lignes, param, suppr_point,
  m, ind_part, or, or_vir);
  }*/

/*
 * Utilis� comme callback : supprime un point
 * => supprime tous les segments qui le relient ainsi que
 * tous les ||grammes qui le contient
 * pt : type : ListeD*, le point � supprimer
 * param : type : Maillage*, le maillage
 */
void suppr_point(void *pt, void *param_call,
		 Param_maillage *param, int bool_def){
  ListeD *p = pt, *vois;
  Ptmaill *ptm = get_elementL2D(p);
  Maillage *m = (Maillage*)param_call;
  int i, ind_seg, del, nbi;
  int dir;
  nbi = 4;
  if (p->vois != NULL)
    nbi = p->vois->nb_vois;
  for (i = 0; i < nbi; i++){
    ind_seg = ptmaill_get_ind_seg(ptm, i);
    if (ind_seg != -1){
      vois = liste_getVois(p, i);
      if (p->vois != NULL)
	dir = vois->el->dir_sing;
      else{
	if (vois->vois != NULL){
	  dir = p->el->pos_sing;
	}
	else {
	  del = listeD_get_saut(p, i);
	  dir = (i + 2 + del) % 4;
	}
      }
      suppr_seg(m, vois, dir, bool_def, param);
    }
  }
  free_ptmaill(ptm);
  suppr_el_listeD(m->lignes, p,param);
}

/*
  void fusion_composantes_maillage(Maillage *m, ListeD *pt_suppr,
  ListeD *remp, int or, int or_vir){
  int ind_part = remp->el->ind_part;
  int ind_part2 = pt_suppr->el->ind_part;
  int or2 = (or+3+2*or_vir)%4;

  remplacer_point(m, pt_suppr, remp, (or+2)%4);

  merger_listes(m->lignes->bord[ind_part],
  m->lignes->bord[ind_part2],
  estBordL, m->lignes);
  merger_composante_isole(m->lignes, ind_part, ind_part2);
  }*/


/*
 * On remplace un point par un autre.
 * Utilis� dans le cas o� on doit fusionner
 * 2 maillage en disant que tel point de tel maillage est en
 * fait tel point de l'autre maillage
 */
/*void remplacer_point(Maillage *m, ListeD *pt_suppr,
  ListeD *remp, int or){
  int i, j, ind, cur;
  Segment *seg;
  Quadr *par;
  Ptmaill *npt = get_elementL2D(remp);
  Ptmaill *suppr_pt = get_elementL2D(pt_suppr);
  ListeD *vois;

  for(i = 0;i<4;i++){
  ind = suppr_pt->ind_seg[i];
  if(ind > -1){
  seg = m->seg[ind];
  if(seg->p1 == suppr_pt->p)
  seg->p1 = npt->p;
  else if(seg->p2 == suppr_pt->p)
  seg->p2 = npt->p;
  for(cur = 0;cur<2;cur++){
  par = NULL;
  if(cur == 0){
  if(seg->q1 != NULL)
  par = get_elementL2D(seg->q1);
  }
  else{
  if(seg->q2 != NULL)
  par = get_elementL2D(seg->q2);
  }
  if(par != NULL){
  for(j = 0;j<4;j++){
  if(par->pts[j] == pt_suppr)
  par->pts[j] = remp;
  }
  }
  }
  vois = liste_getVois(pt_suppr, i);
  if(i != or){
  npt->ind_seg[i] = ind;
  set_vois(remp, i, vois);
  }
  else {
  ajouter_jonction(npt, vois, ind, i);
  }
  set_vois(vois, (i+2)%4, remp);
  }
  }
  }*/

/*
 * Supprime un segment ainsi que les ||grammes qui le contiennent.
 * Attention : ne fonctionne que si l'on est pas une sing
 * l : le point de d�part du segment
 * dir : la direction du segment
 */
void suppr_seg(Maillage *m, ListeD* l, int dir, int bool_def, Param_maillage *param){
  ListeD *q;
  int i;
  Ptmaill *pt = get_elementL2D(l);
  int ind_seg = ptmaill_get_ind_seg(pt, dir);
  Segment *seg = get_element_gtab(m->seg, ind_seg);
  ListeD *vois = liste_getVois(l, dir);
  Ptmaill *pt_vois = get_elementL2D(vois);

  for (i = 0; i < 2; i++){
    if (seg->q1 != NULL)
      q = seg->q1;
    else
      q = seg->q2;
    if (q != NULL){
      supprQuadr(m, q, bool_def, param);
      suppr_el_listeD(m->quadr, q,param);
    }
  }
  if (l->vois != NULL)
    ptmaill_set_ind_seg(pt_vois, vois->el->dir_sing, -1);
  else {
    int del = listeD_get_saut(l, dir);
    ptmaill_set_ind_seg(pt_vois, (dir + 2 + del) % 4, -1);
  }
  ptmaill_set_ind_seg(pt, dir, -1);

  suppr_el_gtab(m->seg, ind_seg, param);
  free_segment(seg);
}

Pt* getvect(ListeD *l){
  Ptmaill *pt = l->el->el;
  return pt->p->pt;
}

int callback_stop_distance_pt(Maillage *m, void *param){
  Param_call1 *p = (Param_call1*)param;
  double dist_p = p->dist_p;
  Pt *tmp;
  p->prec = liste_getVois(p->prec, p->or);
  tmp = getvect(p->prec);
  p->dist_p = distance(tmp->p, p->pt->p);
  if (p->dist_p > dist_p)
    return 1;
  return 0;
}

int extraire_diag(ListeD *in, int or, int or_vir, Pt *c_res){
  int nb = 0;
  int del = 3 + 2 * or_vir;
  ListeD *tmp = in;
  while (tmp != NULL){
    ptcopy(&c_res[nb], getvect(tmp));
    nb++;
    tmp = liste_getVois(tmp, or);
    if (tmp == NULL)
      return nb;
    tmp = liste_getVois(tmp, (or + del) % 4);
  }
  return nb;
}

void export_pas_diag(ListeD *init, int or,
		     int or_vir, char *path, int bool_milieu){
  FILE *f = fopen(path, "w+");
  ListeD *tmp = init;
  Pt *prec = NULL, *cur;
  int or2 = (or + 3 + 2 * or_vir) % 4;
  int bool_in = 1;
  int it, nb = 1;

  if (bool_milieu){
    while (tmp != NULL){
      tmp = liste_getVois(tmp, or);
      if (tmp == NULL)
	break;
      tmp = liste_getVois(tmp, (or2 + 2) % 4);
      nb++;
    }
    nb /= 2;
    tmp = init;
    it = 0;
    while (it < nb){
      tmp = liste_getVois(tmp, or);
      if (tmp == NULL)
	break;
      tmp = liste_getVois(tmp, (or2 + 2) % 4);
      it++;
    }
  }
  cur = NULL;
  while (tmp != NULL){
    prec = cur;
    cur = getvect(tmp);
    if (!bool_in)
      fprintf(f, "%lf\n", distance(prec->p, cur->p));

    bool_in = 0;
    tmp = liste_getVois(tmp, or);
    if (tmp == NULL)
      break;
    tmp = liste_getVois(tmp, or2);
  }
}

void free_maillage(Maillage *m, Param_maillage *param){
  ListeD *pt_suppr;
  GTab *pts = m->lignes->mailles[0];
  GTab *quadr = m->quadr->mailles[0];
  int nbpts = pts->nb_tot;
  int nbpar = quadr->nb_tot;
  int i, nb_seg;

  PRINT_WAR_LOG(param, "AZER1\n");
  for (i = 0; i < nbpts; i++){
    pt_suppr = get_element_gtab(pts, i);
    if (pt_suppr != NULL){
      free_ptmaill(pt_suppr->el->el);
      free_mailleD(pt_suppr, 1);
    }
  }
  for (i = 0; i < nbpar; i++){
    pt_suppr = get_element_gtab(quadr, i);
    if (pt_suppr != NULL){
      freeQuadr(pt_suppr->el->el);
      free_mailleD(pt_suppr, 1);
    }
  }
  free_slisteD(m->quadr);
  free_slisteD(m->lignes);

  nb_seg = m->seg->nb_tot;
  for (i = 0; i < nb_seg; i++)
    free_segment((Segment*)get_element_gtab(m->seg, i));
  free_gtab(m->seg, NULL);
  if (m->pts_dual != NULL)
    free_dual(m);
  free(m);
  if(m->mesh_curv != NULL)
    free_mesh_curv(m->mesh_curv);
}


/*
 * Utilis� comme callback : le point est au bord ou non
 * (il a tous ses ||grammes cr��s ou non)
 */
int est_bord_pt(ListeD *pt, void *param){
  Maillage *m = (Maillage*)param;
  Segment *seg;
  Ptmaill *cour = get_elementL2D(pt);
  int i, ind;
  if (pt->vois != NULL)
    return 1;
  for (i = 0; i < 4; i++){
    ind = ptmaill_get_ind_seg(cour, i);
    if (ind == -1)
      return 1;
    seg = get_element_gtab(m->seg, ind);
    if (seg->b_new && seg->q2 == NULL)
      return 1;
  }
  return 0;
}

void maillage_set_sing(Maillage *m, ListeD *pt){
  Ptmaill *ptm = pt->el->el;
  if (pt->vois == NULL){
    listeD_set_sing(m->lignes, pt);
    ptmaill_ajouter_sing(m, ptm);
  }
}

void maillage_suppr_sing(Maillage *m, ListeD *pt){
  Ptmaill *ptm = pt->el->el;
  if (pt->vois != NULL){
    int dir[NB_BRANCHES_MAX];
    memset(dir, -1, NB_BRANCHES_MAX*sizeof(int));
    listeD_suppr_sing(m->lignes, pt, dir);
    ptmaill_suppr_sing(m, ptm, dir);
  }
}

int maillage_fermer_sing(Maillage* m, ListeD *sing){
  //ajouter une ligne de discontinuit� des voisins
  if (sing->vois != NULL && !sing->vois->est_ferme){
    Ptmaill *ptm = sing->el->el;
    int ind_seg, ind_l = 0;
    if (listeD_saut_ligne_sing(sing, 0))
      ind_l = sing->vois->nb_vois - 1;
    ind_seg = ptmaill_get_ind_seg(ptm, ind_l);
    if (ind_seg != -1){
      ListeD *l_quadr = NULL;
      Segment *seg = get_element_gtab(m->seg, ind_seg);
      int del_prec = 0;
      int i;
      int del, dir_s;
      ListeD *vois;
      int dir = (sing->vois->vois[ind_l]->el->dir_sing + 2) % 4;
      for (i = 0; i < sing->vois->nb_vois; i++){
	vois = liste_getVois(sing, i);
	if (vois != NULL && vois->saut != NULL){
	  if (vois->saut->dir1 == (vois->el->dir_sing + 3) % 4)
	    del_prec += vois->saut->delta;
	}
      }
      del = (sing->vois->nb_vois + del_prec) % 4;
      if (ind_l != 0)
	del = 4 - del;
      l_quadr = seg->q1;
      if (l_quadr != NULL){
	dir_s = listeD_fermer_sing(sing, ind_l, 0, del);
	listeD_fermer_sing(l_quadr, dir, dir_s, del);
	ptmaill_fermer_sing(ptm);
	return 1;
      }
    }
  }
  return 0;
}

/*
 * Appel� lorsqu'on ajoute un voisin � la listeD de points.
 * On cr�� un segment que l'on ajoute au maillage
 */
void callback_aj_vois(ListeD *l1, ListeD *l2,
		      int dir1, int dir2, void *param){
  Ptmaill *ptm1 = l1->el->el;
  Ptmaill *ptm2 = l2->el->el;
  Segment *seg = createSegment(ptm1->p, ptm2->p);
  Maillage *m = (Maillage*)param;
  int pos = aj_gtab(m->seg, seg);
  ptmaill_set_ind_seg(ptm1, dir1, pos);
  ptmaill_set_ind_seg(ptm2, dir2, pos);
}

/*
 * Supprime un quadrangle (et le retire de tous les segments
 * qu'il contient)
 * !!!!!! Pour le moment, on ne g�re pas la fusion de plusieurs morceaux de graphes
 * ====> on n'emp�che un segment supprim� d'�tre utilis� pour une autre construction
 */
void supprQuadr(Maillage *m, ListeD *lq, int bool_def, Param_maillage *param){
  int i, ind;
  Segment *seg;
  Quadr *q = get_elementL2D(lq);
  for (i = 0; i < 4; i++){
    ind = q->ind_seg[i];
    if (ind != -1){
      seg = (Segment*)get_element_gtab(m->seg, ind);
      suppr_quadr_seg(seg, lq);
      seg->b_new = !bool_def;
    }
  }
}

/*
 * callback appel� pour supprimer un �l�ment de la liste de quadrangles
 * supprime un quadrangle (ainsi que les segments
 * qui ne contiennent que lui)
 */
void callback_supprQuadr(void *lq, void *maill){
  Maillage *m = (Maillage*)maill;
  ListeD *l = (ListeD*)lq;
  supprQuadr(m, l, 0, NULL);
}

/*
 * Calcul le point du bord du maillage le plus proche de p
 */
double maillage_projection_bord(Maillage *m, double* p,
				ListeD **ret_l){
  Liste *tmp = m->lignes->bord[0]->l;
  double min_d = INT_MAX;
  double d;
  Ptmaill *ptm;
  ListeD *ma, *res = NULL;
  while (tmp != NULL){
    ma = tmp->el;
    ptm = get_elementL2D(ma);
    d = distance(ptm->p->pt->p, p);
    if (d < min_d){
      res = ma;
      min_d = d;
    }
    tmp = tmp->n;
  }
  ret_l[0] = res;
  return min_d;
}

/*
 * Cas o� on a ajout� un point inutilis� (une courbe qui ne pouvait pas �tre ajout�)
 * : on le supprime
 */
int free_branches(Maillage *m, Param_maillage *param, ListeD *l){
  int res = 0;
  if (l->vois != NULL){
    int nb = l->vois->nb_vois;
    int nb_vois = 0;
    int i, j;
    ListeD *aj;
    int bool_suppr = 1;
    Ptmaill *ptm;
    for (j = 0; j < nb; j++){
      aj = l->vois->vois[j];
      nb_vois = 0;
      bool_suppr = 1;
      for (i = 0; i < 4; i++){
	if (liste_getVois(aj, i) != NULL){
	  nb_vois++;
	  if (nb_vois >= 2){
	    bool_suppr = 0;
	    break;
	  }
	}
      }
      if (bool_suppr){
	suppr_point(l->vois->vois[j], m, param, 0);
	res = 1;
      }
    }

    j = 0;
    while (j < l->vois->nb_vois && l->vois->vois[j] != NULL)
      j++;
    ptm = l->el->el;
    l->vois->nb_vois = j;
    ptm->sing->nb_seg = j;
  }
  return res;
}

/* TODO : supprimer les sauts pour les ||grammes !!!!!! */
void free_saut_sing(Maillage *m, ListeD *sing, int ind){
  Saut_vois* saut = NULL;
  ListeD *tmp, *vois = liste_getVois(sing, ind);
  int del, ind_seg;
  Ptmaill *ptm = sing->el->el;
  Segment*seg;
  ListeD *quadr;
  int i;

  ind_seg = ptmaill_get_ind_seg(ptm, ind);
  seg = get_element_gtab(m->seg, ind_seg);
  quadr = seg->q1;

  for (i = 0; i < 2; i++){
    ind = (vois->el->dir_sing + 2) % 4;
    if (i == 0){
      tmp = vois;
    }
    else {
      tmp = quadr;
    }
    while (tmp != NULL){
      if (tmp->saut != NULL){
	if (tmp->saut != saut){
	  saut = tmp->saut;
	  free(saut);
	}
	tmp->saut = NULL;
      }
      del = listeD_get_saut(tmp, ind);
      tmp = liste_getVois(tmp, ind);
      ind = (ind + del) % 4;
    }
  }
}


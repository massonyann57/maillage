#include "dual.h"

Dual_param* create_dual_param(int nbpts, PListeD *pts){
  Dual_param *res = malloc(sizeof(Dual_param));
  res->nbpts = nbpts;
  res->pts = pts;
  return res;
}

void constr_dual(Maillage *m, Param_maillage *param){
  SListe *bord;
  Liste *elbord;
  ListeD *el_prim, *init = m->lignes->l[0];
  ListeD *vois, *vois2;
  PListeD tab_vois[NB_BRANCHES_MAX];
  int i, _i, j, /*_j, */k;
  int imax, ind_vois[2];
  int or;
  ListeD *vois_tmp;
  GTab *gtab_bool = init_bool_gtab(m->lignes->mailles[0]);
  int ind_tab_vois;
  int ind_aj, ind_aj_new, ind_aj2;
  PListeD pts[2*NB_BRANCHES_MAX];
  ListeD *sing, *vois_sing = NULL;
  int _del, ind_tmp;
  int *ind_quadr_tmp;
  int del_saut, /*dir_saut, */_del_saut, del_saut_tot;
  int _del2, dir_new;
  int ind_tab_init, del_sing;
  int pos_tmp, bool_suppr;
  int bool_sing_atteinte[NB_SING_MAX];
  int bool_aj_sing[NB_SING_MAX];
  int dir_sing, nbv;
  Saut_vois *new_saut;
 
  memset(bool_sing_atteinte, 0, NB_SING_MAX*sizeof(int));
  memset(bool_aj_sing, 0, NB_SING_MAX*sizeof(int));
  m->tot_pts_dual = 0;
  if(m->lignes->nb_sing[0] > 0)
    init = m->lignes->sing[0][0];
  ind_tab_init = init->el->pos_tab;

  PRINT_WAR_LOG(param, "DEBUT DUAL\n");

  ind_quadr_tmp = malloc(5 * sizeof(int));
  for (k = 0; k < 4; k++)
    ind_quadr_tmp[k] = -1;
  ind_quadr_tmp[4] = 0;
  set_element_gtab(gtab_bool, ind_tab_init, ind_quadr_tmp);
  memset(pts, 0, 2*NB_BRANCHES_MAX * sizeof(ListeD*));
  memset(tab_vois, 0, NB_BRANCHES_MAX*sizeof(ListeD*));

  m->pts_dual = createSListeD(init, NULL, NULL, NULL, NULL,
			      NULL, NULL, init->vois != NULL,
			      init->el->dir_sing != -1, -1, 0);
  if(init->vois != NULL)
    m->pts_dual->l[0]->vois->est_ferme = init->vois->est_ferme;
  m->quadr_dual = create_gtab(NULL, NULL);
  bord = createSListe(m->pts_dual->l[0], NULL, NULL, NULL, NULL);


  elbord = bord->l;
  /* on créé le saut du premier point (si nécessaire) */
  pts[0] = elbord->el;
  if(pts[0]->vois != NULL)
    pts[0]->vois->nb_vois = init->vois->nb_vois;
  if(init->saut != NULL){
    pts[0]->saut = dual_copier_saut(init->saut);
    dir_new = init->saut->dir1;
    pts[0]->saut->dir1 = dir_new;
    pts[0]->saut->dir2 = (dir_new + 1) % 4;
  }
  while (elbord != NULL){
    pts[0] = elbord->el;
    el_prim = pts[0]->el->el;
    if (el_prim->vois != NULL)
      imax = el_prim->vois->nb_vois;
    else
      imax = 4;
    for (i = 0; i < imax; i++){
      tab_vois[i] = liste_getVois(pts[0], i);
    }
    for (i = 0; i < imax; i++){
      _i = i + 1;
      if (el_prim->vois != NULL){
	if (el_prim->vois->est_ferme)
	  _i = _i%el_prim->vois->nb_vois;
      }
      else
	_i = _i % 4;
      if (/*tab_vois[i] == NULL || tab_vois[_i] == NULL*/1){
	vois = liste_getVois(el_prim, i);
	if (vois != NULL){
	  if (el_prim->vois != NULL){
	    or = (vois->el->dir_sing + 2) % 4;
	  }
	  else
	    or = i;
	  _del_saut = listeD_get_saut(el_prim, i);
	  if (vois->vois != NULL){
	    // les voisins sont +1 et -1
	    ind_vois[0] = el_prim->el->pos_sing + 1;
	    if (ind_vois[0] == vois->vois->nb_vois){
	      if (vois->vois->est_ferme)
		ind_vois[0] = 0;
	      else
		ind_vois[0] = -1;
	    }
	    ind_vois[1] = el_prim->el->pos_sing - 1;
	    if (ind_vois[1] < 0 && vois->vois->est_ferme)
	      ind_vois[1] = vois->vois->nb_vois - 1;
	  }
	  else {
	    if (/*tab_vois[i] == NULL*/1)
	      ind_vois[0] = (or + 3) % 4;
	    else
	      ind_vois[0] = -1;
	    if (/*tab_vois[_i] == NULL*/1)
	      ind_vois[1] = (or + 1) % 4;
	    else
	      ind_vois[1] = -1;
	  }
	  if (_del_saut > 0){
	    if (ind_vois[0] >= 0)
	      ind_vois[0] = (ind_vois[0] + _del_saut) % 4;
	    if (ind_vois[1] >= 0)
	      ind_vois[1] = (ind_vois[1] + _del_saut) % 4;
	  }
	  for (j = 0; j < 2; j++){
	    //_j = 1-j;
	    if (ind_vois[j] != -1){
	      vois2 = liste_getVois(vois, ind_vois[j]);
	      if (vois2 != NULL){
		/* on l'ajoute s'il n'existe pas
		 * dans le cas contraire, on regarde si
		 * on peut construire un ||gramme
		 */
		del_saut = listeD_get_saut(vois, ind_vois[j]);
		del_saut_tot = del_saut + _del_saut;
		if (pts[0]->vois == NULL){
		  ind_aj = i + j;
		  /*	if (_del_saut>0)
			ind_aj = (ind_aj + _del_saut) % 4;*/
		}
		else{
		  ind_aj = vois2->el->primal_pos_sing;
		  ind_aj_new = (vois->el->dir_sing+j) % 4;
		  if (del_saut_tot > 0){
		    ind_aj_new = (ind_aj_new + del_saut_tot) % 4;
		  }
		}
		ind_tab_vois = vois2->el->pos_tab;
		ind_quadr_tmp = get_element_gtab(gtab_bool,
						 ind_tab_vois);
		if (ind_quadr_tmp == NULL){
		  if (vois2->vois != NULL){
		    ind_aj_new = 0;
		  }
		  sing = NULL;
		  if(pts[0]->vois != NULL){
		    sing = el_prim;
		    vois_sing = vois2;
		    dir_sing = ind_aj_new;
		  }	  
		  if(vois2->vois != NULL){
		    sing = vois2;
		    vois_sing = el_prim;
		    dir_sing = ind_aj;
		  }
		  if(sing != NULL){
		    /* le maillage passe par une singularitée */
		    if(!bool_sing_atteinte[sing->vois->ind_sing]){
		      dual_init_ind_vois_sing(m->pts_dual, sing, 
					      vois_sing, dir_sing, 
					      gtab_bool);
		      bool_sing_atteinte[sing->vois->ind_sing] = 1;	
		      if (pts[0]->vois != NULL)
			ind_aj = vois2->el->primal_pos_sing;
		    }
		  }

		  //  PRINT_WAR_LOG(param, "avant ajout point\n");
		  new_saut = NULL;
		  sing = NULL;
		  if(pts[0]->vois != NULL)
		    sing = el_prim;
		  if(el_prim->el->pos_sing != -1 && el_prim->saut == NULL){
		    sing = liste_getVois(el_prim, el_prim->el->dir_sing);
		  } 
		  if(sing != NULL){
		    nbv = sing->vois->nb_vois;
		    if((sing == el_prim && ind_aj == nbv-1
			&& sing->vois->vois[0]->saut != NULL)
		       || (el_prim->el->dir_sing == nbv-1)){
		      if(sing->vois->vois[0]->saut!=NULL){
			new_saut = dual_copier_saut(sing->vois->vois[0]->saut);
			new_saut->dir1 = (new_saut->delta+new_saut->dir1+2)%4;
			new_saut->delta = 4-new_saut->delta;
		      }
		    }
		    else if((sing == el_prim && ind_aj == 0
			     && sing->vois->vois[nbv-1]->saut != NULL)
			    || el_prim->el->dir_sing == 0){
		      if(sing->vois->vois[nbv-1]->saut!=NULL){
			new_saut = dual_copier_saut(sing->vois->vois[nbv-1]->saut);
			new_saut->dir1 = (new_saut->delta+new_saut->dir1+3)%4;
			new_saut->delta = 4-new_saut->delta;
		      }
		    }
		    if(el_prim->el->pos_sing != -1 && new_saut != NULL){
		      pts[0]->saut = new_saut;
		      new_saut = NULL;
		    }
		  }
		  pts[1] =
		    ajouter_listeD(m->pts_dual,
				   pts[0], vois2, NULL, ind_aj, new_saut,
				   ind_aj_new,
				   vois2->vois != NULL, 
				   vois2->el->dir_sing != -1, 
				   vois2->el->primal_pos_sing, param);
		  if(pts[1]->vois != NULL){
		    pts[1]->vois->nb_vois = vois2->vois->nb_vois;
		  }
		  if(vois2->vois != NULL)
		    pts[1]->vois->est_ferme = vois2->vois->est_ferme;
	//	  PRINT_WAR_LOG(param, "après ajout point\n");
	

		  if (vois2->saut == NULL){
		    if(pts[1]->saut != NULL){
		      /* on supprime ce saut sauf pour un point
		       * au voisinage de la singularité
		       */
		      if(vois2->el->dir_sing != -1){
			pos_tmp = pts[1]->saut->dir1;
			if(vois2->el->dir_sing == pos_tmp)
			  pos_tmp = (pos_tmp+3)%4;
			vois_tmp = liste_getVois(vois2, pos_tmp);
			if(vois_tmp == NULL || vois_tmp->saut == NULL)
			  bool_suppr = 1;
			else bool_suppr = 0;
		      } else
			bool_suppr = 1;
		      if(bool_suppr){
			if (pts[1]->saut != NULL)
			  free(pts[1]->saut);
			pts[1]->saut = NULL;
		      }
		    }
		  }
		  else {
		    if (pts[1]->saut == NULL)
		      pts[1]->saut = dual_copier_saut(vois2->saut);
		    if (vois2->saut != NULL){
		      dir_new = vois2->saut->dir1;
		      pts[1]->saut->delta = vois2->saut->delta;
		      vois_tmp = liste_getVois(pts[1], dir_new + 1);
		      if(pts[1]->el->dir_sing != dir_new){
			pts[1]->saut->dir1 = dir_new;
			if (vois_tmp == NULL || vois_tmp->vois == NULL)
			  pts[1]->saut->dir2 = (dir_new + 1) % 4;
		      } else if (vois_tmp == NULL 
				 || vois_tmp->vois == NULL){
			pts[1]->saut->dir1 = (dir_new + 1) % 4;
			pts[1]->saut->dir2 = -1;
		      } else {
			free(pts[1]->saut);
			pts[1]->saut = NULL;
		      }
		    }
		  }

		  ind_quadr_tmp = malloc(5 * sizeof(int));
		  ind_quadr_tmp[4] = pts[1]->el->pos_tab;
		  for (k = 0; k < 4; k++)
		    ind_quadr_tmp[k] = -1;
		  set_element_gtab(gtab_bool, ind_tab_vois,
				   ind_quadr_tmp);
		  ajouter_fin_liste(bord, pts[1]);
		}
		else{
		  pts[1] = liste_getVois(pts[0], ind_aj);
		  if (pts[1] == NULL){
		    pts[1] = get_element_SListeD(m->pts_dual, 0,
						 ind_quadr_tmp[4]);
		    if (pts[0]->vois == NULL ||
			pts[1]->el->dir_sing == -1){
		      if (_del_saut == 0){
			ind_aj2 = (ind_aj + 2+del_saut) % 4;
			//	PRINT_WAR_LOG(param, "avant ajout vois\n");
			if(pts[1]->vois != NULL){
			  ajouter_vois(m->pts_dual, pts[1], pts[0], 
				       vois2->el->pos_sing, ind_aj, param);
			} else {
			  if(pts[1]->saut == NULL)
			    ajouter_vois(m->pts_dual, pts[0], pts[1],
					 ind_aj, ind_aj2, param);
			  else
			    ajouter_vois(m->pts_dual, pts[1], pts[0],
					 ind_aj2, ind_aj, param);
			    
			}
			//	PRINT_WAR_LOG(param, "après ajout vois\n");
		      }
		      else {
			// on ne l'ajoute pas au voisin si on a traversé un saut au début
			pts[1] = NULL;
		      }
		    }
		  }
		}

		if (pts[1] != NULL){
		  /* on fait le même parcours que dans ajouter_listeD
		   * et on ajoute toutes les possibilités...
		   */
		  _del2 = listeD_get_saut(pts[0], ind_aj);
		  for (k = 0; k < 2; k++){
		    _del = 0;
		    if (pts[1]->vois == NULL){
		      if (pts[0]->vois == NULL)
			ind_tmp = (2 * k + 1 + ind_aj + _del2) % 4;
		      else
			ind_tmp = (2 * k + 1 + ind_aj_new + 2) % 4;
		    }
		    else {
		      ind_tmp = 1 - 2 * k + pts[0]->el->pos_sing;
		      if (pts[1]->vois->est_ferme){
			if (ind_tmp < 0)
			  ind_tmp = pts[1]->vois->nb_vois - 1;
			if (ind_tmp == pts[1]->vois->nb_vois)
			  ind_tmp = 0;
		      }
		      else if (ind_tmp == pts[1]->vois->nb_vois)
			ind_tmp = -1;
		    }
		    if (ind_tmp != -1){
		      _del = listeD_get_saut(pts[1], ind_tmp);
		      pts[2] = liste_getVois(pts[1], ind_tmp);
		      if (pts[2] != NULL){
			if(!dual_autour_sing(pts, gtab_bool, 
					     m->quadr_dual, 
					     ind_tmp,
					     &m->tot_pts_dual, bool_aj_sing)){
			  /* on choisit le 3eme point : dans le cas où le
			   * 2eme point n'est pas une singularité c'est assez simple.
			   * Dans le cas contraire, il faut déterminer si 
			   * la direction est +1 ou -1
			   */
			  if (pts[2]->vois != NULL){
			    ind_tmp = (pts[1]->el->pos_sing + 1);
			    if(pts[2]->vois->est_ferme)
			      ind_tmp %= pts[2]->vois->nb_vois;
			    pts[3] = liste_getVois(pts[2], ind_tmp);
			    del_sing = get_saut_sing(pts[3], pts[1]);

			    if(pts[3] == NULL 
			       || (pts[3]->el->dir_sing+del_sing+4-_del2)%4 == (ind_aj+2)%4){
			      ind_tmp = (pts[1]->el->pos_sing - 1);
			      if(ind_tmp < 0 && pts[2]->vois->est_ferme)
				ind_tmp = pts[2]->vois->nb_vois-1;
			      if(ind_tmp >= 0){
				pts[3] = liste_getVois(pts[2], ind_tmp);
	      
				del_sing = get_saut_sing(pts[3], pts[1]);
				if(pts[3] == NULL 
				   || (pts[3]->el->dir_sing+del_sing+4-_del2)%4 == (ind_aj+2)%4)
				  ind_tmp = -1;
			      }
			    }
			  }
			  else{
			    if (pts[0]->vois == NULL){
			      if(pts[1]->vois == NULL)
				ind_tmp = (2 + ind_aj + _del2) % 4;
			      else{
				del_sing = get_saut_sing(pts[0], pts[2]);
				ind_tmp = (pts[0]->el->dir_sing+2+del_sing)%4;
			      }
			    }
			    else
			      ind_tmp = ind_aj_new;

			  }
			  if(ind_tmp >= 0){
			    pts[3] = liste_getVois(pts[2], ind_tmp + _del);
			    if (pts[3] != NULL){
			      //      PRINT_WAR_LOG(param, "ajout quadr\n");
			      dual_aj_tab(pts, 4, gtab_bool, m->quadr_dual, 
					  &m->tot_pts_dual);
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    if (pts[0]->vois != NULL){
      if (pts[0]->vois->nb_vois == el_prim->vois->nb_vois)
	pts[0]->vois->est_ferme = el_prim->vois->est_ferme;
    }
    suppr_el_liste(bord, elbord);
    elbord = bord->l;
  }
  free_gtab(gtab_bool, free);
  PRINT_WAR_LOG(param, "FIN DUAL\n");
}

int dual_autour_sing(PListeD *pts, GTab *gtab_bool, 
		     GTab *quadr, int dir_suiv, 
		     int *tot_pts, int *bool_aj_sing){
  int i;
  ListeD *primal;
  ListeD *sing = NULL, *tmp2;
  for(i = 0;i<3;i++){
    primal = pts[i]->el->el;
    if(primal->el->pos_sing == -1)
      return 0;
    else if(sing == NULL)
      sing = liste_getVois(primal, primal->el->dir_sing);
  }
  if(bool_aj_sing[sing->vois->ind_sing])
    return 1;
  if(sing->vois->est_ferme){
    int nb_tot = 3;
    ListeD *tmp;
    ListeD *prim;
    int ind = 1;
    int saut;
    tmp = pts[2];
    dir_suiv = (dir_suiv+listeD_get_saut(pts[1], dir_suiv)+ind)%4;
    while(tmp != NULL && tmp != pts[0]){
      tmp2 = liste_getVois(tmp, dir_suiv);
      if(tmp2 == NULL){
	if(ind == 3){
	  tmp = NULL;
	  break;
	} else {
	  tmp2 = liste_getVois(tmp, dir_suiv+2);
	  if(tmp2 == NULL){
	    tmp = NULL;
	    break;
	  }
	}
      }
      prim = tmp2->el->el;
      if(prim->el->pos_sing == -1){
	if(ind == 1){
	  dir_suiv = (dir_suiv+2)%4;
	  tmp2 = liste_getVois(tmp, dir_suiv);
	  if(tmp2 == NULL){
	    tmp = NULL;
	    break;
	  }
	  prim = tmp2->el->el;
	  if(prim->el->pos_sing != -1)
	    ind = 3;
	  else {
	    tmp = NULL;
	    break;
	  }
	}
	else break;
      }
      saut = listeD_get_saut(tmp, dir_suiv);
      tmp = tmp2;
      if(tmp == pts[0])
	break;
      pts[nb_tot] = tmp;
      nb_tot++;
      dir_suiv = (dir_suiv+saut+ind)%4;
    }
    if(tmp != NULL){
      dual_aj_tab(pts, nb_tot, gtab_bool, quadr, tot_pts);
      bool_aj_sing[sing->vois->ind_sing] = 1;
    }
  }
  return 1;
}

int dual_aj_tab(PListeD *pts, int nbpts, 
		GTab *gtab_bool, GTab *quadr, int* tot_pts){
  int i = 0, j, pos;
  int *ind;
  ListeD *l;
  Dual_param *param;
  PListeD *q;
  if(nbpts == 4){
    while (pts[i]->vois != NULL)
      i++;
    l = pts[i]->el->el;
    ind = get_element_gtab(gtab_bool, l->el->pos_tab);
    if (ind != NULL){
      for (j = 0; j < 4; j++){
	if (ind[j] == -1)
	  break;
	param = get_element_gtab(quadr, ind[j]);
	if (param != NULL && param->nbpts == 4){
	  if (quadr_contient_pts(param->pts, pts))
	    return 0;
	}
      }
    }
  }
  // on ajoute le "quadrangle"
  q = malloc(nbpts*sizeof(ListeD*));
  pos = aj_gtab(quadr, create_dual_param(nbpts, q));
  tot_pts[0] += nbpts;
  for (i = 0; i < nbpts; i++){
    q[i] = pts[i];
    if(pts[i]->vois == NULL){
      l = pts[i]->el->el;
      ind = get_element_gtab(gtab_bool, l->el->pos_tab);
      j = 0;
      while (j<4 && ind[j] >= 0)
	j++;
      if(j==4){
	printf("erreur construction dual\n");
	//	PRINT_WAR_LOG("erreur construction dual\n");
      }
      ind[j] = pos;
    }
  }
  return 1;
}

Saut_vois* dual_copier_saut(Saut_vois *s){
  if (s == NULL)
    return NULL;
  return create_saut_vois(s->dir1, s->delta, s->dessus);
}

void dual_init_ind_vois_sing(SListeD *dual, ListeD *sing, 
			     ListeD *vois, 
			     int dir_vois, GTab *gtab_bool){
  ListeD *tmp = vois, *tmp2;
  int bool_init = 1;
  int dir_suiv = dir_vois;
  int ind = -1;
  int *ind_pts;
  ListeD *el_dual;

  if(!sing->vois->est_ferme){
    tmp = sing->vois->vois[0];
    dir_suiv = (tmp->el->dir_sing+3)%4;
    tmp2 = liste_getVois(tmp, dir_suiv);
    dir_suiv = (dir_suiv + listeD_get_saut(tmp, dir_suiv)+1)%4;
    vois = tmp2;
  }
  tmp = vois;
  while(tmp != NULL && (bool_init || tmp != vois)){
    tmp2 = liste_getVois(tmp, dir_suiv);
    if(tmp2 == NULL)
      break;
    tmp->el->primal_pos_sing = tmp2->el->pos_sing;
    ind_pts = get_element_gtab(gtab_bool, tmp->el->pos_tab);
    if(ind_pts != NULL){
      el_dual = get_element_gtab(dual->mailles[0], ind_pts[4]);
      el_dual->el->primal_pos_sing = tmp2->el->pos_sing;
    }
    dir_suiv = (dir_suiv+listeD_get_saut(tmp, dir_suiv))%4;
    if(ind<0){
      if(tmp2->el->dir_sing == (dir_suiv+1)%4)
	ind = 1;
      else
	ind = 3;
    }
    bool_init = 0;
    tmp = liste_getVois(tmp2, dir_suiv);
    dir_suiv = (dir_suiv+listeD_get_saut(tmp2, dir_suiv)+ind)%4;
  }
}

/* Retourne 1 si le quadrangle contient les mêmes points */
int quadr_contient_pts(PListeD *q, PListeD *pts){
  int i, j;
  int bool;
  for(i = 0;i<4;i++){
    bool = 0;
    for(j = 0;j<4;j++){
      if(pts[i] == q[j]){
	bool = 1;
	break;
      }
    }
    if(!bool)
      return 0;
  }
  return 1;
}

void free_dual(Maillage *m){
  GTab *pts = m->pts_dual->mailles[0];
  int nbpts = pts->nb_tot;
  ListeD *pt_suppr;
  int i;

  m->tot_pts_dual = 0;
  for (i = 0; i < nbpts; i++){
    pt_suppr = get_element_gtab(pts, i);
    if (pt_suppr != NULL){
      //   free_ptmaill(pt_suppr->el->el);
      free_mailleD(pt_suppr, 1);
    }
  }
  free_slisteD(m->pts_dual);
  free_gtab(m->quadr_dual, free);
  m->pts_dual = NULL;
  m->quadr_dual = NULL;
}

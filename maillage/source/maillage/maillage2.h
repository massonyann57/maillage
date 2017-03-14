#include "maillage.h"

/*
 * constructeur Maillage
 */
Maillage* createMaillage(Pt* init, double h, 
			 Surf *s, int nbptr_max){
  Maillage *res = malloc(sizeof(Maillage));
  Ptmaill *new = NULL;
  int (*cmp)(void*,void*,void*) = NULL;
  
  if(init != NULL)
    new = createPtmaill(init);
  res->lignes = createSListeD((void*) new, 
			      cmp, print_ptMaill, 
			      est_bord_pt, (void*)res, callback_aj_vois, 
			      res, 0,0);
  res->quadr = createSListeD(NULL, NULL, print_el_quadr, NULL,
			     NULL, NULL,NULL,0,0);
  res->nb_seg = 0;
  res->h = h;

  res->ind_max = INT_MAX;
  res->s = s;
  res->nbptr_max = nbptr_max;
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
 * ind_surf : pas utilisé pour le moment
 * p_maill : le point précédent 
 * new_p : le nouveau point
 * or : la direction de l'ajout du ||gramme
 * boo_sing : est une singularitée ou non
 * (si oui, on ajoute le point mais les segments créés ne pourront 
 * plus être utilisés)
 */
void ajouterPoint_comp(Maillage *m, Pt* new_p, 
		       ListeD *p_maill, int ind_surf, int or, 
		       int boo_sing){
  ListeD *newl;
  Ptmaill *newp = createPtmaill(new_p);
  ListeD *p2;
  ListeD *p3;
  int or_aj_pt = or;
  int or_voistopt = (or+2)%4;
  int est_etoile = 0;
  
  int or_p2 = or+3;
  int _del;
  if(p_maill->vois != NULL)
    or_p2 = or-1;
  _del = listeD_get_saut(p_maill, or_p2);

  or_aj_pt += _del;
  p2 = liste_getVois(p_maill, or_p2);
  p3 = liste_getVois(p_maill, or);

  if(p2->vois != NULL){
    or_voistopt = (p_maill->el->dir_sing+1)%4;
    or_aj_pt = p_maill->el->pos_sing+1;
    est_etoile = 1;
  }
  if(p_maill->vois != NULL){
    or_aj_pt = (p2->el->dir_sing+3)%4;
    or_voistopt = (p2->el->dir_sing+1)%4;
    est_etoile = 1;
  }
  /*if(p_maill->vois != NULL){
    if(!p_maill->vois->est_ferme 
       || or != p_maill->vois->nb_vois){
      or_aj_pt = (p2->el->dir_sing+3)%4;
      or_voistopt = (p2->el->dir_sing+1)%4;
    } else {
      or_aj_pt = (p2->el->dir_sing+2)%4;
      or_voistopt = (p2->el->dir_sing)%4;
    }
    est_etoile = 1;
    }*/
  if(p3->vois != NULL)
    est_etoile = 1;
  

  /* on ajoute à la liste de points */
  newl = ajouter_listeD(m->lignes, p2, (void*) newp, 
			NULL, or_aj_pt, or_voistopt, 0,0);
  ajouterQuadr(m, newl, p_maill, ind_surf, or, est_etoile, _del);
}

/*
 * On a déja 3 des 4 points et on complète le ||gramme.
 * p1 : le 1er point
 * p2 : point en face de p1
 * ind_surf : inutilisé pour le moment
 * or : la direction du suivant de p1
 */
/*void ajouterQuadr_3pts(Maillage *m, ListeD *p1, 
		       ListeD *p2, int ind_surf, int or){
  Ptmaill *pt2 = get_elementL2D(p2);
  Ptmaill *pt_suiv2;
  ListeD *suiv = liste_getVois(p1, or);
  ListeD *suiv2;
  Segment *nseg;
  int nb_seg = m->nb_seg;
  int i;
  int or2;

  m->nb_seg++;
  for(i = 1;i<=3;i+=2){
    if(liste_getVois(suiv, (or+i)%4) == p2){
      suiv2 = liste_getVois(p1, (or+i)%4);
      pt_suiv2 = get_elementL2D(suiv2);
      nseg = createSegment(pt2->p, pt_suiv2->p);
      m->seg[nb_seg] = nseg;
      pt_suiv2->ind_seg[or] = nb_seg;
      pt2->ind_seg[(or+2)%4] = nb_seg;

      ajouter_vois(m->lignes, suiv2, p2, or, 0);
      break;
    }
  }
  if(i==3)
    or2 = or;
  else if(i == 1)
    or2 = (or+1)%4;
  else
    printf("erreur ajouterQuadr_3pts\n");
  ajouterQuadr(m, p2, p1, 0, or2);
}*/

/*
 * Ajoute un point et un ||gramme 
 * ind_surf : pas utilisé pour le moment
 * p_maill : le point précédent 
 * new_p : le nouveau point
 * dir_ros : l'indice du point qui est une rosace (0 si aucun des 2)
 * or : la direction de l'ajout du ||gramme
 * boo_sing : est une singularitée ou non
 * (si oui, on ajoute le point mais les segments créés ne pourront 
 *    plus être utilisés)
 */
void ajouterQuadr(Maillage *m, ListeD *newl, ListeD *p_maill, 
		  int ind_surf, int or, int est_etoile, int del){
  ListeD *p2;
  ListeD *p3;
  Ptmaill *pt_cour = get_elementL2D(p_maill);
  Ptmaill *pt_new = get_elementL2D(newl);
  
  Quadr *newq;
  Segment *seg_vois1 = NULL, *seg_vois2 = NULL;
  PListeD pquadr[4];
  ListeD *nlquadr = NULL;
  int ind_seg;
  int aj_vois1 = 0;

  int or2a, or2b;
  int or_voistopt = (or+2)%4;
  int indseg1;
  int indseg2;

  Segment *seg1;
  Segment *seg2;

  p3 = liste_getVois(p_maill, or);
  ind_seg = ptmaill_get_ind_seg(pt_cour, or);
  seg_vois2 = m->seg[ind_seg];
  if(p_maill->vois == NULL){
    p2 = liste_getVois(p_maill, or+3);
    ind_seg = ptmaill_get_ind_seg(pt_cour, (or+3)%4);
    seg_vois1 = m->seg[ind_seg];
  }
  else{
    p2 = liste_getVois(p_maill, or-1);
    ind_seg = ptmaill_get_ind_seg(pt_cour, or-1);
    seg_vois1 = m->seg[ind_seg];
  }
  if(p2->vois != NULL)
    or_voistopt = (p_maill->el->dir_sing+1)%4;
  else if(p_maill->vois != NULL)
    or_voistopt = (p2->el->dir_sing+1)%4;

  indseg1 = ptmaill_get_ind_seg(pt_new, (or_voistopt+del+3)%4);
  indseg2 = ptmaill_get_ind_seg(pt_new, or_voistopt+del);
  seg1 = m->seg[indseg1];
  seg2 = m->seg[indseg2];

  pquadr[0] = p_maill;  pquadr[1] = p2;
  pquadr[2] = newl;  pquadr[3] = p3;
  
  newq = createQuadr(pquadr, ind_surf, 0);

  if(seg_vois1->q1 != NULL){
    int ind_el_par = 0;
    if(appartientQuadr(get_elementL2D(seg_vois1->q1), p3))
      ind_el_par = 1;
    if(ind_el_par)
      or2a = (or_voistopt+1)%4;
    else
      or2a = (or_voistopt+2)%4;
      
    nlquadr = ajouter_listeD(m->quadr, seg_vois1->q1,
			     newq, NULL, or2a, -1, 0, est_etoile);
    aj_vois1 = 1;
  }
  if(seg_vois2->q1 != NULL){
    int ind_el_par = 0;
    if(appartientQuadr(get_elementL2D(seg_vois2->q1), p3))
      ind_el_par = 1;
    if(ind_el_par)
      or2b = (or_voistopt+1)%4;
    else
      or2b = (or_voistopt+2)%4;
    if(!aj_vois1){
      nlquadr = ajouter_listeD(m->quadr, seg_vois2->q1,
			       newq, NULL, or2b, 0,0,est_etoile);
    }
    else if(nlquadr->el->ind_part != seg_vois2->q1->el->ind_part){
      /* le voisin était jusque là isolé
       * => on cherche à la main un autre voisin
       *   permettant de se raccrocher à la composante connexe
       *   principale
       */
      int part_iso, part_gr;
      
      ajouter_vois(m->quadr, seg_vois2->q1, nlquadr,
		   or2b, 0);
      if(nlquadr->el->ind_part > seg_vois2->q1->el->ind_part){
	part_gr = seg_vois2->q1->el->ind_part;
	part_iso = nlquadr->el->ind_part;
	merger_composante_isole(m->quadr, part_gr, part_iso);

	listeD_changer_partition(m->quadr, nlquadr, part_gr);
	ajouter_bord(m->quadr, seg_vois1->q1, 
		     m->quadr->bord[part_gr]->fin);
	ajouter_bord(m->quadr, nlquadr,
		     m->quadr->bord[part_gr]->fin);
      } else{
	part_gr = nlquadr->el->ind_part;
	part_iso = seg_vois2->q1->el->ind_part;

	merger_composante_isole(m->quadr, part_gr, part_iso);
	ajouter_bord(m->quadr, seg_vois2->q1, 
		     m->quadr->bord[part_gr]->fin);
      }
    }
  } else if(!aj_vois1){
    nlquadr = ajouter_listeD(m->quadr, NULL, newq, NULL, 
			     or, -1, 0,est_etoile);
  }

  seg_ajouter_quadr(seg_vois1, nlquadr);
  seg_ajouter_quadr(seg_vois2, nlquadr);
  seg_ajouter_quadr(seg1, nlquadr);
  seg_ajouter_quadr(seg2, nlquadr);

  actualiser_bord(m->lignes, p2, or, newl->el->ind_part);
}

/* Ajoute le segment pour un nouveau point
 * init : le point précédent
 * or : la direction de l'ajout
 * lnewp : le nouveau point
 */
void ajoute_vois_seg(Maillage *m, ListeD *init, int or,
		     int or_voistopt, ListeD *lnewp){
  Ptmaill *pt_cour, *newp = get_elementL2D(lnewp);
  Segment *segm;
  int nb_seg = m->nb_seg;

  pt_cour = get_elementL2D(init);

  segm = createSegment(pt_cour->p, newp->p);
  segm->b_new = 1;

  m->seg[nb_seg] = segm;
  m->nb_seg++;

  ptmaill_set_ind_seg(pt_cour, or, nb_seg);
  ptmaill_set_ind_seg(newp, or_voistopt, nb_seg);
}

/* Ajoute un point (sans ajouter de ||grammes) 
 * ajoute le point à la liste puis créé les segments
 * or : la direction dans laquelle on ajoute newp
 */
void ajoute_pt(Maillage *m, ListeD *init, int or,
	       int or_voistonew, Ptmaill *newp){
  ajouter_listeD(m->lignes, 
		 init, (void*) newp, NULL,
		 or, or_voistonew, 0,0);
  /*  if(init->vois == NULL)
      or_voistonew = (or+2)%4;*/
  /*  ajoute_vois_seg(m, init, or, or_voistonew, lnewp);*/
}

/* Ajoute un point (sans ajouter de ||grammes) 
 * ajoute le point à la liste puis créé les segments
 */
void ajoute_pt_exis(Maillage *m, ListeD *init, int or,
		    Ptmaill *newp, ListeD *nl){
  /*ListeD* lnewp = */ajouter_listeD(m->lignes, 
				 init, (void*) newp, nl,
				 or, -1, 0,0);
  /*  ajoute_vois_seg(m, init, or, (or+2)%4, lnewp);*/
}

/* Ajoute une courbe : /!\ on n'ajoute pas le premier point
 * or : 0 -> vers le haut ... puis on tourne dans le sens +
 */
void ajoute_courbe_maill(Maillage *m, Pt *pts, 
			 int nb, int ordre, 
			 ListeD *init, int or, 
			 double tol_pas){
  int i;
  ListeD *pt_cour = init;
  ListeD *suiv;
  //  PListeD vois[4], vois2[4];
  //  int _or;
  Pt *pt;

  for(i = 1;i<nb;i++){
    /*for(j = 1;j<=3;j+=2){
      vois[j] = liste_getVois(pt_cour, (or+j)%4);
         if(vois[j] != NULL){
	vois2[j] = liste_getVois(vois[j], or);
	}
	}*/
    pt = createPtN();
    if(!ordre)
      ptcopy(pt, &pts[nb-i-1]);
    else 
      ptcopy(pt, &pts[i]);
    
    ajoute_pt(m, pt_cour, or, -1, createPtmaill(pt));
    suiv = liste_getVois(pt_cour, or);
    /*  for(j = 1;j<=3;j+=2){
      if(vois[j] != NULL && vois2[j] != NULL){
	// On regarde si on doit ajouter les voisins
	// (est-on dans le cas d'une singularitée ou pas?)
	//p1 = get_elementL2D(suiv);
	//  p2 = get_elementL2D(vois2[j]);
	//	dist = distance(p1->p->pt, p2->p->pt);
	if(j==1)
	  _or = or;
	else
	  _or = (or+1)%4;
	//ajoute_vois_seg(m, vois2[j], (or+j+2)%4, (or+j)%4, suiv);
	ajouterQuadr(m, suiv, vois[j], 0, _or,0);
	}
    }*/
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
			   char *path,  char *nom, int ind_part, 
			   Mapping map, void *param_map, 
			   P3D **exp_pts, _QUADR **exp_quadr, 
			   int *res_size, int _free){
  char namestat[SIZE_TAB];
  FILE *fvtk;
  int i, k, nbpts, nbpar;
  Pt* ptmp;
  Ptmaill *tmp;
  int _ind;
  Quadr *el_quadr;
  ListeD *el_cur;
  P3D pt_aff;
  int exp_file = 1;
  int exp_rhino = (exp_pts != NULL);
  GTab *pts = m->lignes->mailles[ind_part];
  GTab *quadr = m->quadr->mailles[ind_part];
  int ind_quadr[4];

  nbpts = pts->nb_tot;
  nbpar = quadr->nb_tot;

  PRINT_MESSAGE_LOG(param, "init export\n");
  PRINT_MESSAGE_LOG(param, "fin liste export\n");
  
  if (m->quadr->nb_part != m->lignes->nb_part){
    PRINT_WAR_LOG(param, "attention, erreur possible : les 2 graphes n'ont pas le même nombre de composantes connexes\n");
  }

  PRINT_MESSAGE_LOG(param, "exportA\n");
  if (exp_rhino){
    res_size[0] = nbpts;
    res_size[1] = nbpar;
    exp_pts[0] = malloc(nbpts*sizeof(P3D));
    exp_quadr[0] = malloc(nbpar*sizeof(_QUADR));
  }

  PRINT_MESSAGE_LOG(param, "exportB\n");
  if (exp_file){
    sprintf(namestat, "%s%s", path, nom);
    fvtk = fopen(namestat, "w+");
    if (fvtk == NULL){
      param->err = IO_ERROR;
      PRINT_ERR_LOG(param, "impossible d'ouvrir le fichier : %s\n", namestat);
      return;
    }
    fprintf(fvtk, "# vtk DataFile Version 2.0\n");
    fprintf(fvtk, "t = 0\n");
    fprintf(fvtk, "ASCII\nDATASET POLYDATA\n");

    fprintf(fvtk, "POINTS %d float\n", nbpts);
  }

  PRINT_MESSAGE_LOG(param, "exportC\n");  
  for (i=0;i<nbpts;i++){
    el_cur = get_element_gtab(pts, i);
    tmp = el_cur->el->el;
    ptmp = tmp->p->pt;
    if(map != NULL)
      map(ptmp->p, pt_aff, param_map);
    else
      VECT_CPY(pt_aff, ptmp->p);
    if (exp_file){
      fprintf(fvtk, "%lf %lf %lf\n", pt_aff[0],
	      pt_aff[1], pt_aff[2]);
    }
    if(exp_rhino){
      VECT_CPY(exp_pts[0][i], pt_aff);
    }
  }
  PRINT_MESSAGE_LOG(param, "exportD\n");
  if (exp_file)
    fprintf(fvtk, "\n");
  
  if(nbpar > 0){
    if (exp_file)
      fprintf(fvtk, "POLYGONS %d %d\n", nbpar, 6*nbpar);
    _ind = 0;
    for(i=0;i<nbpar;i++){
      el_quadr = ((ListeD*)get_element_gtab(quadr, i))->el->el;
      for(k = 0;k<4;k++)
	ind_quadr[k] = el_quadr->pts[k]->el->pos_tab;
      
      if (exp_file){
	fprintf(fvtk, "5 %d %d %d %d %d\n",
		ind_quadr[0], ind_quadr[1], ind_quadr[2],
		ind_quadr[3], ind_quadr[0]);
      }
      if(exp_rhino){
	for (k = 0; k < 4; k++)
	  exp_quadr[0][_ind][k] = ind_quadr[k];
      }
      _ind++;
    }
    if (exp_file)
      fprintf(fvtk, "\n");
  }
  if(exp_file){
    fprintf(fvtk, "POINT_DATA %d\n", nbpts);
    fprintf(fvtk, "SCALARS the int\n");
    fprintf(fvtk, "LOOKUP_TABLE default\n");
    for (i=0;i<nbpts;i++){
      el_cur = get_element_gtab(pts, i);
      fprintf(fvtk, "%d\n", estBordL2D(m->lignes, el_cur));
    }
    fprintf(fvtk, "\n");
  }
  if (exp_file)
    fclose(fvtk);

  if (_free){
    ListeD *pt_suppr;
    for (i = 0; i < nbpts; i++){
      pt_suppr = get_element_gtab(pts,i);
      free_ptmaill(pt_suppr->el->el);
      free_mailleD(pt_suppr, 1);
    }
    for (i = 0; i < nbpar; i++){
      pt_suppr = get_element_gtab(quadr,i);
      free_ptmaill(pt_suppr->el->el);
      free_mailleD(pt_suppr, 1);
    }
    free_slisteD(m->lignes);
    free_slisteD(m->quadr);
  }
}

/*
 * Met tous les couples de segments que l'on peut encore utiliser
 * à inutilisable
 */
void maillage_stop_bord(Maillage *m){
  Liste *tmp;
  ListeD *el;
  Ptmaill *pt;
  int i, j;
  int ind_seg;
  
  for(j = 0;j<m->lignes->nb_part;j++){
    tmp = m->lignes->bord[j]->l;
    while(tmp != NULL){
      el = tmp->el;
      pt = get_elementL2D(el);
      for(i = 0;i<4;i++){
	ind_seg = pt->ind_seg[i];
	if(ind_seg != -1 && m->seg[ind_seg]->q1 == NULL){
	  m->seg[ind_seg]->b_new = 0;
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
 * Utilisé comme callback : supprime un point
 * => supprime tous les segments qui le relient ainsi que 
 * tous les ||grammes qui le contient
 * pt : type : ListeD*, le point à supprimer
 * param : type : Maillage*, le maillage
 */
void suppr_point(void *pt, void *param_call, 
		 Param_maillage *param){
  ListeD *p = pt, *vois;
  Ptmaill *ptm = get_elementL2D(p);
  Maillage *m = param_call;
  int i, ind_seg;

  for (i = 0; i<4; i++){
    ind_seg = ptm->ind_seg[i];
    if (ind_seg != -1){
      vois = liste_getVois(p, i);
      suppr_seg(m, vois, (i + 2) % 4);
    }
  }
  free_ptmaill(ptm);
  suppr_el_listeD(m->lignes, p, 0);
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
 * Utilisé dans le cas où on doit fusionner 
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
 * Attention : ne fonctionne que si l'on est pas une rosace
 * l : le point de départ du segment
 * dir : la direction du segment
 */
void suppr_seg(Maillage *m, ListeD* l, int dir){
  ListeD *q;
  ListeD *l_vois_pt;
  int i, j;
  Ptmaill *pt = get_elementL2D(l);
  int ind_seg = pt->ind_seg[dir];
  Segment *seg = m->seg[ind_seg];
  int bool;
  Ptmaill *vois_pt;
  Quadr *qu;

  pt->ind_seg[dir] = -1;
  m->seg[ind_seg] = NULL;
  for(i = 0;i<2;i++){
    if(i == 0)
      q = seg->q1;
    else
      q = seg->q2;
    if(q != NULL){
      for(j = 1;j<=3;j+=2){
	bool = maillage_suppr_quadr_seg(m, pt, (dir+j)%4, q);
	if(bool){
	  l_vois_pt = liste_getVois(l, (dir+j)%4);
	  vois_pt = get_elementL2D(l_vois_pt);
	  maillage_suppr_quadr_seg(m, vois_pt, dir, q);
	  l_vois_pt = liste_getVois(l_vois_pt, dir);
	  vois_pt = get_elementL2D(l_vois_pt);
	  maillage_suppr_quadr_seg(m, vois_pt, (dir+j+2)%4, q);
	}
      }
      qu = get_elementL2D(q);
      freeQuadr(qu);
      suppr_el_listeD(m->quadr, q, 0);
    }
  }
  free_segment(seg);
}

/*
 * Supprime le segment si le ||gramme q le contient
 * pt : point de départ du segment
 * or : la direction du segment
 * q : le ||gramme
 * retourne 1 si on a trouvé le ||gramme
 */
int maillage_suppr_quadr_seg(Maillage *m, Ptmaill *pt, 
			     int or, ListeD *q){
  int ind_seg_b = pt->ind_seg[or];
  Segment *seg_b;
  if(ind_seg_b == -1)
    return 0;
  seg_b = m->seg[ind_seg_b];
  return suppr_quadr_seg(seg_b, q);
}

Pt* getvect(ListeD *l){
  Ptmaill *pt = l->el->el;
  return pt->p->pt;
}

int callback_stop_distance_pt(Maillage *m, void *param){
  Param_call1 *p = (Param_call1*) param;
  double dist_p = p->dist_p;
  Pt *tmp;
  p->prec = liste_getVois(p->prec, p->or);
  tmp = getvect(p->prec);
  p->dist_p = distance(tmp->p, p->pt->p);
  if(p->dist_p > dist_p)
    return 1;
  return 0;
}

int extraire_diag(ListeD *in, int or, int or_vir, Pt *c_res){
  int nb = 0;
  int del = 3+2*or_vir;
  ListeD *tmp = in;
  while(tmp != NULL){
    ptcopy(&c_res[nb], getvect(tmp));
    nb++;
    tmp = liste_getVois(tmp, or);
    if(tmp == NULL)
      return nb;
    tmp = liste_getVois(tmp, (or+del)%4);
  }
  return nb;
}

void export_pas_diag(ListeD *init, int or, 
		     int or_vir, char *path, int bool_milieu){
  FILE *f = fopen(path, "w+");
  ListeD *tmp = init;
  Pt *prec = NULL, *cur;
  int or2 = (or + 3 + 2*or_vir)%4;
  int bool_in = 1;
  int it, nb = 1;

  if(bool_milieu){
    while(tmp != NULL){
      tmp = liste_getVois(tmp, or);
      if(tmp == NULL)
	break;
      tmp = liste_getVois(tmp, (or2+2)%4);
      nb++;
    }
    nb/=2;
    tmp = init;
    it = 0;
    while(it < nb){
      tmp = liste_getVois(tmp, or);
      if(tmp == NULL)
	break;
      tmp = liste_getVois(tmp, (or2+2)%4);
      it++;
    }
  }
  cur = NULL;
  while(tmp != NULL){
    prec = cur;
    cur = getvect(tmp);
    if(!bool_in)
      fprintf(f, "%lf\n", distance(prec->p, cur->p));
    
    bool_in = 0;
    tmp = liste_getVois(tmp, or);
    if(tmp == NULL)
      break;
    tmp = liste_getVois(tmp, or2);
  }
}

void free_maillage(Maillage *m){
  int i;
  for(i = 0;i<m->nb_seg;i++){
    free_segment(m->seg[i]);
  }
  free_listeD(m->lignes);
  free_listeD(m->quadr);
  free(m);
}


/*
 * Utilisé comme callback : le point est au bord ou non
 * (il a tous ses ||grammes créés ou non)
 */
int est_bord_pt(ListeD *pt, void *param){
  Maillage *m = (Maillage*) param;
  Segment *seg;
  Ptmaill *cour = get_elementL2D(pt);
  int i, ind;
  if(pt->vois != NULL)
    return 1;
  for(i = 0;i<4;i++){
    ind = ptmaill_get_ind_seg(cour, i);
    if(ind == -1)
      return 1;
    seg = m->seg[ind];
    if(seg->b_new && seg->q2 == NULL)
      return 1;
  }
  return 0;
}

void maillage_set_sing(Maillage *m, ListeD *pt){
  Ptmaill *ptm = pt->el->el;
  listeD_set_sing(m->lignes, pt);
  ptmaill_ajouter_sing(m, ptm);
}

void maillage_fermer_sing(Maillage* m, ListeD *sing){
  //ajouter une ligne de discontinuité des voisins
  Ptmaill *ptm = sing->el->el;
  int del = sing->vois->nb_vois%4;
  int dir = (sing->vois->vois[0]->el->dir_sing+2)%4;
  listeD_fermer_sing(sing, dir, del);
  listeD_fermer_sing(m->quadr->l[0], dir, del);
  ptmaill_fermer_sing(ptm);
}

/*
 * Appelé lorsqu'on ajoute un voisin à la listeD de points.
 * On créé un segment que l'on ajoute au maillage
 */
void callback_aj_vois(ListeD *l1, ListeD *l2, 
		      int dir1, int dir2, void *param){
  Ptmaill *ptm1 = l1->el->el;
  Ptmaill *ptm2 = l2->el->el;
  Segment *seg = createSegment(ptm1->p, ptm2->p);
  Maillage *m = (Maillage*) param;
  int nb_seg = m->nb_seg;
  m->seg[nb_seg] = seg;
  m->nb_seg++;
  ptmaill_set_ind_seg(ptm1, dir1, nb_seg);
  ptmaill_set_ind_seg(ptm2, dir2, nb_seg);
}

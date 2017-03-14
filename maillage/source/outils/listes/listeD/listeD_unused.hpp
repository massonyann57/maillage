#include "listeD.h"

/*
 * Constructeur de SListeD (liste doublement chainée en 2D)
 * el : le 1er élément ajouté
 * ind_aj_ros : la direction de la rosace si l'élément est
 *   une rosace (-1 sinon)
 * cmp : callback permettant de comparer 2 éléments
 *   (utilisé pour trier les éléments du bord)
 * exp : callback pour l'exportation des éléments du bord
 * est_bord : callback permettant de décider si un élément est
 *    au bord ou non
 * param_b : paramètre de est_bord
 */
SListeD* createSListeD(void *el, int(*cmp)(void*,
					   void*, void*),
		       void(*exp)(void*),
		       int(*est_bord)(ListeD*, void*),
		       void *param_b,
		       void(*aj_vois)(ListeD*, ListeD*, 
				      int, int, void*),
		       void *param_aj_vois, int est_sing,
		       int est_etoile){
  Elliste *e;
  SListeD* new = malloc(sizeof(SListeD));
  int i;
  memset(new->l, 0, NB_PART_MAX*sizeof(ListeD*));
  memset(new->bord, 0, NB_PART_MAX*sizeof(SListe*));
  for (i = 0; i < NB_PART_MAX;i++)
	memset(new->sing[i], 0, NB_SING_MAX*sizeof(ListeD*));
  memset(new->mailles, 0, NB_PART_MAX*sizeof(PGTab));

  new->nb_sing[0] = 0;
  new->mailles[0] = create_gtab(print_el_listeD, exp);
  new->aj_vois = aj_vois;
  new->param_aj_vois = param_aj_vois;
  if (el == NULL)
    new->l[0] = NULL;
  else{
    e = createElliste(el, 0, 0, est_etoile);
    new->l[0] = createListeD(e, est_sing);
    aj_gtab(new->mailles[0], new->l[0]);
    if (est_sing){
      new->nb_sing[0] = 1;
      new->sing[0][0] = new->l[0];
    }
  }
  if (exp == NULL)
    exp = exp_pos_tab;
  if (cmp == NULL)
    cmp = cmp_pos_tab;

  new->bord[0] = createSListe(new->l[0], cmp, NULL,
			      print_el_listeD, exp);
  new->est_bord = est_bord;
  new->param_bord = param_b;
  new->nb_part = 1;
  return new;
}

/*
 * Constructeur de Elliste (les éléments de la liste2D)
 * el : l'élément
 * est_etoile : utilisé pour les quadrangles (une étoile)
 */
Elliste* createElliste(void *el, int ind_part,
		       int pos_tab, int est_etoile){
  Elliste *res = malloc(sizeof(Elliste));
  res->el = el;
  res->est_bord = 1;
  res->ind_part = ind_part;
  res->pos_tab = pos_tab;
  res->pos_sing = -1;
  res->dir_sing = -1;
  res->est_etoile = est_etoile;
  return res;
}

/*
 * Constructeur de ListeD (maille de la liste2D)
 * est_sing : utilisé pour les points (une singularité)
 */
ListeD* createListeD(Elliste* el, int est_sing){
  ListeD* new = malloc(sizeof(ListeD));
  new->nH = NULL;
  new->pH = NULL;
  new->nL = NULL;
  new->pL = NULL;
  new->el = el;
  new->saut = NULL;
  if (est_sing)
    new->vois = create_maille_sing();
  else
    new->vois = NULL;
  return new;
}

Maille_sing* create_maille_sing(){
  Maille_sing *res = malloc(sizeof(Maille_sing));
  memset(res->vois, 0, NB_BRANCHES_MAX*sizeof(ListeD*));
  res->nb_vois = 0;
  res->est_ferme = 0;
  return res;
}

Saut_vois* create_saut_vois(int dir, int del){
  Saut_vois *res = malloc(sizeof(Saut_vois));
  res->dir1 = dir;
  res->dir2 = -1;
  res->delta = del;
  return res;
}

void listeD_fermer_sing(ListeD *l, int dir, int del){
  ListeD *tmp = l;
  Saut_vois *saut = create_saut_vois((dir + 3) % 4, del);
  if (l->vois != NULL){
    tmp = liste_getVois(l, 0);
    l->vois->est_ferme = 1;
    //    l->saut = create_saut_vois(0, del);
  }
  if (del % 4 != 0){
    while (tmp != NULL){
      tmp->saut = saut;
      tmp = liste_getVois(tmp, dir);
    }
  }
}


/*
 * Ajoute un élément à la liste2D
 * !!!! on tourne toujours dans le sens positif
 *       dans une singularité
 * => si on ferme un parallélogramme touchant une singularité
 *     on donne comme voisin celui à sa gauche dans celle-ci
 * l : la maille précédente
 * el : le nouvel élément
 * nl : si non nul, on ne créé pas de nouvel élément.
 *    C'est nl qui est ajouté
 * ind_aj : - si le voisin n'est pas une singularité :
 *              de 0 à 4 : position de l'ajout
 *           - si le voisin est une singularité :
 *              > 0 : position de l'ajout dans le tableau des voisins
 * ind_aj_new : utilisé seulement si le voisin est une singularité
 *               l'indice de l'ajout new->vois
 */
ListeD* ajouter_listeD(SListeD *sl, ListeD *vois, void *el,
		       ListeD *nl, int ind_aj,
		       int ind_aj_new,
		       int est_sing, int est_etoile){
  ListeD *new;
  Elliste *ell;
  int i;
  int ind_part = sl->nb_part;

  ListeD *vois_tmp, *vois_tmp2;
  int ind_tmp, ind_tmp2;
  int _del, _del2;
  int tmp_bool_aj;
  int pos_tab;
  int del_sing;
 
  if (vois != NULL)
    ind_part = vois->el->ind_part;

  if (vois == NULL){
    if (sl->l[sl->nb_part - 1] == NULL)
      sl->nb_part--;
    ind_part = sl->nb_part;

    ell = createElliste(el, ind_part, 0,
			est_etoile);
    if (nl == NULL)
      new = createListeD(ell, est_sing);
    else
      new = nl;
    if (sl->nb_part != 0 && sl->l[sl->nb_part - 1] != NULL){
      sl->bord[ind_part] = createSListe(new, sl->bord[0]->cmp,
					sl->bord[0]->param_cmp,
					sl->bord[0]->exp,
					sl->bord[0]->param_exp);
      sl->mailles[ind_part]
	= create_gtab(sl->mailles[0]->print_el,
		      sl->mailles[0]->param);
    }

    sl->l[ind_part] = new;
    sl->nb_part++;
    sl->nb_sing[ind_part] = 0;
    memset(sl->sing[ind_part], 0, NB_SING_MAX*sizeof(ListeD*));
    if (est_sing){
      sl->sing[ind_part][0] = new;
      sl->nb_sing[ind_part] = 1;
    }
    aj_gtab(sl->mailles[ind_part], new);

  }
  else {
    ell = createElliste(el, ind_part, -1, est_etoile);
    if (nl == NULL){
      new = createListeD(ell, est_sing);
      if (est_sing){
	int nb_sing = sl->nb_sing[ind_part];
	sl->nb_sing[ind_part]++;
	sl->sing[ind_part][nb_sing] = new;
      }
      pos_tab = aj_gtab(sl->mailles[ind_part], new);
      ell->pos_tab = pos_tab;
    }
    else
      new = nl;
    if(vois->vois == NULL)
      ind_aj_new = (ind_aj+2)%4;
    ajouter_vois(sl, vois, new, ind_aj, ind_aj_new);

    for (i = 0; i < 2; i++){
      _del = 0;
      ind_tmp = 2 * i + 1 + ind_aj;
      tmp_bool_aj = !est_etoile;
      _del = listeD_get_saut(vois, ind_tmp);
      vois_tmp = liste_getVois(vois, ind_tmp);
      del_sing = 0;
      if (vois_tmp != NULL){
	if (vois_tmp->vois != NULL){
	  ind_tmp = (vois->el->pos_sing + 1);
	  if(vois_tmp->vois->est_ferme)
	    ind_tmp %= vois_tmp->vois->nb_vois;
	  /*	  vois_tmp2 = liste_getVois(vois_tmp, ind_tmp + _del);

	  del_sing = get_saut_sing(vois_tmp2, vois);

	  if(vois_tmp2 == NULL 
	     || (vois_tmp2->el->dir_sing+del_sing)%4 == ind_aj){
	    ind_tmp = (vois->el->pos_sing - 1);
	    if(ind_tmp < 0 && vois_tmp->vois->est_ferme)
	      ind_tmp = vois_tmp->vois->nb_vois-1;
	    if(ind_tmp >= 0){
	      vois_tmp2 = liste_getVois(vois_tmp, ind_tmp + _del);
	      
	      del_sing = get_saut_sing(vois_tmp2, vois);
	      if(vois_tmp2 == NULL 
		 || (vois_tmp2->el->dir_sing+del_sing)%4 == ind_aj)
		ind_tmp = -1;
		}
		}*/
	}
	else
	  ind_tmp = ind_aj;
	if(ind_tmp != -1){
	  _del2 = listeD_get_saut(vois_tmp, ind_tmp + _del);
	  vois_tmp2 = liste_getVois(vois_tmp, ind_tmp + _del);
	  _del += _del2;
	  if (vois_tmp2 != NULL){
	    if (!tmp_bool_aj){
	      if (!vois_tmp->el->est_etoile
		  || !vois_tmp2->el->est_etoile)
		tmp_bool_aj = 1;
	    }
	    if (tmp_bool_aj){
	      if (new->vois != NULL){
		ind_tmp = vois->el->pos_sing + 1;
		ajouter_vois(sl, new, vois_tmp2, ind_tmp,
			     vois->el->dir_sing + 1);
	      }
	      else{
		if (vois_tmp2->vois == NULL){
		  ind_tmp = (ind_aj + 3 +2*i/*+4-del_sing*/) % 4;
		  ind_tmp2 = (ind_tmp + 2) % 4;
		}
		else{
		  ind_tmp = vois_tmp->el->pos_sing + 1;
		  ind_tmp2 = (vois_tmp->el->dir_sing + 1) % 4;
		}
		if (_del == 0 && vois_tmp2->saut != NULL){
		  if(liste_getVois(vois_tmp2, 
				   vois_tmp2->saut->dir1) == NULL)
		    _del = 4-vois_tmp2->saut->delta;
		}
		ajouter_vois(sl, vois_tmp2, new,
			     ind_tmp + _del, ind_tmp2);
	      }
	    }
	  }
	}
      }
    }
    actualiser_bord(sl, vois, ind_aj, ind_part);
    ajouter_bord(sl, new, sl->bord[ind_part]->fin);
  }
  return new;
}


/*
 * Ajoute un élément du tableau au bord
 * el : la maille que l'on ajoute au bord
 * el_rech : l'estimation de la position dans le tableau trié
 */
void ajouter_bord(SListeD *sl, ListeD *el, Liste *el_rech){
  el->el->est_bord = 1;
#ifndef _FINL
  ajouter_liste_trie(sl->bord[el->el->ind_part], el,
		     el_rech);
#else
  ajouter_fin_liste(sl->bord[el->el->ind_part], el);
#endif
}

/*
 * Supprime un élément du bord
 */
void retirer_bord(SListeD *sl, ListeD *el){
  el->el->est_bord = 0;
  rech_suppr_el_liste(sl->bord[el->el->ind_part], el);
}

/*
 * Actualise le bord après ajout dans la liste.
 * cur : le voisin de la maille ajouté (voisin ayant servi
 *    à ajouter celle-ci)
 * ind_aj : la direction de l'ajout
 */
void actualiser_bord(SListeD *sl, ListeD *cur, int ind_aj,
		     int ind_part){
  int i;
  ListeD *vois, *vois2;
  for (i = 0; i < 2; i++){
    vois = liste_getVois(cur, 2 * i + 1 + ind_aj);
    if (vois != NULL){
      if (!estBordL2D(sl, vois))
	retirer_bord(sl, vois);

      /* TODO : peut etre retiré dans le cas ou
       *    on définie soit-même le bord
       */
      vois2 = liste_getVois(vois, ind_aj);
      if (vois2 != NULL){
	if (!estBordL2D(sl, vois2)){
	  retirer_bord(sl, vois2);
	}
      }
    }
  }
  if (!estBordL2D(sl, cur))
    retirer_bord(sl, (void*)cur);
}

/*
 * Ajoute un voisin à une maille
 * vois : l'élément de base
 * new : le nouveau voisin de vois (et inversement)
 * ind_aj : la direction de l'ajout
 * ind_aj2 : cas où le voisin est une singularité
 *            => la direction new -> vois
 * !!! new ne peut être une singularité que s'il vient d'être créé
 * retourne la position de new par rapport au voisin
 */
void ajouter_vois(SListeD *sl, ListeD *vois, ListeD *new,
		  int ind_aj, int ind_aj2){
  int _ind_aj, _ind_aj2;
  ListeD *vois_tmp1, *vois_tmp2;

  if (vois->vois == NULL){
    ListeD *tmp1 = NULL, *tmp2 = NULL;
    int i, _ind;
    int aj_newtovois = (ind_aj + 2) % 4;
    ind_aj = ind_aj % 4;
    if (vois->saut != NULL){
      if (ind_aj == vois->saut->dir1 || ind_aj == vois->saut->dir2){
	int new_dir = (ind_aj+2+vois->saut->delta)%4;
	aj_newtovois += vois->saut->delta;
	//	if (new->saut == NULL){
	  new->saut = create_saut_vois(new_dir,
				       4-vois->saut->delta);
	  /*} else 
	    new->saut->dir2 = new_dir;*/
      }
      else if (ind_aj == (vois->saut->dir1 + 3) % 4
	       || ind_aj == (vois->saut->dir1 + 1) % 4){
	// on est dans le primal et on reste sur la frontière
	/*		vois_tmp1 = liste_getVois(vois, vois->saut->dir1+1);
	vois_tmp2 = liste_getVois(vois, vois->saut->dir1+3);
	if((vois_tmp1 != NULL && vois_tmp1->saut != NULL)
	|| (vois_tmp2 != NULL && vois_tmp2->saut != NULL)){*/
	     int new_dir = vois->saut->dir1;
	     new->saut = create_saut_vois(new_dir,
					  vois->saut->delta);
	     //}
      }
    }
    aj_newtovois = aj_newtovois % 4;
    for (i = 0; i < 2; i++){
      if (i == 0){
	_ind = ind_aj;
	tmp1 = vois;
	tmp2 = new;
      }
      else{
	if (new->vois != NULL)
	  break;
	_ind = aj_newtovois;
	_ind_aj2 = _ind;
	tmp1 = new;
	tmp2 = vois;
      }
      if (_ind == 0){
	if (tmp1->nH != NULL)
	  printf("erreur ajout voisin\n");
	tmp1->nH = tmp2;
      }
      else if (_ind == 1){
	if (tmp1->pL != NULL)
	  printf("erreur ajout voisin\n");
	tmp1->pL = tmp2;
      }
      else if (_ind == 2){
	if (tmp1->pH != NULL)
	  printf("erreur ajout voisin\n");
	tmp1->pH = tmp2;
      }
      else{
	if (tmp1->nL != NULL)
	  printf("erreur ajout voisin\n");
	tmp1->nL = tmp2;
      }
    }
    _ind_aj = ind_aj;
  }
  else{
    if (vois->vois->est_ferme)
      ind_aj = ind_aj%vois->vois->nb_vois;

    vois->vois->nb_vois++;
    vois->vois->vois[ind_aj] = new;
    new->el->pos_sing = ind_aj;
    new->el->dir_sing = ind_aj2;
    if (ind_aj2 == 0)
      new->nH = vois;
    else if (ind_aj2 == 1)
      new->pL = vois;
    else if (ind_aj2 == 2)
      new->pH = vois;
    else
      new->nL = vois;

    _ind_aj = ind_aj;
    _ind_aj2 = ind_aj2;
  }
  if (new->vois != NULL){
    if (new->vois->est_ferme)
      ind_aj2 = ind_aj2%new->vois->nb_vois;
    new->vois->vois[ind_aj2] = vois;
    vois->el->pos_sing = ind_aj2;
    vois->el->dir_sing = ind_aj;
    new->vois->nb_vois++;
    _ind_aj2 = ind_aj2;
  }
  if (sl->aj_vois != NULL){
    sl->aj_vois(vois, new, _ind_aj, _ind_aj2, sl->param_aj_vois);
  }
}

/*
 * Met le voisin à null dans une direction donnée
 */
void set_vois(ListeD *l, int ind_aj, ListeD *l2){
  if (l->vois != NULL)
    printf("erreur!!!!!\n");
  ind_aj = ind_aj % 4;
  if (ind_aj == 0)
    l->nH = l2;
  else if (ind_aj == 1)
    l->pL = l2;
  else if (ind_aj == 2)
    l->pH = l2;
  else
    l->nL = l2;
}

/*
 * Supprime un élément de la liste2D
 * bool : décale tous les autres éléments ou non
 */
void suppr_el_listeD(SListeD *sl, ListeD *l, int bool_dec){
  int bool = 1;
  int ind_part = l->el->ind_part;
  if (l->nH != NULL){
    if (sl->l[ind_part] == l){
      sl->l[ind_part] = l->nH;
      bool = 0;
    }
    if (!l->nH->el->est_bord)
      ajouter_bord(sl, (void*)l->nH, sl->bord[ind_part]->fin);
    if (bool_dec)
      l->nH->pH = l->pH;
    else
      l->nH->pH = NULL;
  }
  if (l->nL != NULL){
    if (sl->l[ind_part] == l && bool){
      sl->l[ind_part] = l->nL;
      bool = 0;
    }
    if (!l->nL->el->est_bord)
      ajouter_bord(sl, (void*)l->nL, sl->bord[ind_part]->fin);
    if (bool_dec)
      l->nL->pL = l->pL;
    else
      l->nL->pL = NULL;
  }
  if (l->pL != NULL){
    if (sl->l[ind_part] == l && bool){
      sl->l[ind_part] = l->pL;
      bool = 0;
    }
    if (!l->pL->el->est_bord)
      ajouter_bord(sl, (void*)l->pL, sl->bord[ind_part]->fin);
    if (bool_dec)
      l->pL->nL = l->nL;
    else
      l->pL->nL = NULL;
  }
  if (l->pH != NULL){
    if (sl->l[ind_part] == l && bool){
      sl->l[ind_part] = l->pH;
      bool = 0;
    }
    if (!l->pH->el->est_bord)
      ajouter_bord(sl, (void*)l->pH, sl->bord[ind_part]->fin);
    if (bool_dec)
      l->pH->nH = l->nH;
    else
      l->pH->nH = NULL;
  }
  if (bool && sl->l[ind_part] == l)
    sl->l[ind_part] = NULL;
  suppr_el_gtab(sl->mailles[ind_part], l->el->pos_tab);
  rech_suppr_el_liste(sl->bord[ind_part], l);
  free_mailleD(l, 1);
}

/*
 * Récupère le voisin d'un élément dans une direction donnée
 * l : la maille
 * i : la direction
 */
ListeD* liste_getVois(ListeD *l, int i){
  if (l->vois == NULL){
    i = i % 4;
    if (i == 0)
      return l->nH;
    if (i == 1)
      return l->pL;
    if (i == 2)
      return l->pH;
    else
      return l->nL;
  }
  else {
    if (l->vois->est_ferme)
      i = i%l->vois->nb_vois;
    return l->vois->vois[i];
  }
}

void* get_elementL2D(ListeD *l){
  return l->el->el;
}

void set_elementL2D(ListeD *l, void *el){
  l->el->el = el;
}


/*
 * On supprime les éléments qui ne sont pas utilisés
 * param_is_used : les paramètres du callback is_used
 * callback_is_used : précise si un point est utilisé ou non
 * callback_suppr : supprime l'élément qui n'est pas utilisé
 * param_suppr : les paramètres du callback suppr
 */
void suppr_unused_el_bord(SListeD *sl, Param_maillage *param,
			  void *param_is_used,
			  int(*callback_is_used)(void*, void*,
						 Param_maillage*),
			  void(*callback_suppr)(void*, void*,
						Param_maillage*),
			  void *param_suppr, int ind_part){
  Liste *tmp = sl->bord[ind_part]->l;
  ListeD *el;
  while (tmp != NULL){
    el = tmp->el;
    if (!callback_is_used(el->el, param_is_used, param)){
      tmp = tmp->n;
      callback_suppr(el, param_suppr, param);
    }
    else
      tmp = tmp->n;
  }
}

/*
 * Supprime la dernière ligne du maillage
 * (utilisé lorsqu'on atteint une singularité et
 * que l'on veut prolonger en rosace)
 * !!!!! peut-être ajouter une fonction supprimer point
 *    (prenant un callback) et ajouter la suppression du
 *    bord si nécessaire
 * callback : réalise la suppression de l'élément
 * param : un paramètre du callback
 * or_vir : la direction du virage (or+1 ou or+3 ?)
 */
void suppr_dern_ligne(SListeD *l, Param_maillage *param,
		      void(*callback)(void *el, void *m,
				      Param_maillage*),
		      void *param_call, int ind_part,
		      int or, int or_vir){
  ListeD *tmp, *cp, *tmp2;
  int or2 = (or + 1 + 2 * or_vir) % 4;

  if (l->l[ind_part] != NULL){
    //tmpb = l->bord[ind_part]->l;
    /*  while(tmpb != NULL){
	tmp = tmpb->el;
	if(tmp->nH == NULL || tmp->nL == NULL){
	cpb = tmpb;
	tmpb = tmpb->n;
	retirer_bord(l->bord[ind_part], cpb);
	//	suppr_el_liste(l->bord[ind_part], cpb);
	}
	else
	tmpb = tmpb->n;
	}*/

    tmp = l->l[ind_part];
    while (liste_getVois(tmp, or2) != NULL)
      tmp = liste_getVois(tmp, or2);

    while (tmp != NULL){
      cp = tmp;
      tmp = liste_getVois(tmp, or);
      tmp2 = liste_getVois(cp, (or2 + 2) % 4);
      if (tmp2 != NULL)
	set_vois(tmp2, or2, NULL);
      if (callback != NULL)
	callback(cp, param_call, param);
    }

    tmp = l->l[ind_part];
    while (liste_getVois(tmp, or) != NULL)
      tmp = liste_getVois(tmp, or);

    while (tmp != NULL){
      cp = tmp;
      tmp = liste_getVois(tmp, or2);
      tmp2 = liste_getVois(cp, or + 2);
      if (tmp2 != NULL)
	set_vois(tmp2, or, NULL);
      if (callback != NULL)
	callback(cp, param_call, param);
    }
  }
}

/*
 * On fusionne 2 parties de la liste. Une des 2
 * parties ne contient qu'un seul point
 * sl : la liste
 * l : la maille à qui on va raccrocher la 2ème composante
 *    connexe
 * or : l'orientation de la maille isolé par rapport à l
 * i2 : l'indice de la composante isolée
 */
void merger_composante_isole(SListeD *sl, int ind_princ,
			     int ind_iso){
  int nb_el_tab = sl->mailles[ind_princ]->nb_tot;
/*  int i;
  for (i = 0; i < sl->mailles[ind_iso];)*/
  free_liste(sl->bord[ind_iso]);
  free_gtab(sl->mailles[ind_iso],NULL);
  sl->bord[ind_iso] = NULL;
  sl->l[ind_iso]->el->ind_part = ind_princ;
  sl->l[ind_iso]->el->pos_tab = nb_el_tab;
  aj_gtab(sl->mailles[ind_princ], sl->l[ind_iso]);
  sl->l[ind_iso] = NULL;
  sl->bord[ind_iso] = NULL;
  sl->mailles[ind_iso] = NULL;
  sl->nb_part--;
}

void suppr_autres_composantes
(SListeD *sl, Param_maillage *param,
 int ind, void(*callback_suppr)(void*, void*), void *param_call){
  // TODO : ajouter callback pour la suppression
  int i;
  for (i = 1; i < sl->nb_part; i++){
    free_liste(sl->bord[i]);
    if (sl->l[i] != NULL){
      callback_suppr(sl->l[i], param_call);
      free_mailleD(sl->l[i], 1);
      free_gtab(sl->mailles[i],NULL);
    }
    sl->l[i] = NULL;
    sl->bord[i] = NULL;
    sl->mailles[i] = NULL;
  }
  sl->nb_part = 1;
}

void free_elliste(Elliste *el){
  free(el);
}

void free_mailleD(ListeD *l, int bool_suppr_elliste){
  if (bool_suppr_elliste)
    free_elliste(l->el);
  free(l);
}

void free_listeD(SListeD *sl){
  free(sl);
}

void free_slisteD(SListeD *sl){
  int i;
  for (i = 0; i < sl->nb_part; i++){
    free_gtab(sl->mailles[i],NULL);
    free_liste(sl->bord[i]);
    sl->l[i] = NULL;
  }
  free(sl);
}

void print_el_listeD(void *maille, void *fonc){
  void(*print_el)(void*) = fonc;
  ListeD *el = maille;
  int size_max = 8;
  int size = size_max;
  int pos = el->el->pos_tab;
  int puiss = (int)pow(10, size);
  int i;
  if (pos > 0){
    while (pos / puiss == 0){
      puiss = (int)pow(10, size - 1);
      size--;
    }
    size++;
  }
  else
    size = 1;
  printf("%d", pos);
  for (i = 0; i < size_max - size; i++)
    printf(" ");
  printf("||  ");
  print_el(el->el->el);
}

void DBG_print_estBord(SListeD *sl, int ind_part){
  SListe *b = sl->bord[ind_part];
  Liste *tmp = b->l;
  ListeD *el;
  while (tmp != NULL){
    el = tmp->el;
    printf("%d  : %d\n", el->el->pos_tab, estBordL2D(sl, el));
    tmp = tmp->n;
  }
}

void DBG_print_liste_bord(SListeD *sl, int ind_part){
  DBG_print_liste(sl->bord[ind_part]);
}

void DBG_print_liste_mailles(SListeD *sl, int ind_part){
  exp_gtab(sl->mailles[ind_part]);
}

void listeD_changer_partition(SListeD *sl, ListeD *el,
			      int new_part){
  int nb = sl->mailles[new_part]->nb_tot;
  el->el->ind_part = new_part;
  el->el->pos_tab = nb;
  aj_gtab(sl->mailles[new_part], el);
}

void listeD_set_sing(SListeD *sl, ListeD *l){
  int i, nb_vois = 0;
  int ind_part = l->el->ind_part;
  int nb_sing = sl->nb_sing[ind_part];
  ListeD *vois;
  int i_init;
  Maille_sing *tmp = create_maille_sing();
  //  retirer_bord(sl, l);
  i_init = 4;
  while (i_init > 0 && liste_getVois(l, i_init) != NULL)
    i_init--;
  if (i_init == 4)
    i_init = 0;
  for (i = i_init; i < i_init + 4; i++){
    vois = liste_getVois(l, i);
    if (vois != NULL){
      tmp->vois[nb_vois] = vois;
      vois->el->dir_sing = (i + 2) % 4;
      vois->el->pos_sing = nb_vois;
      nb_vois++;
    }
  }
  l->vois = tmp;
  l->vois->nb_vois = nb_vois;
  sl->sing[ind_part][nb_sing] = l;
  sl->nb_sing[ind_part]++;
}

int listeD_get_saut(ListeD *l, int dir){
  if (l->saut != NULL){
    if (l->vois != NULL)
      dir = dir%l->vois->nb_vois;
    else
      dir = dir % 4;
    if (l->saut->dir1 == dir || l->saut->dir2 == dir){
      return l->saut->delta;
    }
  }
  return 0;
}

int listeD_get_nb_max_vois(ListeD *l){
  if (l->vois == NULL)
    return 4;
  else return l->vois->nb_vois;
}

ListeD* get_element_SListeD(SListeD *sl, int ind_part, int ind_el){
  GTab *tab = sl->mailles[ind_part];
  if(tab != NULL){
    return get_element_gtab(tab, ind_el);
  } else
    return NULL;
}

int get_saut_sing(ListeD *v_sing1, ListeD *v_sing2){
  if(v_sing2 != NULL && v_sing1 != NULL){ 
    if(v_sing1->saut != NULL 
       && (v_sing1->saut->dir1 + v_sing1->saut->delta)%4 
       == (v_sing2->el->dir_sing+2)%4)
      return v_sing1->saut->delta;
    else if(v_sing2->saut != NULL 
	    && (v_sing2->saut->dir1+v_sing2->saut->delta)%4 
	    == (v_sing1->el->dir_sing+2)%4)
      return 4-v_sing2->saut->delta;
  }
  return 0;
}

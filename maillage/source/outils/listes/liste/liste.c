#include "liste.h"

/*
 * Constructeur d'une liste
 * le 1er élément de la liste
 * cmp : callback permettant de comparer les éléments de la 
 * liste si on souhaite la trier
 * param : paramètre de cmp
 * exp : callback permettant d'exporter la suite
 */
SListe* createSListe(void *el,
		     int (*cmp)(void*,void*,void*),void* param_cmp,
		     void (*exp)(void*,void*), void *param_exp){
  SListe* new = malloc(sizeof(SListe));
  if(el != NULL)
    new->l = createListe(el);
  else
    new->l = NULL;
  new->fin = new->l;
  new->cmp = cmp;
  new->exp = exp;
  new->param_cmp = param_cmp;
  new->param_exp = param_exp;
  return new;
}

/* 
 * Constructeur d'une maille de la liste
 */
Liste* createListe(void *el){
  Liste* new = malloc(sizeof(Liste));
  new->n = NULL;
  new->p = NULL;
  new->el = el;
  return new;
}

/*
 * Ajoute un élément à la fin de la liste
 */
void ajouter_fin_liste(SListe *l, void *el){
  Liste *new = createListe(el);
  if(l->l != NULL){
    l->fin->n = new;
    l->fin->n->p = l->fin;
    l->fin = l->fin->n;
  } else {
    l->l = new;
    l->fin = new;
  }
}

/* 
 * ajoute un élément au début de la liste
 */
void ajouter_debut_liste(SListe *l, void *el){
  Liste *new = createListe(el);
  new->n = l->l;
  if(l->l != NULL){
    l->l->p = new;
    l->l = new;
  } else {
    l->l = new;
    l->fin = new;
  }
}

/* 
 * ajoute un élément au début de la liste
 */
void ajouter_apres_liste(SListe *l, Liste *m, void *el){
  Liste *new = createListe(el);
  new->p = m;
  new->n = m->n;
  new->n->p = new;
  m->n = new;
  if(l->fin == m)
    l->fin = new;
}

/* 
 * Ajoute un élément à la liste trié
 * prec : une estimation de la position ou on va être ajouté
 * retourne l'élément ajouté
 */
Liste* ajouter_liste_trie(SListe *l, void *el, Liste *prec){
  if(l->l == NULL){
    l->l = createListe(el);
    l->fin = l->l;
    return l->l;
  } else {	
    int signe;
    Liste *tmp;
    Liste *n = createListe(el);
    if(prec == NULL)
      prec = l->fin;
    signe = l->cmp(el, prec->el, l->param_cmp);
    tmp = prec;
	  
    while(tmp != NULL
	  && l->cmp(el, tmp->el, l->param_cmp) == signe){
      if(signe == -1)
	tmp = tmp->p;
      else
	tmp = tmp->n;
    }
    if(signe == -1){
      if(tmp != NULL){
	n->n = tmp->n;
	if(tmp->n != NULL)
	  tmp->n->p = n;
	else 
	  l->fin = n;
	n->p = tmp;
	n->p->n = n;
      } else {
	n->n = l->l;
	l->l = n;
	n->p = NULL;
	n->n->p = n;
      }
    } else {
      if(tmp != NULL){
	n->p = tmp->p;
	if(tmp->p != NULL)
	  tmp->p->n = n;
	else 
	  l->l = n;
	n->n = tmp;
	n->n->p = n;
      } else {
	n->p = l->fin;
	l->fin = n;
	n->n = NULL;
	n->p->n = n;
      }
    }
    return n;
  }
}

void free_elements_liste(SListe *l){
  Liste *tmp = l->l;
  Liste *tmp2;
  while(tmp != NULL){
    tmp2 = tmp;
    tmp = tmp->n;
    free(tmp2);
  }
  l->l = NULL;
  l->fin = NULL;
}

void free_liste(SListe *l){
  free_elements_liste(l);
  free(l);
}

/*
 * On recherche un élément de la liste triée puis on le supprime
 */
void rech_suppr_el_liste(SListe *l, void *el){
  Liste *tmp;
  if(l == NULL || l->l == NULL)
    return;
  if(l->l->el == el){
    tmp = l->l;
    l->l = l->l->n;
    if(l->l != NULL)
      l->l->p = NULL;
    free(tmp);
  } else {
    tmp = l->l;
    while(tmp->n != NULL
	  && el != tmp->n->el)
      tmp = tmp->n;
    if(tmp->n != NULL){
      Liste *tmp2 = tmp->n->n;
      free(tmp->n);
      tmp->n = tmp2;
      if(tmp2 == NULL)
	l->fin = tmp;
      else
	tmp->n->p = tmp;
    }
  }
}

/* 
 * Supprime un élément de la liste
 */
void suppr_el_liste(SListe *sl, Liste *l){
  if(l->n == NULL)
    sl->fin = l->p;
  else
    l->n->p = l->p;
  if(l->p == NULL)
    sl->l = l->n;
  else
    l->p->n = l->n;
  free(l);
}

/* 
 * On retourne un tableau contenant les éléments de la liste
 */
int liste2tab(SListe *l, Pvoid *res){
  int i = 0;
  Liste *tmp = l->l;
  while(tmp != NULL){
    res[i] = tmp->el;
    tmp = tmp->n;
    i++;
  }
  return i;
}

void* get_elementL(Liste *l){
  return l->el;
}

/*
 * On copie la liste (en la triant)
 * call_aj : un callback disant si un élément 
 *   doit être ajouté ou non
 * param : le paramètre de ce callback
 */
SListe* copier_trier_liste(SListe *l,
			   int (*call_aj)(void*,void*),
			   void* param){
  SListe *res = createSListe(NULL, l->cmp, l->param_cmp,
			     l->exp, l->param_exp);
  merger_listes(res, l, call_aj, param);
  return res;
}

void merger_listes(SListe *l1, SListe *l2, 
		   int (*call_aj)(void*,void*),
		   void* param){
  Liste *tmp = l2->l;
  Liste *aj_prec = NULL;
  while(tmp != NULL){
    if(call_aj == NULL || call_aj(tmp->el, param))
      aj_prec = ajouter_liste_trie(l1, tmp->el, aj_prec);
    tmp = tmp->n;
  }
}

/* 
 * Retourne la taille de la liste
 */
int size_liste(SListe *l){
  int res = 0;
  Liste *tmp = l->l;
  while(tmp != NULL){
    res++;
    tmp = tmp->n;
  }
  return res;
}

/* 
 * Affiche les éléments de la liste
 */
void DBG_print_liste(SListe *l){
  int s = 0;
  Liste* tmp = l->l;
  while(tmp != NULL){
    l->exp(tmp->el, l->param_exp);
    tmp = tmp->n;
    s++;
  }
  printf("taille : %d\n", s);
}

/* 
 * Affiche les pointeurs de la liste
 */
void DBG_print_point(SListe *l){
  Liste *tmp = l->l;
  while(tmp != NULL){
    printf("%p\n", tmp->el);
    tmp = tmp->n;
  }
}

#ifndef H_LISTE_STRUCT

#define H_LISTE_STRUCT

/*
 * Une maille de la liste double chain�e
 */
typedef struct Liste{
  void *el; /* �l�ment courant */
  struct Liste* n; /* maille suivante */
  struct Liste* p; /* maillle precedente */
}Liste;

/*
 * Une liste doublement chain�e 
 * (qui peut �tre tri� via un callback)
 */
typedef struct SListe{
  /* callback comparant 2 �l�ments
   * 0 : m�me �l�ment
   * 1 : el1 > el2
   * -1 : el1 < el2
   */
  int (*cmp)(void *el1, void *el2, void *param); 
  /* callback exportant l'�l�ment */
  void *param_cmp; /* un parametre de cmp*/
  void (*exp)(void *el, void *param);
  void *param_exp;
  struct Liste* l; /* la 1ere maille */
  struct Liste* fin; /* la derniere maille */
}SListe;

typedef SListe* PSListe;

#endif

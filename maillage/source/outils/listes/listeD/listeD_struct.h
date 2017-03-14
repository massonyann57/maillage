#ifndef H_LISTED_STRUCT

#define H_LISTED_STRUCT

#include "../liste/liste_struct.h"
#include "../../tab/tab_struct.h"
#include "../../../parametre/global_def.h"
#include "../../tab/gtab_struct.h"

typedef struct LISTED ListeD;

typedef ListeD* PListeD;

typedef struct SAUT_VOIS{
  int dir1;
  int dir2;
  int delta;
  int delta2;
  int dessus;
} Saut_vois;

/*
 * L'élément contenu dans une liste doublement chainée 
 * dans 2 dimensions
 */
typedef struct ELLISTE{
  void *el; /* element */
  int est_bord; /* est dans la liste du bord ou non */

  int ind_part; /* la structure peut être partitionné :
		 * l'indice de cette partition
		 */
  int pos_sing; /* dans le cas où un voisin est une singularité : la position dans cette sing */
  int dir_sing; /* la direction dans laquelle est la singularité */
  int pos_tab;
  int est_etoile; /* utilisé pour les quadrangles*/

  int primal_pos_sing; /* utilisé pour générer le maillage dual */
  int pos_tab_exp;
}Elliste;


typedef struct MAILLE_SING{
  PListeD vois[2*NB_BRANCHES_MAX];
  int nb_vois;
  int est_ferme;
  int ind_sing;
}Maille_sing;

/*
 * Une maille de la liste doublement chainée dans 2 dimensions
 */
typedef struct LISTED{
  Elliste *el; /* l'élément */
  ListeD* nH; /* le suivant vers le haut (indice 0) */
  ListeD* pH; /* le précédent vers le haut (indice 2) */
  ListeD* nL; /* le suivant vers la droite (indice 3) */
  ListeD* pL; /* le précedent vers la droite (indice 1) */
  Maille_sing* vois; /* dans le cas où on a une singularité (valence > 4)*/
  Saut_vois *saut;
}ListeD;


/*
 * Une liste doublement chainée dans 2 directions
 */
typedef struct SLISTED{
  PListeD l[NB_PART_MAX]; /* Une maille de la liste 
	      * (considérée comme la 1ere) */
  PSListe bord[NB_PART_MAX]; /* liste de mailles de la liste (le bord) */
  PGTab mailles[NB_PART_MAX];

  int (*est_bord)(ListeD*, void*); 
  int use_bord;
  /* callback disant si un élément est au bord ou non */
  void *param_bord; /* paramètre du callback */

  void (*aj_vois)(ListeD*,ListeD*,int,int,void*);
  void *param_aj_vois;

  int nb_part;

  int nb_sing[NB_PART_MAX];
  PListeD sing[NB_PART_MAX][NB_SING_MAX];
}SListeD;

#endif

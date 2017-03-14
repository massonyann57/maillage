#ifndef POINT_STRUCT_H

#define POINT_STRUCT_H

#include "../../parametre/global_def.h"
#include "../../outils/listes/listeD/listeD_struct.h"

typedef struct JONC_SING{
  int ind_seg[NB_BRANCHES_MAX];
  int nb_seg;
  int est_ferme;
}Jonc_sing;

/*
 * Un point ainsi que toutes ses caractéristiques
 */
typedef struct POINT{
  Pt* pt; /* le point */
  int ind_constr; /* indice de la construction 
		   * (0 s'il s'agit de la construction initiale, 
		   *  1 s'il s'agit du 1er ajout de courbe,...)*/
  double the; /* l'angle du maillage*/
  int est_bord_sector;
}Point;

typedef Point* PPoint;

/*
 * Les segments (ne sert pour le moment qu'à
 * associer les points aux ||grammes)
 */
typedef struct SEGMENT{
  Point* p1; 
  Point* p2; /*les 2 points du segment*/
  PListeD q1; 
  PListeD q2; /*les 2 ||grammes au voisinage du segment */
  int b_new; /* booléen : construction possible ? */
}Segment;

typedef Segment* Psegment;

/*
 * Un point du maillage (point + segments partant de ce point)
 */
typedef struct PTMAILL{
  Point *p;
  int ind_seg[4]; /* les segments (à travers un
		   * élément de la liste de segments)*/
  //  Jonction *jonc;
  Jonc_sing *sing;
}Ptmaill;

typedef Ptmaill* PPtmaill;

#endif

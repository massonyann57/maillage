#ifndef H_QUADR_STRUCT

#define H_QUADR_STRUCT

#include "../../outils/listes/listeD/listeD_struct.h"

/*
 * Un ||gramme : contient 4 points ainsi 
 *   que les caractéristiques du ||gramme
 */
typedef struct QUADR{
  PListeD pts[4]; /* chacuns des points est un élément
		   * de la liste ligne */
  int ind_seg[4];
  int ind_surf;
  int ind;
  int est_diag;
}Quadr;

typedef Quadr* PQuadr;

#endif

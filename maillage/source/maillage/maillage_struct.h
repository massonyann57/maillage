#ifndef H_MAILLAGE_STRUCT

#define H_MAILLAGE_STRUCT

#include "../outils/listes/listeD/listeD_struct.h"
#include "elements/point_struct.h"
#include "../surface/surface_struct.h"
#include "../surface/curvature_acc.h"

/*
 * Un maillage par ||grammes
 */
typedef struct MAILLAGE{
  double h; /* le pas du maillage*/
  SListeD *lignes; /* liste double de Points (les lignes)*/
  SListeD *quadr; /* liste double de Quadr (les ||grammes)*/
  GTab* seg; /* tableau des segments */

  Surf_param *s; /* la surface que l'on maille*/
  int ind_max; /* l'indice maximal autorisé 
		* (utilisé pour la gestion des singularités)*/
  Mesh_curv *mesh_curv;
  
  SListeD *pts_dual;
  GTab *quadr_dual;
  int tot_pts_dual;
  int ind_constr_max;
}Maillage;

typedef Maillage* PMaillage;

/*
 * Un maillage par ||grammes
 */
typedef struct PARAM_CALL1{
  int or;
  double dist_p;
  Pt *pt;
  ListeD *prec;
}Param_call1;

#endif

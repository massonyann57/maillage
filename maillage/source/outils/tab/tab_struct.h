#ifndef H_TAB_STRUCT

#define H_TAB_STRUCT

#include "../../parametre/global_def.h"
#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"

/*
 * Un tableau 2D
 */
typedef struct TAB2D{
  Pvoid el[SIZE_TAB][SIZE_TAB]; /* les éléments du tableau */
  int bool[SIZE_TAB][SIZE_TAB]; /* élément utilisé ou non */
  int nbi;                      /* le nombre de lignes */
  int init_j[SIZE_TAB];         /* le début de chaques colonnes */
  int fin_j[SIZE_TAB];          /* la fin de chaques colonnes */
  double (*exp)(void *el);      /* callback pour l'exportation 
				 * de chaque éléments*/
  int (*is_used)(void*,void*,Param_maillage*);  /* callback disant si un élément 
				 * est utilisé ou non */
  void *param;                  /* paramètre de is_used */
  int ind[SIZE_TAB][SIZE_TAB];  /* les indices de chaques éléments 
				* du tableau*/
}Tab2D;

#endif

#ifndef H_GTAB_STRUCT

#define H_GTAB_STRUCT

#include "../../parametre/global_def.h"
#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"

/*
 * Un tableau 2D
 */
typedef struct GTAB{
  PPvoid el[SIZE_GTAB]; /* les éléments du tableau */
  int nbi;                      /* le nombre de lignes */
  int nbj;
  int nb_tot;

  int pos_suppr[SIZE_SUPPR_GTAB][2];
  int nb_suppr;
  Pint bool_suppr[SIZE_GTAB];

  int pos_gtab;
  void (*print_el)(void*,void*);
  void *param;
}GTab;

typedef GTab* PGTab;

#endif

#ifndef SURFACE_STRUCT_H

#define SURFACE_STRUCT_H

#include "../parametre/global_def.h"
#include "../outils/functions/functions_struct.h"

/* 
 * Une surface paramétrée de X0 à X1 
 * /!\ peut-être pas au point...
 */
typedef struct SURF_PARAM{
  int type; /* le type de la surface (inutilisé pour le moment) */
  P3D init; /* le début : X0*/
  P3D fin; /* la fin : X1 */
  int ind_quotient; /* %2 != 0 => la 1ere variable est un quotient
		    *  /2 != 0 => la 2eme variable est un quotient
		    */
  Champ_vect* f; /* paramétrage de la surface*/
  Champ_r* gauss;
}Surf_param;  

/*
 * Une surface définie par une contrainte
 */
typedef struct SURF_CONTR{
  Champ_r* f; /* la fonction définissant la contrainte*/
}Surf_contr;

/* Une surface regroupant tous les types vues précédemment */
typedef struct SURF{
  int type;
  /* le type de la surface
   * 1 pour parametree
   * 2 pour definition par contrainte
   * 3 pour les 2 definitions
   */
  int dom;
  /* 
   * 1 pour parametree
   * 2 pour definition par contrainte
   */
  
  Surf_contr* s_contr;
  Surf_param* s_param;
  /* les pointeurs vers les différents types (un seul est non nul)*/

  double (*metrique_conforme)(double*, void *param);
}Surf;


#endif

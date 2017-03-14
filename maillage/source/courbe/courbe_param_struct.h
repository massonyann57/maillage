#ifndef COURBE_PARAM_STRUCT

#define COURBE_PARAM_STRUCT

#include "../outils/functions/functions_struct.h"

/*
 * La définition d'une courbe : une fonction donnant
 *  le paramétrage et des bornes pour celui-ci
 */
typedef struct COURBEP{
  Fonc_vect *param; // la fonction
  double p_init; //minimum du paramétrage de la courbe
  double p_fin; //maximum du paramétrage de la courbe
  int est_periodique; /* la courbe est périodique ou non 
			* image(p_init) == image(p_fin)
			*/
  double in; //le point de départ de la courbe

  /* dans le cas où on est l'identité : param == NULL */ 
  P3D param_init;
  int dir;
}CourbeP;

typedef CourbeP* PCourbeP;

#endif

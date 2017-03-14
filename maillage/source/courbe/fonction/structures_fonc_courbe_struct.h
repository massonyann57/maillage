#ifndef STRUCTURES_FONC_COURBE_STRUCT

#define STRUCTURES_FONC_COURBE_STRUCT

#include "../../surface/surface_struct.h"
#include "../courbe_param_struct.h"

/*
 * Structure permettant de définir une courbe sur 
 * une surface à partir d'une courbe dans le plan
 * Si la courbe est un pointeur nulle, la courbe est une droite
 */
typedef struct COURBE_SURF_PARAM{
  Surf_param *s; //la surface

/* dans le cas où la courbe est nulle : 
 * y=l2 dans le cas bool=0
 * x=l2 dans le cas bool=1
 */ 
  int bool;  
  double l2;
  /*  */
  CourbeP *c; /* la courbe dans l'espace R^2 */
}Courbe_surf_param;

typedef struct MULT_COURBES{
  int nb;
  PCourbeP c[NB_MULT_COURBE_MAX];
  double init[NB_MULT_COURBE_MAX];
  double fin[NB_MULT_COURBE_MAX];
}Mult_courbes;

typedef struct NEWMULT_COURBES{
  int pos;
  Mult_courbes *courbes;
}NewMult_courbes;

/* Définie une courbe de Bézier dans le plan */
typedef struct BEZIER{
  int nb; // le nombre de points
  P3D pts[NB_PTS_BEZIER_MAX]; // les points
  double coeff[NB_PTS_BEZIER_MAX]; // les coeffs C_i^(nb-1)
}Bezier;


#endif

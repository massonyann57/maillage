#ifndef FUNCTIONS_STRUCT_H

#define FUNCTIONS_STRUCT_H

#include "../../parametre/global_def.h"

/* Structure contenant une application à valeur vectorielle
 * ainsi que sa dérivée */
typedef struct CHAMP_VECT{
  void (*f)(P3D, void*, P3D); /* la fonction à valeur vectorielle */
  void (*f_p)(P3D, void*, P3D*); /* la jacobienne */
  void (*f_p2)(double*,void*,P3D*); /* dérivée seconde */
  void (*f_p3)(double*,void*,P3D*); /* dérivée troisième (uuv et vvu) */
  void *param; /* paramètre à passer à la fonction à
		*  chaque appel */
}Champ_vect;

/* Une fonction ainsi que sa dérivée 1ère et seconde */
typedef struct CHAMP_R{
  double (*f)(double*,void*); /* la fonction */
  void (*f_p)(double*,void*,double*);  /* la dérivée */
  void *param; /* paramètre à passer à la fonction à
		*  chaque appel */
}Champ_r;

/* Structure contenant une application à valeur vectorielle
 * ainsi que sa dérivée */
typedef struct FONC_VECT{
  void (*f)(double, void*, P3D); /* la fonction à valeur vectorielle */
  void (*f_p)(double, void*, P3D); /* la jacobienne */
  void *param; /* paramètre à passer à la fonction à
		*  chaque appel */
}Fonc_vect;

/* Structure contenant une application à valeur vectorielle
 * ainsi que sa dérivée */
typedef struct FONC_R{
  double (*f)(double, void*); /* la fonction à valeur reelle */
  double (*f_p)(double,void*); /* la dérivée*/
  void *param;
}Fonc_r;
#endif

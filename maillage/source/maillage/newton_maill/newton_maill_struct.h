#ifndef NEWTON_MAILL_STRUCT_H

#define NEWTON_MAILL_STRUCT_H

#include "../../courbe/courbe_param_struct.h"
#include "../../parametre/global_def.h"
#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"
#include "../../surface/surface_struct.h"

/*
 * Les paramètres de l'application définie pour le calcul du 
 * ||gramme suivant
 */
typedef struct PARAM_NEW1{
  P3D c1; /* le centre de la 1ere sphere */
  P3D c2; /* le centre de la 2eme sphere */
  double R1; /* le rayon de la 1ere sphère */
  double R2; /* le rayon de la 2eme sphère */
  Surf_param *s;  /* la surface */
  Param_maillage *param;
}Param_new1;

/*
 * Les paramètres de l'application définie pour le calcul du 
 * point suivant d'une courbe sur une surface
 */
typedef struct PARAM_NEW2{
  P3D pos; /* le point précédent */
  P3D base[2]; /* le repère */
  P3D centre; /* le centre de la sphère (doit être le meme que pos dans le cas 
	      de la recherche d'une courbe avec courbure donnée)*/
  double R; /* rayon de la sphère */
  double the; /* l'angle définissant la direction par rapport
	       * au repère */
  Surf_param *s;   /* la surface */
}Param_new2;

/*
 * Les paramètres de l'application définie pour le calcul du 
 * point suivant d'une ligne de paramétrage 
 * sur une surface paramétrée
 */
typedef struct PARAM_NEW3{
  double R; /* le pas de la discrétisation */
  P3D pos; /* le point précédent (centre de la sphère)*/
  CourbeP *c;
}Param_new3;

/*
 * Les paramètres de l'application définie pour le calcul 
 * de l'intersection entre 2 plans et une surface
 */
typedef struct PARAM_NEW5{
  P3D pos1; /* point contenu dans le 1er plan */
  P3D pos2; /* point contenu dans le 2ème plan */
  P3D nor1; /* la normale au plan 1 */
  P3D nor2; /* la normale au plan 2 */
  Surf_param *s;   /* la surface */
}Param_new5;

/*
 * Les paramètres de l'application définie pour le calcul du 
 * point suivant d'une courbe avec une singularitée
 */
typedef struct PARAM_NEW4{
  Param_new2 *p_inter1;
  Param_new2 *p_inter2;
  Param_new5 *p_inter3;
  Param_maillage *param_m;
  Pt* pos; /* le point précédent */
  P3D dir[2]; /* le repère (la direction de chacunes des 
		* deux géodésiques) */
  double R1; /* rayon 1 à obtenir */
  double R2; /* rayon 2 à obtenir */
  Pt* res; /* le point de l'intersection (paramétré) */
  Pt* inter1; /* le point de la 1ere courbe (paramétré) */
  Pt* inter2; /* le point de la 2eme courbe (paramétré) */
  P3D delta[2]; /* les 2 vecteurs delta entre l'intersection et 
		 * le point de la courbe */
  Surf_param *s; /* la surface */
}Param_new4;

#endif

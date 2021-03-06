#ifndef COURBE_STRUCT

#define COURBE_STRUCT

#include "courbe_param_struct.h"
#include "../parametre/global_def.h"

/*
 * Définition des données permettant de construire une courbe 
 * à partir de sa courbure géodésique
 */
typedef struct COURBEG{
  int size; /* le nombre de point */
  double pas; /* les pas de la discrétisation */
  P3D dir_init; /* la direction initiale de la géodésique */
  Pt *init; /* Le point de départ de cette courbe */
  double courbure[SIZE_TAB]; /* la courbure discrète de la courbe 
			      * (/!\ la courbure va aussi 
			      *      dépendre du pas) */
}CourbeG;

/*
 * Définition des données permettant de construire une courbe 
 * à partir d'un certain nombre de points
 */
typedef struct COURBE_PTS{
  int nb; /* le nombre de point */
  Pt pts[SIZE_TAB]; /* les points */
}CourbePts;

/*
 * Définition d'une courbe : elle peut soit être donnée par 
 * la courbure géodésique et le pas 
 * soit par la donnée d'une courbe paramétrée
 */
typedef struct COURBE{
  int type; /* 0 => courbe paramétrée
	     * 1 => courbe donnée par courbure géodésique
	     * 2 => courbe donnée par des pts
	     */
  CourbeP *c_param;
  CourbeG *c_geod;
  CourbePts *c_pts;
} Courbe;

typedef Courbe* PCourbe;

#endif

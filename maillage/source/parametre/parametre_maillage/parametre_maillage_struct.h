#ifndef PARAMETRE_MAILLAGE_STRUCT_H

#define PARAMETRE_MAILLAGE_STRUCT_H

#include <stdio.h>

#include "../global_def.h"
#include "../erreur/erreur_struct.h"

/*
 * les différents paramètres de la construction du maillage
 */
typedef struct PARAM_MAILLAGE{
  double coeff1_new; 
  double coeff2_new; /* 2 coefficients utilisés par l'algorithme 
		      * de Newton */
  int nbMax_new; /* le nombre d'itération maximal de Newton */
  double tol_new;

  double pourc_ajout; /* décalage utilisé à l'initialisation de Newton*/
  double tol_angle_rad; /* tolérence par rapport au rebroussement 
		     * (inutilisé dans le cas ou on ne traite pas 
		     * les singularités) */
  double tol_angle; /* tolérence par rapport au rebroussement 
		     * (inutilisé dans le cas ou on ne traite pas 
		     * les singularités) */
  double tol_angle2; /* utilisé dans le cas ou on ne traite pas 
		      * les singularités : prend le pas sur tol_angle*/
  double tol_rebr; /* tolérence par rapport au rebroussement */
  
  double tol_grad; /* utilisé dans la méthode de Newton 
		      (on regarde si on la matrice est inversible 
		      sinon on fait une méthode de gradient)*/
  double delta_grad; /* on fait baisser le pas dans le cas d'un
		      *	déplacement trop grand */
  double pas_grad;  /* le pas utilisé dans le cas de la 
		      * direction de descente */
  int bool_sing; /* traite-t-on les singularités ou pas */
  int bool_suppr_doublons; /* façon dont on traite le recouvrement*/

  double tol_pas; /* tolérance sur le pas
		   * (utilisé pour la fusion des segments voisins)
		   */
  double pas_max_new; /* pas maximal autorisé lors du Newton */

  int nb_fus; /* pour le calcul de l'intersection de 2 courbes :
	       * le nombre de catégorie de regroupement */
  int gr_fus[SIZE_TAB]; /* les regroupements */
  
  double tol_metr;
  int iter_param_max; /* on itère pour faire des déplacement 
		       * infinitésimal avant d'appliquer le Newton
		       * dans le cas d'une surface paramétrée
		       * C'est le nombre d'itération maximal
		       */

  int nb_etape_new_courbeP;
  double tol_new_courbeP;
  double tol_del_new_courbeP;

  int courbeFerme_nbMin;
  int courbeFerme_nbMax;
  double courbeFerme_dist_max;

  double tol_fermeture_courbe;

  double dist_bord_max;

  Error_type err;
  FILE *file_log;

  /* discretisation de la surface pour le 
     calcul de la courbure*/
  int discr_nbx; 
  int discr_nby;
  int del_discr_nbx;
  int del_discr_nby;
}Param_maillage;

#endif

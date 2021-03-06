#ifndef CURVATURE_STRUCT_H

#define CURVATURE_STRUCT_H

/* la structure de la discretisation utilisé pour la 
 * la somme de la courbure
 */
typedef struct QUADR_MESH{
  int pts[4]; 
  double curv;
  double aire;
  double borne[2][2];
  
  int passage; /* 0 : pas de passage
		* 1 : courbe1 --> -1 après passage
		* 2 : courbe2 --> -2 après passage
		* 3 : au centre et passé
		*/
  int passage2;

  double aire_inter;
  int dir_prop;

  int dirs; /* %2 == 0 => nH non prise
	     * %4 == 0 => pL non prise
	     * ...
	     */
  Liste *pt_dir;

  int bool_util;

  /* DBG */ double curv_aj;
}Quadr_mesh;

typedef Quadr_mesh* PQuadr_mesh;

typedef struct MESH_CURV{
  GTab *pts; 
  SListeD *mesh;
  double min_surf[2];
  double max_surf[2];

  int nbx;
  int nby;
}Mesh_curv;

/* utilisé pour la propagation lors du calcul 
 * de la somme de la courbure : 
 * on a une liste de cette structure qui est 
 * la case suivante à regarder
 */
typedef struct EL_CONSTR_CURV{
  ListeD *l; /* la case suivante */
  int dir_prec; /* la direction dans laquelle on est allé 
		 * pour avoir cette case */
  int aj_int; /* point obtenu par un changement de courbe */
  int est_connexe; /* les courbes initiales ont possiblement 
		    * plusieurs composantes connexes : vaut 1 sauf 
		    * s'il s'agit du bord de ces 
		    * composantes connexes 
		    */
}El_constr_curv;

#endif

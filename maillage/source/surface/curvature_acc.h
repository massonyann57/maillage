#ifndef CURVATURE_ACC_H

#define CURVATURE_ACC_H

#include "curvature_struct.h"
#include "surface_acc.h"

Mesh_curv* create_mesh_curv(Surf_param *s, 
			    double dist_sing_constr,
			    double dist_sing_min,
			    Param_maillage *param_s, 
			    int nbx, int nby);
void free_mesh_curv(Mesh_curv *m);

int calcul_secteur_courbe_geod(Surf_param *s, Mesh_curv *mesh, 
			       SListe *c, int ind_c, 
			       Param_maillage *param_s,
			       ListeD **inits, int *nb_init);
int calc_curv(Surf_param *s, Courbe *c1, Courbe *c2, 
	      SListe *_c1, SListe *_c2,	      
	      double *res, Mesh_curv **mesh, 
	      Param_maillage *param, double *h, 
	      PListeD init[2][10], 
	      int *nb_init, int nbx, int nby);
void mesh_suppr_passage_curv(Mesh_curv *mesh, PListeD init[2][10], 
			     int nb_init[2], Param_maillage *param, 
			     int ind1, int ind2);
#endif

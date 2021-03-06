#ifndef INIT_MAIN_ACC

#define INIT_MAIN_ACC

#include "../../courbe/courbe_struct.h"
#include "../../surface/surface_struct.h"
#include "../../maillage/maillage_struct.h"
#include "../../maillage/elements/quadr_struct.h"
#include "../../parametre/parametre_maillage/parametre_maillage_acc.h"
#include "../../parametre/erreur/erreur_struct.h"

/*__inline */ double r_ext_tore(double t, void *param);
/*__inline */ double r_ext_tore_p(double t, void *param);

Surf* init_surf(int type_surf, Pt *init, double *v_init, 
		Error_type *err);
int discretiser_courbe(Surf_param *s, Courbe *c, double *h, int nb,
		       Param_maillage *param, Pt *res, 
		       int *bool_fermer);
Maillage* init_courbes_primales(Surf_param *s, Pt *c1, Pt *c2, 
				int nbptr1, int nbptr2, double h, 
				Param_maillage *param);
Maillage* create_sing_dual(Surf_param *s, TabPT *c, int *nbpts, int nbc,
			   double pas, Param_maillage *param,
			   int fermer_sing, double _eps, 
			   int bool_algo);
void free_constr(Maillage *m, int ind, Param_maillage *param);
void update_sing(Maillage *m);

#endif

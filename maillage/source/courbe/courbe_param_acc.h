#ifndef COURBE_PARAM_ACC

#define COURBE_PARAM_ACC

#include "courbe_param_struct.h"

CourbeP* create_courbeP(Fonc_vect *f, double p_init, 
			double p_fin, int est_per, 
			double in, int dir, P3D param_init);
double courbe_get_dir(CourbeP *c, double d, P3D res);
int courbe_position_borne(CourbeP *c, double d);
int courbe_deplacer(CourbeP *c, double* d1, double delta);
void free_courbeP(CourbeP *c);
void courbeP_get_image(CourbeP *c, double d, P3D res);
void courbeP_get_image_p(CourbeP *c, double d, P3D res);

#endif

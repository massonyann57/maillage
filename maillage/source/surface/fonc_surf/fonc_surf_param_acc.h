#ifndef FONC_SURF_PARAM_ACC

#define FONC_SURF_PARAM_ACC

Champ_vect* create_champ_plan_param();
Champ_vect* create_champ_sphere_param(P3D r);
#ifndef _WINDOWS
Champ_vect* create_champ_trinoid_param();
#endif
Champ_vect* create_champ_enneper_param(int n);
Champ_vect* create_champ_tmp_param(int n, double d);
Champ_vect* create_champ_catenoid_param();
Champ_vect* create_champ_catenoid2_param();
Champ_vect* create_champ_plan_param2();
Champ_vect* create_champ_cube_param(double coeff1, 
				    double coeff2);
Champ_vect* create_champ_tore_param(double a, Fonc_r *b);
Champ_vect* create_champ_enneper2_param(double R1, double R2, int n);
Champ_vect* create_champ_boite_oeuf_param();

#endif

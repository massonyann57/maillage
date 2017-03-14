#ifndef COURBE_ACC

#define COURBE_ACC

Courbe* create_courbe(int type, CourbeP *c_param, CourbeG *c_geod, 
		      CourbePts *c_pts);
CourbeG* create_courbeG(Pt *init, P3D dir_init, double *pas, int size, 
			double *courbure);
CourbePts* create_courbePts(P3D *pts, int nb);

void init_courbe_geod(Courbe* c, P3D dir_init, 
		      Pt *init, double pas, 
		      int nb, double *courbure);
void init_courbe_param(Courbe* c, Fonc_vect *f, double p_init, 
		       double p_fin, double in, int dir);
void set_init_courbe(Courbe *c, double in);

void free_courbeG(CourbeG* c);
void init_courbePts(Courbe* c, P3D *param, P3D *pts, int nb);

#endif

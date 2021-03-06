#ifndef GENERAL_FUNCTIONS_ACC

#define GENERAL_FUNCTIONS_ACC

#include "../parametre/global_def.h"

/*__inline */ int estNulle(double d);
/*__inline */ int estPetit(double d);
/*__inline */ int estPetit2(double d);
/*__inline */ int estPetit_tol(double d, double tol);
/*__inline */ int signe(double d);
/*__inline */ int _max(int i, int j);
/*__inline */ int _min(int i, int j);
/*__inline */ double _fmax(double i, double j);
/*__inline */ double _fmin(double i, double j);
/*__inline */ double norme(P3D d);
/*__inline */ double _norme(P3D p, int d);
/*__inline */ double prod_scal(P3D d1, P3D d2);
/*__inline */ double distance(P3D p1, P3D p2);
double func_sphere(double* x, double R, P3D c);
void func_sphere_p(double* x, double R, P3D c, 
		   double *res);
double fonc_angle(double* x, P3D* base, double the, 
	     double* pos);
void fonc_angle_p(double*  x, P3D *base, double the, 
	     double* pos, double* res);
void prod_vect(P3D x, P3D y, P3D res);

double f_boite_oeuf(double x, void *dummy);
double f_boite_oeuf_p(double x, void *dummy);

int solv_2nd_degre(double a, double b, double c, 
		   double res[2]);

double f1(double x);
double g1(double x);
double deter(P3D* mat, int size);
int inverse(P3D* m, int size, P3D* res);
int inverse_tol(P3D* m, int size, P3D* res, double tol);
void cofact(double m[3][3], double res[3][3]);
void prod_mat(double m1[3][3], int nbl1, int nbc1,
		     double m2[3][3], int nbc2,
		     double res[3][3]);
void prod_mat_vect(double m[3][3], int nbl, int nbc,
			  double vect[3], double res[3]);
double trace(double m[3][3]);
void prod_mattransp_mat(double m[3][3], double res[3][3], 
			       int nbl, int nbc);

double intersection_segm_cercle(P3D p1, P3D p2, P3D c, 
				       double h);
int intersection_droite_cercle(P3D p1, P3D vect, 
				      P3D c, double h, 
				      P3D res[2]);
void calc_nouv_base_contr(P3D nor, P3D v, P3D base[2]);
void calc_base_dir(P3D grad[2], P3D metr[2], P3D v, P3D base[2]);
void get_ortho(P3D vect, P3D base[2], P3D res);
int coordonne(P3D vect, P3D base[2], 
	      P3D metr[2], P3D res, double min_metr, 
	      int bool_norme, int bool_dege);
void coordonne_bon(P3D vect, P3D base[2], P3D res);

int normer(P3D vect);
void orthonormer(P3D vect[2]);
void rotation_vect(P3D vect, P3D base[2], 
			  double the, P3D res);
void rotation_pt(P3D init, P3D pt, P3D base[2], 
			double the, P3D res);

Pt* createPtN();
void ptcopy(Pt* nouv, Pt *modele);
void orienter_base(P3D base[2], int bool, P3D b_init[2]);


void export_fonc_gnuplot3D(double **tab, double *X,
			   double *Y, int s1, int s2,
			   char *path);
void export_fonc_gnuplot2D(double *tab, double *X,
			   int s, char *path);

void dbg_delta(int a[SIZE_TAB][SIZE_TAB],
	       int b[SIZE_TAB][SIZE_TAB],
	       int s, int *s2);
void dbg_export(P3D p1, P3D p2, P3D p3, P3D p4, char* path);
void export_courbe(Pt *pts, int nb, char *path);
int read_liste_reel_fic(char *path, double *res);

double angle(P3D v1, P3D v2);
int factorielle(int n);

void set_ident(P3D mat[2]);
void homothetie(P3D vect, double scal);
void get_path_filelog(char *res, char *name);
int appartient_quadr(PPt *pts, Pt *p, double borne[2][2]);
int intersection_segm_quadr(PPt *pts, P3D pt1, P3D pt2, 
			    int *ind, double *prop, 
			    int *pos_prec, int *pos_min);
void projeter_vect(P3D base[2], P3D vect, P3D coor);
int est_dans_borne(Pt *p, double borne[2][2]);
double prod_scal_metr(P3D v1, P3D v2, P3D metr[3]);
double norme_metr(P3D v, P3D metr[3]);
int ajuste_borne(Pt *p1, Pt *p2, double borne[2][2]);
int ajuste_borne_dir(Pt *p1, Pt *p2, double borne[2][2], int i);
int dir_suiv_mauv_inter(Pt *p, double borne[2][2], int inter);

void exp_pt_liste(void *el, void *param);

#endif

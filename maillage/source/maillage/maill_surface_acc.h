#ifndef H_MAILL_SURF_ACC

#define H_MAILL_SURF_ACC

#include "maillage_struct.h"
#include "../surface/surface_struct.h"
#include "newton_maill/newton_maill_struct.h"

#include "../parametre/parametre_maillage/parametre_maillage_acc.h"

#include "../courbe/courbe_struct.h"

/* estimation pour initialiser la méthode de Newton */
int estim_pt_suiv_courb(Surf_param *s, Param_maillage *param, Pt* prec, 
			 P3D base[2], 
			 double alph, double pourc, 
			 double h, P3D res);
int estim_pt_suiv_surf(Surf_param *s, Param_maillage *param, 
		       P3D Xi1, P3D Xi2, Pt *prec,
		       double pourc, P3D res);
int deplacement_infinitesimal(Surf_param *s, Param_maillage *param, Pt* prec, P3D delta, P3D res);
double iter_deplacement_infinitesimal(Surf_param *s, Pt* prec, 
				    P3D delta, P3D res);
   
/* appel de l'algorithme de Newton */
double point_suivant_surface(P3D Xi1, P3D Xi2, Pt* prec, 
			     Param_new1 *param,
			     int nbMax, Pt* res,
			     Param_maillage* param_s, P3D init);

int point_suivant_courbeG(Pt *Xi, double* Xi_, 
			 Param_new2 *param,
			 int nbMax, Pt* res,
			 Param_maillage* param_s, P3D metr[3], 
			  double pas_init, double* init);
int point_suivant_courbeP(Surf_param *s, double init_p, double dist_courb,
			  Param_new3 *param,
			  Param_maillage* param_s, 
			  double *res_param_courbe);
 
int inters_double_plan_surface(Pt* Xi1, Pt* Xi2,
			       P3D v1, P3D v2, P3D *base, 
			       Param_new5 *param, 
			       int nbMax, Pt* res, 
			       Param_maillage* param_s);

/* calcul des courbes initiales */
int calc_courbeG(CourbeG *cg, Surf_param *s, double *h,
		 Param_maillage *p_maill,
		 Pt *res, int nb);
int calc_courbeP_surf(int nbptr, 
		      CourbeP *c_plan, CourbeP *c_surf,
		      double *h, Surf_param *s, 
		      Param_maillage *p_maill, 
		      Pt *res, double *dern_param, double tol_gross);
int calc_courbeP_ferme(int nbptr, 
		      CourbeP *c_plan, CourbeP *c_surf,
		      double *h, Surf_param *s, 
		      Param_maillage *p_maill, 
		       Pt *res, double *dern_param, 
		       double tol);

int maillage_che(Maillage *m, Param_maillage* param_s, 
		 int bool_unique, 
		 ListeD *init, int ind_part, int ind_constr);

int ajoute_courbe_dual_maillage(Maillage *m,
				Courbe *c, ListeD *init, 
				int or_primal, int or_vir, 
				Param_maillage* param_s, 
				int type_maill, int ind_constr);

void ajoute_diag_ferme_maill(Maillage *m, PPt *pts, 
			     int nb, ListeD *init, int or, 
			     int or_vir, 
			     Param_maillage* param_s, int or_base, 
			     int type_maill, int ind_constr);
int ajoute_diag_maill(Maillage *m,
		       PPPt *pts, int nb_dual, int* nb,
		       ListeD *init, int or, int or_vir, 
		       int or_voistopt, 
		       Param_maillage* param_s, 
		       int or_base, int type_maill, 
		      void (*callback_stop)(Maillage*,int,int*,
					    void*), 
		      void *param_call, int fermer_sing, 
		      int ind_constr);
int calc_diag(Maillage *m, 
	      double the, double h, P3D dir,
	      Param_maillage *p_maill, int or, int or_vir,
	      int or_base, ListeD *linit, int ind_constr);
void deplacer_courbe(Surf_param *s, Pt *pts, int nbpts, Pt *p,
		     Pt *res, Param_maillage *p_maill);
int calc_courbe(int nbptr, Courbe *c, CourbeP *c_plan,
		double *h, Surf *s, 
		Param_maillage *p_maill, 
		Pt *res, double *dern_param);


/*void calc_maillage_diag(Surf *s, Pt *init, Param_maillage *param_m,
			Courbe *c1, Courbe *c2, P3D dir2, 
			double ang_init, double R1,
			double R2, int nbptr, double h,
			int or, int or_vir, int or_base);*/
int calc_courbe_sing(Maillage *m,
		     int nbptr, Pt* pt_suppr, 
		     Pt* npc2,
		     double* the1, double* the2, double h_m, 
		     double* h_d, P3D dir_init,
		     P3D dir_init2, 
		     Param_maillage *p_maill, int or, 
		     ListeD *linit);
Maillage* 
calc_maillage_init_sing(int nbptr, Pt* init, 
			double h, P3D dir[2], 
			Surf_param *s, double *the1,
			double *the2,
			Param_maillage *p_maill);
Maillage* 
calc_maillage_init_sing2(int nbptr, Pt* init, 
			 double h, P3D dir[2], 
			 double *psi, double psi2, 
			 Surf *s, double *the1, 
			 double *the2,
			 Param_maillage *p_maill, 
			 int or);
Maillage* calc_maillage_init_sing3(int nbptr, Pt* init, 
				   double h, P3D dir[2], 
				   double eta,
				   double thet0, double R1,
				   double R2, Surf *s,	   
				   Param_maillage *p_maill, 
				   int or, int bool_bezier);
void ajout_courbe_dual(Maillage *m, Surf *s, PPt *pts, int nbpts,
		       Param_maillage* param_s, int or, 
		       int or_vir, int bool_fermer, int ind_constr);
void jonction_dual_pts(Maillage *m, int nb_dual, 
		       PPt *pts, PPPt *npts, int *nbpts,
		       Param_maillage* param_s, 
		       int or, int or_vir, double eps, 
		       int fermer_sing, int ind_constr);

int calc_nouv_pt_courbe(Surf_param *s, int nb, Pt* p1, Pt *p_prec, 
			Pt *pts, double pas_pos, 
			double pas_centre,
			Param_new2 *param_new, Pt *res, 
			Param_maillage *param_m, 
			int bool_p1_centre);
int nouv_pt_courbe(Surf_param *s, double *centre, Pt *pos, P3D dir, 
		    double pas_centre, 
		    Param_new2 *param_new, Pt *res, 
		    Param_maillage *param_m);
int calc_courbePts(Surf_param *s, CourbePts *c, 
		   Param_maillage *param, 
		   double *h, int nb, Pt *res);
double maillage_projection_bord(Maillage *m, double* p, 
				ListeD **ret_l);
void maill_dir_tan(Surf_param *s, Courbe *c, double param, int ind,
		   Pt *pos, Pt *prec, double *dir_tan);
void maill_dir_tan_init(Surf_param *s, Courbe *c, 
			double *dir_tan);
int subd_courbe(Surf_param *s, Param_new2 *param_new, 
		SListe *c, Liste *pt, 
		double R, double dist_min, int nbpt, 
		Param_maillage *param_m);

#endif

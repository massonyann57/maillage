#ifndef H_MAILLAGE_ACC

#define H_MAILLAGE_ACC

#include "maillage_struct.h"
#include "../parametre/parametre_maillage/parametre_maillage_struct.h"

Maillage* createMaillage(Pt* init,  double h, Surf_param *s);
Param_call1* create_param_call1(int or, double dist, 
				Pt *pt, ListeD *prec);

int ajouterQuadr(Maillage *m, Param_maillage *param_s, 
		 ListeD *newl, ListeD *p_maill, 
		 int ind_surf, int or, int est_etoile, int del);
void ajouterQuadr_3pts(Maillage *m, ListeD *p1, 
		       ListeD *p2, int ind_surf, int or);
void ajouterPoint_comp(Maillage *m, Param_maillage *param_s, 
		       Pt* new_p, int ind_constr,
		       ListeD *p_maill, int ind_surf, int or, 
		       int boo_sing);
void ajoute_courbe_maill(Maillage *m, PPt *pts,
			 int nb, int ordre, ListeD *init,
			 int or, int or_voistonew,
			 double tol_pas, Param_maillage *param, 
			 int ind_constr);
Point* ligne_get_pt(ListeD *l);
void ligne_set_pt(ListeD *l, Ptmaill *p);
void ajoute_pt(Maillage *m, ListeD *init, int or,
	       int or_voistonew, Ptmaill *newp, Param_maillage *param);
void ajoute_pt_exis(Maillage *m, ListeD *init, int or,
	Ptmaill* pt, ListeD *nl, Param_maillage *param);
void maill_export_paraview(Maillage *m, Param_maillage *param, 
			   int bool_pull, int bool_dual, 
			   int bool_curv,
			   char *path, char *nom, int ind_part, 
			   Mapping map, void *param_map, 
			   P3D **exp_pts, int **exp_quadr, 
			   int **exp_nbpts_quadr, int *size);
void ajoute_vois_seg(Maillage *m, ListeD *init, int or,
		     int or_voistopt, ListeD *lnewp);
void maillage_stop_bord(Maillage *m);
void maillage_suppr_der_ligne(Maillage *m, Param_maillage *param, 
			      int bool_bord, int ind_part, 
			      int or, int or_vir);
void suppr_seg(Maillage *m, ListeD* l, int dir, int bool_def, Param_maillage *param);
void suppr_point(void *pt, void *param_call, 
		 Param_maillage *param, int bool_def);
int fusion_quadr(Maillage *m, Param_maillage *param, PListeD lpt[2], 
		 int ind_ros, double tol_retour);
Pt* getvect(ListeD *l);
//void suppr_diag(Maillage *m, ListeD *l);

void remplacer_point(Maillage *m, ListeD *pt_suppr, 
		     ListeD *remp, int or);
void fusion_composantes_maillage(Maillage *m, ListeD *pt_suppr, 
				 ListeD *remp, int or, int or_vir);

int callback_stop_distance_pt(Maillage *m, void *param);
int extraire_diag(ListeD *in, int or, int or_vir, Pt *c_res);

void export_pas_diag(ListeD *init, int or, 
		     int or_vir, char *path, int bool_milieu);
void free_maillage(Maillage *m, Param_maillage *param);

void maillage_set_sing(Maillage *m, ListeD *pt);
int maillage_fermer_sing(Maillage *m, ListeD *sing);

void supprQuadr(Maillage *m, ListeD *lq, int bool_def, Param_maillage *param);
void callback_supprQuadr(void *lq, void *maill);
int free_branches(Maillage *m, Param_maillage *param, ListeD *l);
void free_saut_sing(Maillage *m, ListeD *sing, int ind);
void maillage_suppr_sing(Maillage *m, ListeD *pt);

#endif

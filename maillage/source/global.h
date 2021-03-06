#ifndef _GLOBAL_ACC

#define _GLOBAL_ACC

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "parametre/erreur/erreur_acc.h"
#include "outils/init_main/init_main_acc.h"
#include "surface/fonc_surf/fonc_surf_contr_acc.h"
#include "surface/fonc_surf/fonc_surf_param_acc.h"
#include "surface/surface_struct.h"
#include "surface/surface_acc.h"

#include "surface/mapping_acc.h"
#include "maillage/maillage_acc.h"
#include "maillage/maillage_struct.h"
#include "maillage/maill_surface_acc.h"
#include "maillage/dual_acc.h"

#include "maillage/elements/quadr_acc.h"
#include "maillage/elements/quadr_struct.h"

#include "outils/listes/listeD/callback_listeD_acc.h"
#include "outils/listes/listeD/listeD_acc.h"
#include "outils/listes/liste/liste_acc.h"

#include "outils/functions/functions_acc.h"

#include "outils/general_functions_acc.h"

#include "parametre/parametre_maillage/parametre_maillage_acc.h"
#include "test/test_vecteurs_acc.h"

#include "courbe/fonction/fonction_courbe_param_acc.h"
#include "courbe/courbe_acc.h"
#include "courbe/courbe_param_acc.h"


#ifdef _COMPILE_WIN

#include <process.h>
#include <windows.h>
#define DLLEXP __declspec(dllexport)
#define _CDECL __cdecl
#else
#define DLLEXP 
#define _CDECL 
#endif

extern PMaillage save[NB_SAVE_MAX];
extern int nb_maillage_saved;

DLLEXP void _CDECL exp_free_memory(P3D *pts, int *quadr, 
				   int *nbpts_quadr);
DLLEXP void _CDECL exp_free_maillage(Maillage *m);
DLLEXP CourbeP* 
_CDECL exp_createCourbe_param(void (*f)(double, void*, P3D),
			     void (*f_p)(double, void*, P3D), 
			     void *param_f, double init, 
			     double fin, int est_per, 
			     double pos_init, int dir);
DLLEXP Courbe* 
_CDECL exp_createCourbe(P3D *param, P3D *pts, int nbpts, 
		       CourbeP *c_param, double p_init,
		       double *param_init, double *dir_init,
		       double *the, double pas);
DLLEXP Surf* _CDECL 
exp_createSurface(void (*f)(P3D, void*, P3D),
void(*f_p)(P3D, void*, P3D*), double(*f_gauss)(P3D, void*),
		  void *param_f, double init1, double fin1,
		  double init2, double fin2, int ind_quotient);
DLLEXP Surf* 
_CDECL exp_createSurface_exemple(int type, P3D param_init, 
				P3D vect);
DLLEXP Maillage* _CDECL 
exp_maillage_courbes_primales(Surf *s, Courbe *c1, Courbe *c2,
			      double h, int nb1, int nb2,      
			      double tol, char *path_filelog, int *indice);
DLLEXP Maillage* _CDECL 
exp_maillage_primal_dual(Surf *s, Courbe *c1, Courbe *c2, int bool_swi,
			 double h, int nb1,
			 double tol, char *path_filelog, int *indice);
DLLEXP void _CDECL 
exp_exporter_maillage(Maillage *m, int bool_pull,
		      int bool_dual, int bool_curv, 
		      char *path, char *name_file, 
		      P3D **res_pts, int **res_quadr, int **res_nbpts_quadr,
		      int *size_pts, int *size_quadr);
DLLEXP Maillage* _CDECL 
exp_ajouter_courbe(Maillage *m, Surf *s, double *pt, Courbe *c, 
		   int est_primal, double *h, double pas, 
		   int nb, double tol, int bool_fermer, int bool_fermer_surf,
		   char *path_filelog, int *indice, int ind_constr);
DLLEXP Maillage* _CDECL 
exp_maillage_sing(Surf *s, PCourbe *c, int *nbpts, double **H,
		  int nbc, double pas,
		  double tol, char *path_filelog, double eps, int *indice);
DLLEXP Maillage* _CDECL exp_get_maill(int indice);
int ajouter_maillage(Maillage *m, Param_maillage *param);
DLLEXP Maillage* _CDECL
exp_generer_sing_neg(Surf *s, P3D param_init, P3D dir, double h,
		     int nbptr, double tol_maill, double tol,
		     int *ind_maill);

#endif

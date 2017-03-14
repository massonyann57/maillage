#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <stdlib.h>
#include <string.h>

#include "surface/fonc_surf/fonc_surf_contr_acc.h"
#include "surface/fonc_surf/fonc_surf_param_acc.h"
#include "surface/surface_struct.h"
#include "surface/surface_acc.h"

#include "surface/mapping_acc.h"

#include "maillage/maillage_acc.h"
#include "maillage/maillage_struct.h"
#include "maillage/maill_surface_acc.h"

#include "maillage/elements/quadr_acc.h"
#include "maillage/elements/quadr_struct.h"

#include "outils/listes/listeD/callback_listeD_acc.h"
#include "outils/listes/listeD/listeD_acc.h"


#include "outils/listes/liste/liste_acc.h"
#include "outils/general_functions_acc.h"

#include "outils/init_main/init_main_acc.h"

#include "parametre/parametre_maillage/parametre_maillage_acc.h"
#include "test/test_vecteurs_acc.h"

#include "courbe/fonction/fonction_courbe_param_acc.h"
#include "courbe/courbe_acc.h"
#include "courbe/courbe_param_acc.h"

#include "global.h"

int main(int argc, char *argv[], char *envp[]){
  Param_maillage *param;

#ifdef _COMPILE_WIN
  char _path_filelog[SIZE_TAB] = "C:\\Users\\Cermics\\Desktop\\test\\test.log";
  char path[SIZE_TAB] = "C:\\Users\\Cermics\\Desktop\\test\\";
#else
  // char _path_filelog[SIZE_TAB] = "test.log";
  char path[SIZE_TAB] = "";
#endif
  char *path_filelog = NULL;

  double h = 0.1;
  //  int N = 100;
  int nbptr = 200;
  // int nb_discr = 1;
  
  //  double size_exp = 1;

  //  int nb_discr_etoi = 30;
  int i = 1;
  int type_test;
  //  int bool_init = 0;
  //  int bool_simu;

  char name[SIZE_TAB] = "test.vtk";

  /*
   * type_surf = 0 ==> goursat
   */
  int type_surf = -1; //le type de la surface
  //  int type_calc = 0;
  /*
   * type_calc = 0 ==> un calcul
   *           = 1 ==> boucle
   */
  param = create_param_maillage(0.0, path_filelog, 0.0);
  /*
   * on maille par une singularitée ?
   */
  // int bool_etoile = 0;
  while(i<argc){
    if(argv[i][0] != '-')
      printf("erreur entree parametre (pas de '-' à l'entree %d)\n", i);
    /* else if(strcmp(argv[i], "-maillage") == 0){
       type_calc = 0;
       }*/
    else if(strcmp(argv[i], "-boucle") == 0){
      //type_calc = 1;
    }
    else if(i+1 >= argc || argv[i+1][0] == '-')
      printf("erreur entree parametre (pas d'entree apres le parametre %d)\n", i);
    else if(strcmp(argv[i], "-type") == 0){
      i++;
      type_surf = atoi(argv[i]);
    }
    else if(strcmp(argv[i], "-nbptr") == 0){
      i++;
      nbptr = atoi(argv[i]);
    }
    else if(strcmp(argv[i], "-N") == 0){
      i++;
      //N = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-h") == 0){
      i++;
      h = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-bool_suppr") == 0){
      i++;
      param->bool_suppr_doublons = atoi(argv[i]);
    }
    else if(strcmp(argv[i], "-bool_sing") == 0){
      i++;
      param->bool_sing = atoi(argv[i]);
    }
    else if(strcmp(argv[i], "-type_test") == 0){
      i++;
      type_test = atoi(argv[i]);
    }
    else if(strcmp(argv[i], "-size_exp") == 0){
      i++;
      //size_exp = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-nb_discr") == 0){
      i++;
      //nb_discr = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-bool_init") == 0){
      i++;
      //bool_init = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-bool_etoile") == 0){
      i++;
      //bool_etoile = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-nb_discr_etoi") == 0){
      i++;
      //nb_discr_etoi = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-bool_simu") == 0){
      i++;
      //bool_simu = atof(argv[i]);
    }
    else if(strcmp(argv[i], "-name") == 0){
      i++;
      sprintf(name, "%s", argv[i]);
    }
    else if(strcmp(argv[i], "-dir") == 0){
      i++;
      sprintf(path, "%s", argv[i]);
    }   else {
      printf("erreur entre %d : parametre \"%s\" inexistant\n", i, argv[i]);
    }
    i++;
  }
  if (type_test == 6 && param->bool_sing != 1){

    /*  printf("!!!! erreur possible : on ne s'arrete pas après une singularité, ce qui peut amener des bugs dans le cas où on maille par une diagonale\n"); */
  }

  param->pas_max_new = h;
  /* test_rosace(s, v_init, h, init, nbptr,
     path, param, nbptr);*/
  if(type_test==0){
  }
  else if (type_test == 8){
    /*   P3D *pts;
	 _QUADR *quadr;
	 int size_pts, size_quadr;*/
    char name[SIZE_TAB] = "test.vtk";
    char name_dual[SIZE_TAB] = "test_dual.vtk";
    //    char name_curv[SIZE_TAB] = "test_curv.vtk";
#ifdef _COMPILE_WIN
    char path[SIZE_TAB] = "C:\\Users\\yannick.masson\\Desktop\\test\\";
    char _path_filelog[SIZE_TAB] = "C:\\Users\\yannick.masson\\Desktop\\test\\tmp.log";
#else
    char path[SIZE_TAB] = "";
    //   char _path_filelog[SIZE_TAB] = "tmp.log";
    char *path_filelog = NULL;
#endif
    //char *path_filelog = NULL;

    P3D param_init;
    P3D vect;
    double the[SIZE_TAB];
    Maillage *m = NULL;
    Surf *s = exp_createSurface_exemple(type_surf, param_init, 
					vect);
    Courbe *c1;
    Courbe *c2;
    CourbeP *cp, *cp2;
    double R[4];
    double tol = 1.;
    int type_test2 = 1;
    int type_test3 = 0;
    int nb_dir = 5;
    int bool_curv = 1;
    double H[SIZE_TAB];
    P3D vect_cp;
    int N2 = 50;
    int _m = _max(N2,nbptr);
    //   double del_theta = 0.1*PI;

    bool_curv=0;
    if(bool_curv){
      type_test2 = 0;
      type_test3 = 1;
    } 
    if(/*!bool_curv*/1){

      for(i = 0;i<_m;i++){
	//  H[i] = h*(0.5+((double)i)/((double)nbptr));
	H[i] = h*0.9;
      }

      R[0] = 0.2;R[1] = 0.2;R[2] = 0.0;R[3]=0.0;
    
      for(i = 0;i<nbptr;i++)
	the[i] = 0.0;
      the[0] = PI;
      VECT_CPY(vect_cp, vect);
      c1 = exp_createCourbe(NULL, NULL, nbptr, NULL, 
			    0.0, param_init, vect, 
			    the, h);
      if(type_test2 == 0 || type_test2 == 3){
	the[0] += PI/15.;//2.0*PI/nb_dir;
	c2 = exp_createCourbe(NULL, NULL, nbptr, NULL, 0.0, 
			      param_init, vect_cp, the, h);
      } else if (type_test2==1 || type_test2 == 4){
	/*	cp = create_courbeP(create_fonction_ellipse(0.,0.,R[0],R[1]),
			    0., 2*PI, 1, 0., 1);*/
	cp = exp_createCourbe_param(NULL,NULL, 
	   NULL, -PI, PI, 1, 0.0, 0);
	cp->param=NULL;
	cp2 = exp_createCourbe_param(NULL,NULL, 
	   NULL, -PI, 2*PI, 0, 0.0, 3);
	cp2->param = NULL;
	c1 = exp_createCourbe(NULL, NULL, nbptr, cp, 0.0, NULL, 
			      NULL, NULL, h);
	c2 = exp_createCourbe(NULL, NULL, nbptr, cp2, 0.0, NULL, 
			      NULL, NULL, h);
	//cp = surf_get_courbe_from_courbeP(s->s_param, cp2);
      } else if(type_test2 == 2 || type_test2 == 5){
	P3D param_tmp[SIZE_TAB], pts_tmp[SIZE_TAB];
	double del = 0.02;
	double par = 0.0;
	/*	cp = exp_createCourbe_param(fonction_ellipse, 
				    fonction_ellipse_p, 
				    R, 0.0, 2*PI, 1, 0.0, 1);*/
	cp = exp_createCourbe_param(NULL, NULL, NULL, 0.0, 2*PI, 1, 0.0, 1);
	/*	for(i = 0;i<nbptr;i++){
	  image_fvect(cp->param, par, param_tmp[i]);
	  image_cvect(s->s_param->f, param_tmp[i], pts_tmp[i]);
	  courbe_deplacer(cp, &par, del);
	  }*/
	c2 = exp_createCourbe(NULL, NULL, nbptr, cp, 0.0, NULL, 
			      NULL, NULL, h);
	/*	c2 = exp_createCourbe(param_tmp, pts_tmp, nbptr, NULL, 0.0,
		NULL, NULL, NULL, h);*/
      }
      if(type_test3 > 0 && type_test3 <= 2){
	if(type_test2<3){
	  m = exp_maillage_courbes_primales(s, c1, c2, h, 
					    nbptr, nbptr, 
					    tol, path_filelog, NULL);
	} else {
	  m = exp_maillage_primal_dual(s, c1, c2, 0, h, nbptr, 
				       tol, path_filelog, NULL);
	}
	if(type_test3 == 2){
	  //int type_test4 = 0;
	  Courbe *c3;
	  Ptmaill *ptm_init = m->lignes->l[0]->el->el;
	  Ptmaill *ptm_tmp;
	  double *pt_init = ptm_init->p->pt->p;
	  int bool_primal = 1;
	  int fermer_sing = 0;
	  ListeD *init, *tmp;
	  int ind_constr = 1, j;
	  //	nbptr = 40;
	  /*	if(type_test4 == 1)
		bool_primal = 0;*/

	  for(i = 2;i<nb_dir;i++){
	    the[0] = ((double)i)*2.*PI/(nb_dir+1); 
	    c3 = exp_createCourbe(NULL, NULL, nbptr, NULL, 
				  0.0, param_init, vect, 
				  the, h);
	    if(i==nb_dir-1){
	      fermer_sing = 1;
	      //     ind_constr--;
	    }

	    exp_ajouter_courbe(m, NULL, pt_init, 
			       c3, bool_primal, H, 0.0, nbptr, 
			       tol, fermer_sing, 1, 
			       path_filelog, NULL, ind_constr);
	    ind_constr++;
	  }
	  tmp = m->lignes->l[0];
	  while(tmp != NULL){
	    init = tmp->nL;
	    if(init != NULL)
	      tmp = init->nL;
	    else {
	      init = tmp;
	      break;
	    }
	  }
	  //  init = init->nH->nH->nH->nH;
	  //  ptm_tmp = init->pL->el->el;
	  //  ptm_init = init->el->el;
	  //  pt_init = ptm_init->p->pt->p;
	  the[0] = 0.;//2.*PI-0.5;
	  //  VECT_DEL(vect, ptm_tmp->p->pt->p, pt_init, i);
	  //  normer(vect);
	 /*  c1 = exp_createCourbe(NULL, NULL, nbptr, NULL,  */
	/* 			0.0, ptm_init->p->pt->param, vect,  */
	/* 			the, h); */
	/*   exp_ajouter_courbe(m, NULL, pt_init,  */
	/* 		     NULL, 0, H, 0.0, nbptr,  */
	/* 		     tol, 1, 1,  */
	/* 		     path_filelog, NULL, ind_constr); */
	/*   ind_constr++; */
	/*   for(j = 0;j<60;j++){ */
	/*     for(i = 0;i<nbptr;i++){ */
	/*       //  H[i] = h*(0.5+((double)i)/((double)nbptr)); */
	/*       H[i] = h*(1.3+0.1*((double)j)); */
	/*     } */
	/*     exp_ajouter_courbe(m, NULL, pt_init,  */
	/* 		       c1, 0, H, 0.0, nbptr,  */
	/* 		       tol, 1, 1,  */
	/* 		       path_filelog, NULL, ind_constr-1); */
	/*     sprintf(name, "test%d.vtk", j+1); */
	/*     exp_exporter_maillage(m, 0, 1, 0, path, name,  */
	/* 			  NULL, NULL, NULL, NULL, NULL); */
	/*     printf("j : %d   pt_suppr : %d   quadr_suppr : %d    seg_suppr : %d\n",  */
	/* 	   j, m->lignes->mailles[0]->nb_suppr,  */
	/* 	   m->quadr->mailles[0]->nb_suppr,  */
	/* 	   m->seg->nb_suppr); */
	/*   } */
	/*   //	free_constr(m, 2, param); */
	} 
      } else if(type_test3 == 0){
	ListeD *tmp, *init;
	Ptmaill *pt_end;
	Pt *_tmp;
	the[0] = 0.8*PI;
	m = exp_ajouter_courbe(NULL, s, NULL, c2, 0, H, 
			       h, nbptr, tol, 1, 1, 
			       path_filelog, NULL, 0);

	tmp = m->lignes->l[0];
	pt_end = (Ptmaill *) tmp->el->el;
	_tmp = pt_end->p->pt;

	cp = exp_createCourbe_param(NULL,NULL, 
				    NULL, -PI, PI, 
				    1, _tmp->param[1], 1);
	cp->param=NULL;
	c1 = exp_createCourbe(NULL, NULL, nbptr, cp, 0.0, NULL, 
			      NULL, NULL, h);  
	exp_ajouter_courbe(m, NULL, _tmp->p, c1, 1, H, 0.0, N2,  
			   tol, 0, 0,  path_filelog, NULL, 1);
	while(tmp != NULL){
	  init = tmp->nL;
	  if(init != NULL)
	    tmp = init->pH;
	  else {
	    init = tmp;
	    break;
	  }
	}
	pt_end = (Ptmaill *) init->el->el;
	_tmp = pt_end->p->pt;
	cp = exp_createCourbe_param(NULL,NULL, 
				    NULL, -PI, PI, 
				    1, _tmp->param[1], 3);
	cp->param=NULL;
	cp->param_init[0] = _tmp->param[0];
	cp->param_init[1] = _tmp->param[1];
	for(i=0;i<N2;i++)
	  H[i]/=2.;
	  
	c1 = exp_createCourbe(NULL, NULL, nbptr, cp, 0.0, NULL, 
			      NULL, NULL, h);  
	exp_ajouter_courbe(m, NULL, _tmp->p,  
			   c1, 1, H, 0.0, N2,  
			   tol, 0, 0,  path_filelog, NULL, 2);
	exp_ajouter_courbe(m, NULL, _tmp->p,  
			   c1, 0, H, 0.0, N2,  
			   tol, 0, 1,  path_filelog, NULL, 2);
	exp_ajouter_courbe(m, NULL, _tmp->p,  
			   c1, 1, H, 0.0, N2,  
			   tol, 1, 0,  path_filelog, NULL, 2);
	exp_ajouter_courbe(m, NULL, _tmp->p,  
			   c1, 1, H, 0.0, N2,  
			   tol, 0, 0,  path_filelog, NULL, 3);
      } else if(type_test3 == 3){
	PCourbe crbs[NB_BRANCHES_MAX];
	int size[NB_BRANCHES_MAX];
	int nbc = 7;
	for(i = 0;i<nbptr;i++)
	  the[i] = 0.0;
	for(i = 0;i<nbc;i++){
	  size[i] = nbptr;
	  crbs[i] = exp_createCourbe(NULL, NULL, size[i], NULL, 
				     0.0, param_init, vect, 
				     the, h);
	  the[0] += 2.0*PI/((double)nbc);
	}
	m = exp_maillage_sing(s, crbs, size, NULL, nbc, h, 
			      tol, path_filelog, 10.0, NULL);
      }
      exp_exporter_maillage(m, 0, 0, 0, path, name, 
			    NULL, NULL, NULL, NULL, NULL);
      exp_exporter_maillage(m, 0, 1, 0, path, name_dual, 
			    NULL, NULL, NULL, NULL, NULL);
    }
    bool_curv=0;
    if(bool_curv){
      double tol_maill = 15.;
      double tol = 40.;
      P3D base[2], dummy[2];

      if (!surf_calc_base(s->s_param, param_init,
			  base, dummy))
	return 0;
      m = exp_generer_sing_neg(s, param_init, base[0], h, nbptr, 
			       tol_maill, tol, NULL);
      exp_exporter_maillage(m, 0, 0, 0, path, name, 
			    NULL, NULL, NULL, NULL, NULL);
      exp_exporter_maillage(m, 0, 1, 0, path, name_dual, 
			    NULL, NULL, NULL, NULL, NULL);
      /* exp_exporter_maillage(m, 0, 0, 1, path, name_curv, 
	 NULL, NULL, NULL, NULL, NULL);*/
    }
  }
  return 0;
}
    

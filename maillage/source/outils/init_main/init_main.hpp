#include "init_main.h"

Surf* init_surf(int type_surf, Pt *init, double *v_init, 
		double **v_init2, Error_type *err){
  double init_u, fin_u;
  double init_v, fin_v;
  if(type_surf == 0){
    init->p[0] = 1.0; init->p[1] = 0.0; init->p[2] = 0.0;
    v_init[0] = 0.0;  v_init[1] = 1.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0, -1,
		    create_champ_goursat());
  }
  else if(type_surf == 1){
    init->p[0] = 1; init->p[1] = 0; init->p[2] = 0;
    v_init[0] = 0.0;  v_init[1] = 1.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_sphere());
  }
  else if(type_surf == 2){
    init->p[0] = 1;
    init->p[1] = 0/*sqrt((double)TMP_BOULE)*/; init->p[2] = 0;
    v_init[0] = 0;  v_init[1] = 1.0; v_init[2] = 0;
	
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_sphere());
  }
  else if(type_surf == 3){
    init->p[0] = TORE_R2-TORE_R1; init->p[1] = 0; init->p[2] = 0;
    v_init[0] = 0.0;  v_init[1] = 1.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_tore());
  }
  else if(type_surf == 4){
    init->p[0] = 1; init->p[1] = 0; init->p[2] = 0;
    v_init[0] = 0.0;  v_init[1] = 0.0; v_init[2] = 1.0;
  
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_hyperb());
  }
  else if(type_surf == 5){
    init->p[0] = 0; init->p[1] = 0; init->p[2] = 0.1;
    v_init[0] = 1.0;  v_init[1] = 0.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_tore2T());
  }
  else if(type_surf == 6){
    double coeff1 = COEFF_BOULE_ENF1;
	
    init->p[0] = 0; init->p[1] = 0;
    init->p[2] = sqrt(1-coeff1*(1+tanh(3)));
    v_init[0] = 1.0;  v_init[1] = 0.0; v_init[2] = 0.0;  
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_boule_enf());
  }
  /* else if(type_surf == 7){
     init->p[0] = 0; init->p[1] = 0;init->p[2] = 2*AMPL_BOITE;
     v_init[0] = 0.0;  v_init[1] = 1.0; v_init[2] = 0.0;
  
     return create_surf(3, 0, 1, 0, -1, NULL, 0, 1, NULL, NULL,
     f_boite_oeuf, f_boite_oeuf_p,
     f_boite_oeuf, f_boite_oeuf_p,
     NULL, NULL, NULL, metrique_eucl);
     }*/
  else if(type_surf == 8){
    init->p[0] = 0; init->p[1] = 0;init->p[2] = 0;
    v_init[0] = 1.0/sqrt(2.0);  v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_conoide());
  }
  else if(type_surf == 9){
    init->p[0] = 0; init->p[1] = 0;init->p[2] = 0;
    v_init[0] = 1.0;  v_init[1] = 0.0; v_init[2] = 0.0;
  
    return create_surf(2, NULL, 0,0,0,0,-1,
		    create_champ_plan());
  }
  else if(type_surf == 10){
    P3D R;
    R[0] = 1.0;R[1] = 1.0;R[2] = 1.0;

    init->p[0] = 1.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0/sqrt(2.0); v_init[2] = 1.0/sqrt(2.0);
    init->param[0] = 0.0;
    init->param[1] = 0.0;// PI/2.0;
    init_u = -PI/2.0;
    fin_u = PI/2.0;
    init_v = -PI;
    fin_v = PI;
  
    return create_surf(1, create_champ_sphere_param(R),
		       init_u, fin_u, init_v, fin_v, 1, NULL);
  }
#ifndef _WINDOWS
  else if(type_surf == 11){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 1.0/sqrt(2.0); v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 0.0;
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -5.0;
    fin_u = 2.0;
    init_v = -3.0;
    fin_v = 2.0;
  
    return create_surf(1, create_champ_trinoid_param(), 
		       init_u, fin_u, init_v, fin_v, -1, NULL);
  }
#endif
  else if(type_surf == 12){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 1.0; v_init[1] = 0.0; v_init[2] = 0.0;
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -10000.0;
    fin_u = 100000.0;
    init_v = -100000.0;
    fin_v = 1000000.0;
  
    return create_surf(1, create_champ_plan_param(), 
		    init_u, fin_u, init_v, fin_v, -1, NULL);
  }
  else if(type_surf == 13){
    int n = 4;
    v_init2[0] = malloc(3*sizeof(double));
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0; v_init[2] = 0.0;
    v_init2[0][0] = 1.0; v_init2[0][1] = 0.0; 
    v_init2[0][2] = 0.0;
    init->param[0] = -0.3;
    init->param[1] = 0.0;
    init_u = -10000;
    fin_u = 100000;
    init_v = -10000;
    fin_v = 10000;
  
    return create_surf(1, create_champ_enneper_param(n), 
		    init_u, fin_u, init_v, fin_v, -1, NULL);
  }
  else if(type_surf == 14){
    int n = 3;
    double _d = 0.2;

    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 1.0; v_init[1] = 0.0; v_init[2] = 0.0;
    init->param[0] = 0.5;
    init->param[1] = 0.0;
    init_u = 0.001;
    fin_u = 1.0;
    init_v = 0.0;
    fin_v = 2*PI/n;
  
    return create_surf(1, create_champ_tmp_param(n, _d), 
		       init_u, fin_u, init_v, fin_v, -1, NULL);
  }
  else if(type_surf == 15){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 1.0/sqrt(2.0);
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -100.0;
    fin_u = 100.0;
    init_v = -20.0*PI-0.1;
    fin_v = 20.0*PI+0.1;

    //malloc(3*sizeof(double));
    //v_init2[0] = 0.0;v_init2[1] = 1.0;v_init2[2] = 0.0;

    return create_surf(1, create_champ_catenoid_param(), 
		       init_u, fin_u, init_v, fin_v, 1, NULL);
  }
  else if(type_surf == 16){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0/sqrt(2.0); 
    v_init[2] = 1.0/sqrt(2.0);
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -PI;
    fin_u = 2.0;
    init_v = 0.0;
    fin_v = 2*PI;

    //malloc(3*sizeof(double));
    //v_init2[0] = 0.0;v_init2[1] = 1.0;v_init2[2] = 0.0;

    return create_surf(1, create_champ_catenoid2_param(),
		       init_u, fin_u, init_v, fin_v, -1, NULL);
  }
  else if(type_surf == 17){
    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0; 
    v_init[2] = 0.0;
    init->param[0] = 1.0;
    init->param[1] = 0.0;
    init_u = 0.0000;
    fin_u = 1000;
    init_v = -2.0*PI;
    fin_v = 2.0*PI;

    //malloc(3*sizeof(double));
    //v_init2[0] = 0.0;v_init2[1] = 1.0;v_init2[2] = 0.0;

    return create_surf(1, create_champ_plan_param2(), 
		       init_u, fin_u, init_v, fin_v, 1, NULL);
  }
  else if(type_surf == 18){
    double c1 = 1.0;
    double c2 = 0.0; // pour avoir une surface de goursat : c2 = 2

    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0; 
    v_init[2] = 0.0;
    init->param[0] = 0.;
    init->param[1] = 0.;
    init_u = -PI/2.0;
    fin_u = PI/2.0;
    init_v = -PI;
    fin_v = PI;

    //malloc(3*sizeof(double));
    //v_init2[0] = 0.0;v_init2[1] = 1.0;v_init2[2] = 0.0;

    return create_surf(1, create_champ_cube_param(c1, c2), 
		       init_u, fin_u, init_v, fin_v, 1, NULL);
  }
  else if(type_surf == 19){
    double* c1 = malloc(sizeof(double));
    Fonc_r *f_tore;
    c1[0] = 1.5;
    f_tore = create_fonc_r(r_ext_tore, r_ext_tore_p, c1);

    init->p[0] = 0.0; init->p[1] = 0.0;init->p[2] = 0.0;
    v_init[0] = 0.0; v_init[1] = 1.0; 
    v_init[2] = 0.0;
    init->param[0] = 0.0;
    init->param[1] = 0.0;
    init_u = -PI;
    fin_u = PI;
    init_v = -0.4;
    fin_v = 2*PI;

    return create_surf(1, create_champ_tore_param(c1[0], f_tore),
		       init_u, fin_u, init_v, fin_v, 0, NULL);
  }
  else{
    err[0] = WRONG_PARAM_USER;
    return NULL;
  }
}

void test_sing(Surf *s, P3D vect_init, P3D vect_init2, 
	       double h, Pt* init, int nbptr, 
	       char *path, char *name,
	       Param_maillage *param, int nb_dir, 
	       int bool_dual, int _free){
  P3D **res_pts = NULL;
  _QUADR **res_quadr = NULL;
  double theta_in1[SIZE_TAB];
  double theta_in2[SIZE_TAB];
  int i, j = 0;
  Maillage *m;
  P3D v_dir[2];
  P3D metr[2];
  P3D vect_tmp[2];
  double theta1, theta2;
  double psi1[SIZE_TAB];
  double psi2 = 0.1;
  char *_name2 = NULL;
  char name2[1000];
  //  double psi = 0.0;
  double _psi;

  if (bool_dual != 0){
    nb_dir = 6;
    //psi = PI/3.0;
  }

  for (i = 0; i<nbptr; i++)
    psi1[i] = 2 * PI / 3.0 + i*PI / ((double)3.0*nbptr);

  memset(theta_in1, 0, nbptr*sizeof(double));
  memset(theta_in2, 0, nbptr*sizeof(double));
  //aff_erreur(nbptr*nbptr, nbptr);

  if (s->type == 1)
    image_cvect(s->s_param->f, init->param, init->p);

  if (!get_base_surf_param2(s->s_param, init->param,
	  vect_tmp, metr)){
	  PRINT_ERR_LOG(param, "ERREUR CALCUL BASE INITIAL\n");
	  return;
  }
  if (estNulle(deter(metr, 2))){
    int ind = 0;
    P3D tmp;
    if (estNulle(metr[0][0]))
      ind = 1;
    VECT_CPY(metr[0], vect_init);
    VECT_CPY(metr[1], vect_init2);
    orthonormer(metr);
    VECT_CPY(tmp, vect_tmp[ind]);
    calc_nouv_base_param(metr, NULL, tmp, vect_tmp);
  }

  for (j = 0; j<nb_dir; j++){
    theta1 = 2.0*j*PI / ((double)nb_dir);
    theta2 = 2.0*(j + 1)*PI / ((double)nb_dir);
    for (i = 0; i<3; i++){
      v_dir[0][i] = cos(theta1)*vect_tmp[0][i]
	+ sin(theta1)*vect_tmp[1][i];
      v_dir[1][i] = cos(theta2)*vect_tmp[0][i]
	+ sin(theta2)*vect_tmp[1][i];
    }
    _psi = theta2 - theta1;
    if (bool_dual == 1 || bool_dual == 3){
      if (bool_dual == 3){
	double r0 = 0.1;
	double N = 30;
	double the0 = asin(PI*r0 / (N*h));
	int _nb = ((int)N / 2) - 1;
	for (i = 0; i<_nb; i++)
	  psi1[_nb - 1 - i] = PI - (the0 + ((double)2 * (i + 1) / N)*PI);
	nbptr = _nb;
	psi2 = 2 * PI - 2 * _psi - psi1[0];
      }
      m = calc_maillage_init_sing2(nbptr, init,
				   h, v_dir, psi1, psi2,
				   s, theta_in1,
				   theta_in2, param, 0);
    }
    else if (bool_dual == 2){
      /*   double eta = 0.9;
      double R1 = 0.8;
      double R2 = 0.3;
      ListeD *last, *last_pr, *last_pr2;
      int or = 0;
      int nbp = nbptr;
      Pt *plast, *plast_pr;
      Pt pts[SIZE_TAB];
      double H[SIZE_TAB];
      double phi[SIZE_TAB];
      P3D dir;
      int type_aj_diag = 3;
      int or_vir = 0;
      int bool_bezier = 1;
      Pt *pt1, *pt2;
      P3D del1, del2;
      double phi0;
      double the0 = 0.2;//2*psi-0.1;
      CourbeG *cg;

      init->param[0] = 0.0;
      init->param[1] = 0.0;

      m = calc_maillage_init_sing3(nbptr, init,
				   h, v_dir, eta, the0,
				   R1, R2, s, param, or,
				   bool_bezier);
      last = m->lignes->l[0];
      last_pr = NULL;
      last_pr2 = NULL;
      while (last != NULL){
	last_pr2 = last_pr;
	last_pr = last;
	last = liste_getVois(last, or);
	if (last != NULL)
	  last = liste_getVois(last, (or + 3) % 4);
      }
      plast = getvect(last_pr);
      plast_pr = getvect(last_pr2);
      VECT_DEL(dir, plast_pr->p, plast->p, i);
      pt1 = getvect(liste_getVois(last_pr2, or));
      pt2 = getvect(liste_getVois(last_pr2, or + 3));
      VECT_DEL(del1, plast_pr->p, pt1->p, i);
      VECT_DEL(del2, plast_pr->p, pt2->p, i);
      phi0 = fabs(PI - angle(del1, del2));

      for (i = 0; i<nbp; i++)
	phi[i] = _fmax(0.05, phi0 - 0.02*i);

      for (i = 0; i<nbptr; i++)
	H[i] = 2 * h*cos((PI - phi[i]) / 2.0);
      cg = create_courbeG(plast, dir, H, nbptr, theta_in1);

      nbp = calc_courbeG(cg, s, NULL,
			 param, pts);
      ajoute_diag_maill(m, s, pts, nbp, last_pr,
			or, or_vir, param,
			1, type_aj_diag, NULL, NULL);*/
      printf("sing3 desactivée!!!!!\n");
    }
    else if (bool_dual == 3){
      //      int ind_ligne = 0;
      double the0 = PI / 8;
      double the_in[NB_PTS_SING_MAX];
      //  int or = 3;
      //    int or_vir = 0;
      Courbe c1;//, c2;
      P3D dir = { 1.0, 1.0, 0.0 };
      double H[NB_PTS_SING_MAX];
      double psi[NB_PTS_SING_MAX];
      /*     double taille_trou1 = 0.5;
	     double taille_trou2 = 0.3;
	     double ang_init = 0.2;*/

      for (i = 0; i<NB_PTS_SING_MAX; i++){
	psi[i] = the0;

	the_in[i] = 0.0;
	H[i] = 2 * h*cos((PI - psi[i]) / 2.0);
      }
      init_courbe_geod(&c1, dir, init, H, nbptr, the_in);
      the_in[0] = 2.0*PI / 3.0;
      init_courbe_geod(&c1, dir, init, H, nbptr, the_in);
      /*     calc_maillage_diag(s, init, param,
	     &c1, &c2, dir, ang_init,
	     taille_trou1, taille_trou2,
	     nbptr, h, or, or_vir, 1);*/
    }
    else{
      m = calc_maillage_init_sing(nbptr, init,
				  h, v_dir, s, theta_in1,
				  theta_in2, param);
    }
	if (name != NULL){
		sprintf(name2, "%s%d.vtk", name, j);
		_name2 = name2;
	}
    if(m != NULL)
      maill_export_paraview(m, param, path, _name2, 0, NULL, 
			    NULL, res_pts, res_quadr, NULL, _free);
    else
      printf("maillage nul (erreur quelque part...)\n");
    if (bool_dual != 0)
      break;
    if(_free){
   //   free_maillage(m);
    }
  }
}

void test_contrainte(Surf *s, P3D vect_init,
		     P3D vect_init2, double h,
		     Pt* init, int nbptr, char *path, char *name,
		     Param_maillage *param, char *path_angles1,
		     char *path_angles2, int bool_courbe, 
		     P3D **res_pts, _QUADR **res_quadr, 
		     int *res_size, int _free){
  double theta_in1[SIZE_TAB];
  double theta_in2[SIZE_TAB];
  int i;
  Pt c1[SIZE_TAB], c2[SIZE_TAB];
  PPt _c1[SIZE_TAB], _c2[SIZE_TAB];
  Maillage *m;
  int s_courbe1, s_courbe2;
  int or = 3;
  int aj_etoi = 0;
  double H[SIZE_TAB];
  P3D base[2];
  char *_name2 = NULL;
  char name2[1000];

  int nb_the1 = 0;
  int nb_the2 = 0;
  CourbeG *g1, *g2;

  if (s->type == 1)
    image_cvect(s->s_param->f, init->param, init->p);

  if (vect_init2 != NULL){
    VECT_CPY(base[0], vect_init);
    VECT_CPY(base[1], vect_init2);
  }
  else{
    if (!calc_base(s, init, vect_init, base, 1)){
      PRINT_ERR_LOG(param, "ERREUR CALCUL BASE INITIALE\n");
      return;
    }
  }
  for (i = 0; i<nbptr; i++)
    H[i] = h;
  aff_erreur(nbptr*nbptr, nbptr);

  m = createMaillage(init, h, s, nbptr);

  if (aj_etoi)
    param->bool_sing = 1;
  if (bool_courbe == 0){
    if (path_angles1 != NULL)
      nb_the1 = read_liste_reel_fic(path_angles1, theta_in1);
    if (path_angles2 != NULL)
      nb_the2 = read_liste_reel_fic(path_angles2, theta_in2);
    if (nb_the1 == 0){
      memset(theta_in1, 0, nbptr*sizeof(double));
      nb_the1 = nbptr;
      theta_in1[0] = 0.0;
    }
    if (nb_the2 == 0){
      memset(theta_in2, 0, nbptr*sizeof(double));
      nb_the2 = nbptr;
      theta_in2[0] = PI / 2.0;
    }
    g1 = create_courbeG(init, base[0], H, nb_the1, theta_in1);
    g2 = create_courbeG(init, base[0], H, nb_the2, theta_in2);
    s_courbe1 = calc_courbeG(g1, s, NULL,
			     param, c1);
    s_courbe2 = calc_courbeG(g2, s, NULL,
			     param, c2);
  }
  else if (bool_courbe == 2){
    /*
     * on est dans le plan et on maille avec une
     * ellipse
     */
    double x = 4.0 - 2.0*sqrt(3.0);
    /*ros1*/
    /*  double r1 = 2.0*sqrt(3.0)-3.0;
	double r2 = 2.0*sqrt(3.0)-3.0;
	double a = x;
	double b = 0.0
	init_c = PI;*/

    /*ros2*/
    /*   double r1 = 2.0*sqrt(3.0)-3.0;
	 double r2 = 2.0*sqrt(3.0)-3.0;
	 double a = -x/2.0;
	 double b = x*sqrt(3.0)/2.0;
	 double init_c = -PI/3.0;*/

    /*ros3*/
    double r1 = 2.0*sqrt(3.0) - 3.0;
    double r2 = 2.0*sqrt(3.0) - 3.0;
    double a = -x / 2.0;
    double b = -x*sqrt(3.0) / 2.0;
    double init_c = PI / 3.0;

    Fonc_vect *fonc
      = create_fonction_ellipse(a, b, r1, r2);
    CourbeP *c = create_courbeP(fonc, 0.0, 2*PI, 1, init_c, 0);
    //map = mapping_stereo_inv;

    init->p[0] = r1; init->p[1] = 0.0; init->p[2] = 0.0;
    init->param[0] = 0.0; init->param[1] = 0.0;
    s_courbe1 = calc_courbeP_surf(nbptr, init, 0, c, NULL,
				  H, s->s_param,
				  param, c1, NULL, -1);

    s_courbe2 = calc_courbeP_surf(nbptr, init, 1, c, NULL,
				  H, s->s_param,
				  param, c2, NULL, -1);

  }
  else {
    s_courbe1 = calc_courbeP_surf(nbptr, init, 0,
				  NULL, NULL, H,
				  s->s_param,
				  param, c1, NULL, -1);
    s_courbe2 = calc_courbeP_surf(nbptr, init, 1,
				  NULL, NULL, H,
				  s->s_param,
				  param, c2, NULL, -1);
  }
  for(i = 0;i<s_courbe1;i++){
    _c1[i] = createPtN();
    ptcopy(_c1[i], c1+i);
  }
  for(i = 0;i<s_courbe2;i++){
    _c2[i] = createPtN();
    ptcopy(_c2[i], c2+i);
  }
  ajoute_courbe_maill(m, _c1, s_courbe1, 1,
		      m->lignes->l[0], or % 4,
		      param->tol_pas);
  ajoute_courbe_maill(m, _c2, s_courbe2, 1,
		      m->lignes->l[0], (or + 1) % 4,
		      param->tol_pas);

  //   maillage_che(m, param, s, 0, NULL);
  //  maillage_stop_bord(m);
  maillage_che(m, param, s, 0, NULL, 0);
  //  creer_triple_ros(m, or);
  /*
    maillage_che(m, param, s, 0, NULL);
    maillage_stop_bord(m);
    maillage_suppr_der_ligne(m);
  */
  if (name != NULL){
    sprintf(name2, "%s.vtk", name);
    _name2 = name2;
  }
  path = "";
  maill_export_paraview(m, param, path, _name2, 0, NULL, NULL, 
			res_pts, res_quadr, res_size, _free);
  if(_free){
    free_maillage(m);
  }
  /*calc_courbe_init(nbptr, init, theta_in2,
    h, s, param, 0, &c2);*/
  /* u_export_paraview(u, path, "out_cheb.vtk", dist, dbg_u,
     dbg_v1, dbg_v2, 0, 0, 1, 1, 0);
     u_export_paraview(u, path, "out_cheb_dual.vtk",
     dist, dbg_u,
     dbg_v1, dbg_v2, 0, 0, 0, 1, 0);*/
}

void test_primal_dual(Surf *s, P3D vect_init, 
		      double h, Pt* init, int type_courbe,
		      int nbptr, char *path, char *name,
		      Param_maillage *param, 
		      P3D **res_pts, _QUADR **res_quadr, 
		      int *res_size, int _free){
  int nbp1;
  int i;
  double H[SIZE_TAB];
  int or = 0;
  Pt c1[SIZE_TAB];
  PPt _c1[SIZE_TAB];
  int dir = 0;
  int type_maill = 2;
  Maillage *m;
  int or_vir = 0;
  double theta[SIZE_TAB];
  char name2[1000];
  char *_name2 = NULL;

  memset(theta, 0, nbptr*sizeof(double));

  for (i = 0; i < nbptr; i++)
    H[i] = h;
  
  m = createMaillage(init, h, s, nbptr);  
  PRINT_MESSAGE_LOG(param, "maillage à partir du point (%lf;%lf)\n", 
		    init->param[0], init->param[1]);
  if (type_courbe == 0){
    CourbeG *cg = create_courbeG(init, vect_init, H, nbptr, theta);
    nbp1 = calc_courbeG(cg, s, NULL, param, c1);
  }
  else if (type_courbe == 1){
    nbp1 = calc_courbeP_surf(nbptr, init, (dir+1)%2, NULL,
			     NULL, H, s->s_param,
			     param, c1, NULL, -1);
  }
  for(i = 0;i<nbp1;i++){
    _c1[i] = createPtN();
    ptcopy(_c1[i], c1+i);
  }
  ajoute_courbe_maill(m, _c1, nbp1, 1,
		      m->lignes->l[0], or,
		      param->tol_pas);
  ajoute_courbe_dual_maillage(m, s, NULL, 0.0, dir, m->lignes->l[0], 
			      or, or_vir, param, type_maill);
  if (name != NULL){
	  sprintf(name2, "%s.vtk", name);
	  _name2 = name2;
  }
  maill_export_paraview(m, param, path, _name2, 0, NULL, NULL, 
			res_pts, res_quadr, res_size, _free);
}

void test_diag(Surf *s, int dir, P3D vect_init,
	       P3D vect_init2, double h, double discr,
	       Pt* init, int nbptr, char *path, char *name,
	       Param_maillage *param, char *path_angles,
	       char *path_pas, int type_courbe, P3D **res_pts,
	       _QUADR **res_quadr, int *res_size,
	       int _free){
  double theta_in1[SIZE_TAB];
  int nbp1;
  int i;
  Pt c1[SIZE_TAB];
  PPt c1_tmp[SIZE_TAB];
  Maillage *m;
  int or = 3;
  double H[SIZE_TAB];
  P3D base[2];
  char name2[1000];
  char *_name2 = NULL;
  //  double x;
  int type_maill = 3;
  int or_vir = 0;
  Mapping map = NULL;
  void *param_map = NULL;
  int est_ferme = 1;
  int nb_pas = 0;

  PPPt _c1 = c1_tmp;

  est_ferme &= (nbptr > param->courbeFerme_nbMin);
  if (path_pas != NULL)
    nb_pas = read_liste_reel_fic(path_pas, H);
  if (nb_pas == 0){
    for (i = 0; i < nbptr; i++)
      H[i] = 2.0*discr * h;
  }
  else
    nbptr = nb_pas;

  if (s->type == 1)
    image_cvect(s->s_param->f, init->param, init->p);

  if (vect_init2 != NULL){
    VECT_CPY(base[0], vect_init);
    VECT_CPY(base[1], vect_init2);
  }
  else{
    if (!calc_base(s, init, vect_init, base, 1)){
      PRINT_MESSAGE_LOG(param, "ERREUR CALCUL BASE INITIALE\n");
      return;
    }
  }

  aff_erreur(nbptr*nbptr, nbptr);

  m = createMaillage(init, h, s, nbptr);

  PRINT_MESSAGE_LOG(param, "maillage à partir du point (%lf;%lf)\n", 
		    init->param[0], init->param[1]);
  if (type_courbe == 1){
    double tol = param->tol_fermeture_courbe;
    if(est_ferme){
      nbp1 = calc_courbeP_ferme(nbptr, init, dir, NULL,
				NULL, H, s->s_param,
				param, c1, NULL, tol);
      est_ferme &= (nbp1>0);
      nbp1 = abs(nbp1);
      if (est_ferme == 0){
	PRINT_WAR_LOG(param, "La courbe du dual n'a pas pu être fermé!!\n");
      }
    } else {
      nbp1 = calc_courbeP_surf(nbptr, init, dir, NULL,
			       NULL, H, s->s_param,
			       param, c1, NULL, tol);
    }
  }
  else if (type_courbe == 0){
    int nb_the = 0;
    CourbeG *cg;
    if (path_angles != NULL)
      nb_the = read_liste_reel_fic(path_angles, theta_in1);

    if (nb_the == 0){
      nb_the = nbptr;
      memset(theta_in1, 0, nbptr*sizeof(double));
      theta_in1[0] = 0.0; //PI/4.0;
    }
    cg = create_courbeG(init, base[0], H, nb_the, theta_in1);
    nbp1 = calc_courbeG(cg, s, NULL, param, c1);
  }
  else if (type_courbe == 2){
    /*
     * on est dans le plan et on maille avec une
     * ellipse dans la diagonale
     */
    double r1 = 0.11;
    double r2 = 0.11;

    double init_c = 0.0;
    Fonc_vect *fonc
      = create_fonction_ellipse(0.0, 0.0, r1, r2);
    CourbeP *c = create_courbeP(fonc, 0, 2*PI, 1, init_c, 0);
    //    char n_fic_ell[1000] = "ell.txt";
    //map = mapping_stereo_inv;

    init->p[0] = r1; init->p[1] = 0.0; init->p[2] = 0.0;
    init->param[0] = 0.0; init->param[1] = 0.0;
    if(est_ferme){
      nbp1 = calc_courbeP_ferme(nbptr, init, 0, c, NULL,
				H, s->s_param,
				param, c1, NULL, -1);
      est_ferme &= (nbp1>0);
      nbp1 = abs(nbp1);
      if (!est_ferme){
	PRINT_WAR_LOG(param, "La courbe du dual n'a pas pu être fermé\n");
      }
    } else {
      nbp1 = calc_courbeP_surf(nbptr, init, 0, c, NULL,
			       H, s->s_param,
			       param, c1, NULL, -1);
    }
   // export_curvature(s, c1, nbp1, n_fic_ell);

  }
  else if (type_courbe == 3){
    /*
     * on est dans le plan et on maille avec une
     * courbe de Bezier dans la diagonale
     */
    double init_c = 0.0;

    Fonc_vect *fonc;
    CourbeP *c;
    P3D pts[2];
    P3D der[2];
    double _h = 0.5;

    pts[0][0] = 0.5; pts[0][1] = 0.5;  pts[0][2] = 0.0;
    pts[1][0] = 1.5; pts[1][1] = 1.5;  pts[1][2] = 0.0;
    der[0][0] = 1.0; der[0][1] = 0.0;  der[0][2] = 0.0;
    der[1][0] = 1.0; der[1][1] = 0.0;  der[1][2] = 0.0;

    fonc = create_fonction_bezier(pts, der, _h);
    c = create_courbeP(fonc, 0, 1, 0, init_c, 0);
    if(est_ferme){
      nbp1 = calc_courbeP_ferme(nbptr, init, 0, c, NULL,
				H, s->s_param,
				param, c1, NULL, -1);
      est_ferme &= (nbp1>0);
      nbp1 = abs(nbp1);
      if (!est_ferme){
	PRINT_WAR_LOG(param, "La courbe du dual n'a pas pu être fermé\n");
      }
    } else {
      nbp1 = calc_courbeP_surf(nbptr, init, 0, c, NULL,
			       H, s->s_param,
			       param, c1, NULL, -1);
    }
  }
  else if (type_courbe == 4 || type_courbe == 5){
    /*
     * on est dans le plan et on maille avec un
     * carre dans la diagonale
     */
    double init_c = 0.0;
    double coeff1 = 0.5;
    double coeff2 = 0.0;
    Fonc_vect *fonc;
    CourbeP *c;
    //   char n_fic_ell[1000] = "carre.txt";
    if(type_courbe == 4){
      fonc = create_fonction_carre(coeff1, coeff2);
      c = create_courbeP(fonc, 0, 2*PI, 1, init_c, 0);
    } else {
      coeff2 = 5.;
      fonc = create_fonction_triangle(coeff1, coeff2);
      c = create_courbeP(fonc, 0, 2*PI, 1, init_c, 0);
    }

    //map = mapping_stereo_inv;
    type_maill = 2;
    init->p[0] = 1.0; init->p[1] = 0.0; init->p[2] = 0.0;
    init->param[0] = 0.0; init->param[1] = 0.0;
    if(est_ferme){
      nbp1 = calc_courbeP_ferme(nbptr, init, 0, c, NULL,
				H, s->s_param,
				param, c1, NULL, -1);
      est_ferme &= (nbp1>0);
      nbp1 = abs(nbp1);
      if (!est_ferme){
	PRINT_WAR_LOG(param, "La courbe du dual n'a pas pu etre ferme\n");
      }
    } else {
      nbp1 = calc_courbeP_surf(nbptr, init, 0, c, NULL,
			       H, s->s_param,
			       param, c1, NULL, -1);

    }
    //  export_curvature(s, c1, nbp1, n_fic_ell);
  }

  for(i = 0;i<nbp1;i++){
    c1_tmp[i] = createPtN();
    ptcopy(c1_tmp[i], c1+i);
  }
  if (est_ferme){
    PRINT_MESSAGE_LOG(param, "ajout courbe fermé maillage : %d\n", nbp1);
    ajoute_diag_ferme_maill(m, s, _c1, nbp1, m->lignes->l[0],
			    or, or_vir, param, 1, type_maill);
  }
  else{
    PRINT_MESSAGE_LOG(param, "ajout courbe maillage : %d\n", 
		      nbp1);
    ajoute_diag_maill(m, s, &_c1, 1, &nbp1, m->lignes->l[0],
		      or, or_vir, -1, param, 1, type_maill,
		      NULL, NULL, 0);
    PRINT_MESSAGE_LOG(param, "fin ajout courbe\n");
  }
  if (name != NULL){
    sprintf(name2, "%s.vtk", name);
    _name2 = name2;
  }
  path = "";
  maill_export_paraview(m, param, path, _name2, 0, map, param_map, 
			res_pts, res_quadr, res_size, _free);
  PRINT_MESSAGE_LOG(param, "fin exportation\n");
  if(_free){
    free_maillage(m);
  }
}

void test_sing_dual(Surf *s, int dir, P3D vect_init,
		    P3D vect_init2, double h, double discr1, 
		    double discr2,
		    Pt* init, int nbptr, char *path, char *name,
		    Param_maillage *param, char *path_angles,
		    char *path_pas, int type_courbe, P3D **res_pts,
		    _QUADR **res_quadr, int *res_size,
		    int _free){
  double theta_in1[SIZE_TAB];
  int i;
  static TabPT c[NB_BRANCHES_MAX];
  static TabPPT c_new[NB_BRANCHES_MAX];
  Maillage *m;
  int or = 2;
  double H1[SIZE_TAB];
  double H2[SIZE_TAB];
  P3D base[2];
  char name2[1000];
  char *_name2 = NULL;
  //  double x;
  //  int type_maill = 3;
  int or_vir = 1;
  Mapping map = NULL;
  void *param_map = NULL;
  int nbpts[NB_BRANCHES_MAX];
  int nb_courbe = 8;
  double *_h;

  PPt _c[NB_BRANCHES_MAX];
  PPPt _c_new[NB_BRANCHES_MAX];
  double _eps = 10.0;

  double eps = _eps*PI/180.0;

  for (i = 0; i < nbptr; i++){
    H1[i] = 2.0*discr1 * h;
    H2[i] = 2.0*discr2 * h;
  }
  for(i = 0;i<nb_courbe;i++){
    _c[i] = c[i];
    _c_new[i] = c_new[i];
  }
  if (s->type == 1)
    image_cvect(s->s_param->f, init->param, init->p);

  if (vect_init2 != NULL){
    VECT_CPY(base[0], vect_init);
    VECT_CPY(base[1], vect_init2);
  }
  else{
    if (!calc_base(s, init, vect_init, base, 1)){
      PRINT_MESSAGE_LOG(param, "ERREUR CALCUL BASE INITIALE\n");
      return;
    }
  }

  aff_erreur(nbptr*nbptr, nbptr);
  m = createMaillage(init, h, s, nbptr);

  PRINT_MESSAGE_LOG(param, "maillage à partir du point (%lf;%lf)\n", 
		    init->param[0], init->param[1]);
  if (type_courbe == 1){
    /* P3D base[2];
       P3D metr[2];*/
    double tol = param->tol_fermeture_courbe;
    //calc_base(s, init, dir, base, 1);
    // get_base_surf_param2(s->s_param, init->param, base, metr);
    nbpts[0] = calc_courbeP_surf(nbptr, init, (dir)%2, NULL,
			     NULL, H1, s->s_param,
			     param, c[0], NULL, tol);
    nbpts[1] = calc_courbeP_surf(nbptr, init, (dir+1)%2, NULL,
			     NULL, H2, s->s_param,
				 param, c[1], NULL, tol);
  }
  else if (type_courbe == 0){
    int nb_the = 0; 
    CourbeG *cg;
    if (nb_the == 0){
      nb_the = nbptr;
      memset(theta_in1, 0, nbptr*sizeof(double));
      theta_in1[0] = 0.0; //PI/4.0;
    }
    for(i = 0;i<nb_courbe;i++){
      if(i==0)
	_h = H1;
      else
	_h = H2;
      cg = create_courbeG(init, base[0], _h, nb_the, theta_in1);
      nbpts[i] = calc_courbeG(cg, s, NULL, param, c[i]);
      free_courbeG(cg); 
      //theta_in1[0] -= 2*PI/3.0;
      theta_in1[0] -= 2*PI/((double)(nb_courbe));/*+PI/((double)(nb_courbe))*/
    }
  }

  PRINT_MESSAGE_LOG(param, "ajout courbe maillage : %d\n", 
		    nbpts[0]);
  jonction_dual_pts(m, s, nb_courbe, _c, _c_new, 
		    nbpts, param, or, or_vir, eps);
  //  maillage_fermer_sing(m, m->lignes->l[0]);
  //  maillage_che(m, param, s, 0, NULL, 0);
  if (name != NULL){
	  sprintf(name2, "%s.vtk", name);
	  _name2 = name2;
  }
  _free = 0;
  path = "";
  maill_export_paraview(m, param, path, _name2, 0, map, param_map, 
			res_pts, res_quadr, res_size, _free);
  PRINT_MESSAGE_LOG(param, "fin exportation\n");
  if(_free){
    free_maillage(m);
  }
}

void discr_angle(int nb, double(*fonc)(double), double *res){
  int i;
  double somme = 0;
  for (i = 0; i<nb; i++){
    res[i] = fonc(((double)i) / ((double)nb));
    somme += res[i];
  }
  for (i = 0; i<nb; i++){
    res[i] *= 2 * PI / somme;
    //   printf("%lf\n", res[i]);
  }
}

/*__inline */  double discr_asin(double val){
  return 1 / sqrt(1 - val*val);
}
/*__inline */  double discr_x2(double val){
  return fabs(0.5 - val);
}
/*__inline */  double discr_x(double val){
  return 1;
}

/*__inline */  double r_ext_tore(double t, void *param){
  double r_int = ((double*)param)[0];
  return r_int / 2 + cos(t)*r_int / 6;
  //return 1;
}
/*__inline */  double r_ext_tore_p(double t, void *param){
  double r_int = ((double*)param)[0];
  return -sin(t)*r_int / 6;
  // return 0;
}

/*void creer_triple_ros(Maillage *m, int or){
  ListeD *tmp = m->lignes->l[0];
  ListeD *tmp_p = tmp;
  double r1 = 1.0;
  double r2 = 1.0;
  int nbp;

  double init_c = 0.0;
  Fonc_vect *fonc
  = create_fonction_ellipse(0.0, 0.0, r1, r2);
  CourbeP *c = create_courbeP(fonc, -0.2, 2.0*PI/3.0);

  init->p[0] = r1;init->p[1] = 0.0;init->p[2] = 0.0;
  init->param[0] = 0.0;init->param[1] = 0.0;
  nbp = calc_ligne_espace_param(nbptr, init, 0, c, init_c,
  H, s->s_param,
  param, c1, NULL);
  while(tmp != NULL){
  tmp_p = tmp;
  tmp = liste_getVois(tmp, or);
  }
  ajoute_diag_maill(m, s, c1, nbp, tmp_p,
  or, or_vir, 0.0001, param, 1, type_maill);
  }*/

void aff_erreur(int nbprl, int nbptr){
  if (nbptr >= SIZE_TAB)
    printf("ERREUR possible : le nombre de poutres est supérieur au nombre maximal autorise\n");
}

#include "parametre_maillage.h"

Param_maillage* create_param_maillage(double pas_max_new, 
				      char *path_log, double tol){
  Param_maillage *res = malloc(sizeof(Param_maillage));
  res->coeff1_new = 0.7;
  res->coeff2_new = 0.9;
  res->nbMax_new = 2000;
  res->tol_new = 0.00001;

  res->pourc_ajout = 0.0001;

  if (tol <= 0.)
    tol = 10.0;
  res->tol_angle_rad = PI*tol/180.;
  res->tol_angle = cos(PI*tol/180.);
  res->tol_angle2 = cos(PI*tol/180.);
    res->tol_rebr = pow(10,-3);
  res->tol_grad = pow(10,-5);
  res->bool_sing = 2;
  /* continuer apres l'appartition d'une singularite */

  res->bool_suppr_doublons = 1;
  /* si 1 : on supprime les doublons */
  /* si 2 : on supprime les doublons et
   *	 on les traite comme une singularitée */

  res->nb_fus = 3;
  res->gr_fus[0] = 10;
  res->gr_fus[1] = 2;
  res->gr_fus[2] = 1;

  res->tol_pas = pow(10,-4);
  if(pas_max_new > 0.0)
    res->pas_max_new = pas_max_new;
  else
    res->pas_max_new = 0.05;
  res->tol_metr = 0.05;

  res->iter_param_max = 100;
  res->nb_etape_new_courbeP = 10;
  res->tol_new_courbeP = 0.1;
  res->tol_del_new_courbeP = 0.1;

  res->tol_fermeture_courbe = pow(10,-3);

  res->courbeFerme_dist_max = 3.0;
  res->courbeFerme_nbMax = 100;
  res->courbeFerme_nbMin = 10;
  res->err = NO_ERR;
  res->dist_bord_max = 0.25;
  if (path_log != NULL)
    res->file_log = fopen(path_log, "w+");
  else
    res->file_log = NULL;

  res->discr_nbx = 100;
  res->discr_nby = 100;
  res->del_discr_nbx = 50;
  res->del_discr_nby = 50;
  return res;
}

void free_param_maillage(Param_maillage *param){
  if (param->file_log != NULL)
    fclose(param->file_log);
  free(param);
}

void print_err_log(Param_maillage *param, int line, char *fonction){
  if (param->file_log != NULL){
    char aff[1000];
    error_type2string(param->err, aff);
    fprintf(param->file_log, "------ Erreur fonction %s ligne %d ", fonction, line);
    fprintf(param->file_log, "  erreur : %s\n", aff);
  }
}

void print_warning_log(Param_maillage *param, int line, char *file){
  if (param->file_log != NULL){
    char aff[1000];
    error_type2string(param->err, aff);
    fprintf(param->file_log, "----- Warning fonction %s ligne %d ",
	    file, line);
  }
}

void flush_log(Param_maillage *param){
  if (param->file_log != NULL)
    fflush(param->file_log);
}

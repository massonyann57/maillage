#include <string.h>
#include <stdio.h>

#include "erreur_struct.h"

int error_type2int(Error_type err){
  switch(err) {
  case IO_ERROR: return 0;
  case SIZE_MAX_ERROR: return 1;
  case NULL_POINTEUR_ERROR: return 2;
  case WRONG_PARAM_USER: return 3;
  case PB_CONV: return 4;
  case HORS_BORNE: return 5;
  case NO_ERR: return -1;
  default: return -1;
  }
}

void error_type2string(Error_type err, char *res){
  switch(err) {
  case IO_ERROR: sprintf(res, "erreur ouverture/fermeture fichier");
  case SIZE_MAX_ERROR: sprintf(res, "erreur ouverture/fermeture fichier");
  case NULL_POINTEUR_ERROR: sprintf(res, "erreur pointeur nul");
  case WRONG_PARAM_USER: sprintf(res, "erreur entrée utilisateur");
  case PB_CONV: sprintf(res, "problème de convergence dans le Newton");
  case NO_ERR: sprintf(res, "pas d'erreur??");
  case HORS_BORNE: sprintf(res, "sorti des bornes de la surface");
  default: sprintf(res, "pas d'erreur??");
  }
}

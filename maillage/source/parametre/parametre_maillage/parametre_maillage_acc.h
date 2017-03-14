#ifndef PARAMETRE_MAILLAGE_ACC_H

#define PARAMETRE_MAILLAGE_ACC_H

#include "parametre_maillage_struct.h"

Param_maillage* create_param_maillage(double pas_max_new, 
				      char *path_log, double tol);
void print_err_log(Param_maillage *param, int line, const char *file);
void print_warning_log(Param_maillage *param, int line, const char *file);
void free_param_maillage(Param_maillage *param);
void flush_log(Param_maillage *param);


#endif

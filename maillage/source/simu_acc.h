#define DIM 3

double simu_grille(int size, double *init[DIM],
				   double *fin[DIM], double ***pos,
				   double ***der_x, double ***der_y,
				   double h, char *path_inter, char *dir_exp,
				   int nb_discr, double ***res_pos, 
				   double ***res_der_x, double ***res_der_y,
				   double ***dbg_pts, double ***dbg_v1,
				   double ***dbg_v2, int *_size, int bool_init,
				   int nb_cond_dir, int **ind_cond_dir,
				   double **pos_cond_dir);

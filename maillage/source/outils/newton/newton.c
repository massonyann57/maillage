#include "newton.h"

#ifdef _DBG_NEW
int nb_tot = 0;
#endif

/*
 * Calcul l'inverse de la matrice si possible
 * retourne 0 sinon
 */
int invmat(double A[3][3], double *b, double *res, 
	   int dim, double tol){
  int i,j;
  int ind[3];
  int ind2[3];
  int nb = 0;
  double v[3];
  double mat[3][3];
  double inv_mat[3][3];
  double vect[3];
  int it = 0, it2 = 0;
  int tmp;
  
  if(dim == 3){
    memset(ind, 0, 3*sizeof(int));
    memset(ind2, 0, 3*sizeof(int));
  
    for(i=0;i<3;i++){
      for(j = 0;j<3;j++){
	if(fabs(A[i][j]) > EPS)
	  ind[i] = 1;
      }
      if(ind[i] == 1)
	nb++;
      for(j = 0;j<3;j++){
	if(fabs(A[i][j]) > EPS)
	  ind2[j] = 1;
      }
    }
  
    for(i=0;i<3;i++){
      if(ind2[i] == 1){
	it = 0;
	for(j=0;j<3;j++){
	  if(ind[j]==1){
	    if(it2==0)
	      vect[it] = b[j];
		  
	    mat[it][it2] = A[j][i];
	    it++;
	  }
	}
	it2++;
      }
    }
    if(it==it2){
      if(inverse_tol(mat, it, inv_mat, tol)){
	for(i=0;i<it;i++){
	  v[i] = 0;
	  for(j=0;j<it;j++)
	    v[i] += inv_mat[i][j]*vect[j];
	}
	tmp = 0;
	for(i=0;i<3;i++){
	  if(ind2[i]==0) 
	    res[i] = 0;
	  else{
	    res[i] = v[tmp];
	    tmp++;
	  }
	}
	return 1;
      }
      else return 0;
    } else return 0;
  } else {
    if(!inverse_tol(A, 2, inv_mat, tol)){
      res[0] = 0.0;
      res[1] = 0.0;
      if(estPetit(A[0][0]) && estPetit(A[1][0])){
	res[0] = 0.0;
	res[1] = b[0]/A[0][1];
      } else if(estPetit(A[0][1]) && estPetit(A[1][1])){
	res[0] = b[0]/A[0][0];
	res[1] = 0.0;
      }
      else return 0;
      return 1;
  }
  for(i = 0;i<2;i++){
      res[i] = 0.0;
      for(j=0;j<2;j++)
	res[i] += inv_mat[i][j]*b[j];
    }
    return 1;
  }
}

/*
 * Applique la méthode de Newton pour résoudre un système 
 * non linéaire 3x3
 * init : initialisation de l'algorithme
 * f : l'application représentant le système à résoudre
 * f_p : la dérivée de cette application
 * depl : pointeur de fonction permettant de se déplacer à chaque 
 *        itération du Newton
 * param : les paramètres de cette fonction
 * delta : le pas maximal que l'on prendre
 * del2 : le pas lorsqu'on applique une simple méthode de 
 *  descente
 * res : le résultat 
 * p_new : les différents paramètre de la méthode de Newton
 * dim : la dimension (2 ou 3)
 * retourne 1 s'il a convergé, 
 *         -1 si on n'a pas convergé,
 *         -2 si on est sorti des bornes
 */
int newton(P3D init, Newton_f f, Newton_f_p f_p, void *param_f,
	   Mapping_new depl, void *param_depl, 
	   double delta, double del2, P3D res,
	   Param_maillage *p_new, int dim){
  int i;
  double coeff1 = p_new->coeff1_new;
  double coeff2 = p_new->coeff2_new;
  double err[3];
  double m[3][3];
  double grad[3];
  double dir[3];
  double nerr, nerrp;
  int nb = 0;
  int aze = 0;
  int nbMax = p_new->nbMax_new;
  double tol = p_new->tol_new;
  int bo_inv;
  double nor_dir;
  double tol_tmp;
  double puiss_tol = 0.0;
  int tol_nb_init = 6;
  int tol_nb = tol_nb_init;
  double delta_puiss_tol = 0.05;
  double prem_tol = 0.5;
  double tol_min = 3.*pow(10.,-6);
  double nerr_prec;
  int _nb = 20; 
  int cur_nb = 0;
  /* lorsqu'on a du mal à CV (près d'une singularité)
   * on accepte d'être seulement à tol_min
   * si l'erreur n'evolue quasiment plus depuis _nb
   */
  
  dir[2] = 0.0;
  memcpy(res, init, dim*sizeof(double));
  f(res, param_f, err);
  for(i = 0;i<dim;i++)
    err[i] = fabs(err[i]);
  nerr = _norme(err, dim);

  while(nerr > EPS_TOL_NEW && nb < nbMax){
    nerr_prec = nerr;
    nerrp = nerr;
    f_p(res, param_f, m, grad);
    if(dim == 1){
      if(estNulle(m[0][0]))
	break;
      else{
	res[0] -= err[0]/m[0][0];
	f(res, param_f, err);
	err[0] = fabs(err[0]);
	nerr = err[0];
      }	
    } else {
      bo_inv = invmat(m, err, dir, dim, tol);
      if(bo_inv){
	nor_dir = _norme(dir, dim);
	tol_tmp = pow(nerr,puiss_tol)*delta;
	if(nor_dir>tol_tmp){
	  for(i=0;i<dim;i++)
	    dir[i] *= tol_tmp/nor_dir;
	}
	if(depl != NULL){
	  if(!depl(res, dir, param_depl, -1))
	    return -2;
	}
	else{
	  for(i=0;i<dim;i++)
	    res[i] -= dir[i];
	}
	f(res, param_f, err);
	for(i = 0;i<dim;i++)
	  err[i] = fabs(err[i]);
	nerr = _norme(err, dim);
      } else {
	if(depl != NULL){
	  for(i=0;i<dim;i++)
	    dir[i] = del2*grad[i];
	  if(!depl(res, dir, param_depl, -1))
	    return -2;
	} else {
	  for(i=0;i<dim;i++)
	    res[i] -= del2*grad[i];
	}
	f(res, param_f, err);
	for(i = 0;i<dim;i++)
	  err[i] = fabs(err[i]);
	  
	nerr = _norme(err, dim);
	if(nerr>nerrp){
	  if(depl != NULL){
	    for(i=0;i<dim;i++)
	      dir[i] = del2*grad[i];
	    if(!depl(res, dir, param_depl, 1))
	      return -2;
	  } else {
	    for(i=0;i<dim;i++){
	      res[i]+=del2*grad[i];
	    }
	  }
	  del2 *= coeff1;
	  aze++;
	} else {
	  if(aze>0){
	    del2 /= coeff2;
	    aze--;
	  }
	}
      }
    }
    nb++;
    if(estNulle(nerr - nerr_prec))
      cur_nb++;
    else
      cur_nb = 0;
    if(cur_nb >= _nb){
      /* on n'a plus évolué depuis assez de temps */
      if(nerr < tol_min)
	break;
      else
	return -1;
    }

    if(nb>tol_nb){
      if(puiss_tol == 0.0)
	puiss_tol = prem_tol;
#ifdef _DBG_NEW
      printf("tol atteint\n");
#endif

      puiss_tol += delta_puiss_tol;
      tol_nb += tol_nb_init;
    }
  }
  for(i = 0;i<dim;i++){
    if(!isfinite(res[i]))
      return -1;
  }

#ifdef _DBG_NEW
  nb_tot += nb;
  printf("nb_tot : %d\n", nb_tot);
#endif

  if(nb>20){
    PRINT_WAR_LOG(p_new, "convergence longue : %d, %lf\n", nb, del2);
    if(nb==nbMax)
      return -1;
  }
  return 1;
}

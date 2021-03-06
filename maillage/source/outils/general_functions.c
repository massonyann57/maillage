#include "general_functions.h"

/*__inline */ int estNulle(double d){
  if(fabs(d)>EPS_TOL)
    return 0;
  else
    return 1;
}

/*__inline */ int estPetit(double d){
  if(fabs(d)>EPS_TOL_PETIT)
    return 0;
  else
    return 1;
}
/*__inline */ int estPetit2(double d){
  if(fabs(d)>EPS_TOL_PETIT)
    return 0;
  else
    return 1;
}

/*__inline */ int estPetit_tol(double d, double tol){
  if(fabs(d)>tol)
    return 0;
  else
    return 1;
}

/*__inline */ int signe(double d){
  if(d>EPS_TOL)
    return 1;
  else if(d<-EPS_TOL)
    return -1;
  return 1;
}

/*__inline */ int _max(int i, int j){
  if(i>j)
    return i;
  else
    return j;
}
/*__inline */ int _min(int i, int j){
  if(i<j)
    return i;
  else
    return j;
}

/*__inline */ double _fmax(double i, double j){
  if(i>j)
    return i;
  else
    return j;
}
/*__inline */ double _fmin(double i, double j){
  if(i<j)
    return i;
  else
    return j;
}
/*__inline */ double norme(P3D p){
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}
/*__inline */ double _norme(P3D p, int d){
  int i;
  double res = 0;
  for(i = 0;i<d;i++)
    res += p[i]*p[i];
  return sqrt(res);
}

/*__inline*/ double prod_scal(P3D p1, P3D p2){
  return p1[0]*p2[0]+p1[1]*p2[1]+p1[2]*p2[2];
}

/*__inline*/ double distance(P3D p1, P3D p2){
  P3D del;
  int i;
  VECT_DEL(del, p1, p2, i);
  return norme(del);
}

/*__inline*/ double deter(double mat[3][3], int size){
  if(size == 1)
    return mat[0][0];
  else if(size == 2)
    return mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];
  else{
    int i,j;
    double res = 0;
    double tmp1 = 1;
    double tmp2 = 1;
    for(i=0;i<3;i++){
      tmp1 = 1;
      tmp2 = 1;
      for(j = 0;j<3;j++){
	tmp1 *= mat[(j+i)%3][j];
	tmp2 *= mat[(j+i)%3][2-j];
      }
      res+= (tmp1-tmp2);
    }
    return res;
  }
}

/*__inline*/ void cofact(double m[3][3], double res[3][3]){
  int i,j;
  for(i=0;i<3;i++){
    for(j=0;j<3;j++){
      res[i][j] = m[(i+1)%3][(j+1)%3]*m[(i+2)%3][(j+2)%3]
	-m[(i+1)%3][(j+2)%3]*m[(i+2)%3][(j+1)%3];
    }
  }
}

/*__inline*/ int inverse_tol(double m[3][3], int size, 
			     double res[3][3], double tol){
  if(size == 1){
    if(estNulle(m[0][0]))
      return 0;
    res[0][0] = 1/m[0][0];
    return 1;
  } else {
    double det = deter(m, size);
    double tmp[3][3];
    if(estPetit_tol(det, tol))
      return 0;
    if(size==2){
      res[0][0] = m[1][1]/det;
      res[1][1] = m[0][0]/det;
      res[1][0] = -m[1][0]/det;
      res[0][1] = -m[0][1]/det;
    } else {
      int i, j;
      cofact(m, tmp);
      for(i=0;i<3;i++){
	for(j=0;j<3;j++){
	  res[i][j] = tmp[j][i]/det;
	}
      }
    }
    return 1;
  }
}

/*__inline*/ int inverse(double m[3][3], int size, 
			 double res[3][3]){
  if(size == 1){
    if(estNulle(m[0][0]))
      return 0;
    res[0][0] = 1/m[0][0];
    return 1;
  } else {
    double det = deter(m, size);
    double tmp[3][3];
    if(estNulle(det))
      return 0;
    if(size==2){
      res[0][0] = m[1][1]/det;
      res[1][1] = m[0][0]/det;
      res[1][0] = -m[1][0]/det;
      res[0][1] = -m[0][1]/det;
    } else {
      int i, j;
      cofact(m, tmp);
      for(i=0;i<3;i++){
	for(j=0;j<3;j++){
	  res[i][j] = tmp[j][i]/det;
	}
      }
    }
    return 1;
  }
}
/*__inline*/ double func_sphere(P3D x, double R, P3D c){
  return R*R - pow(x[0] - c[0], 2) 
    - pow(x[1] - c[1], 2)
    - pow(x[2] - c[2], 2);
}

/*__inline*/ void func_sphere_p(P3D x, double R, P3D c, 
				double *res){
  int i;
  for(i = 0;i<3;i++)
    res[i] = -2*(x[i]-c[i]);
}

/*
 * Fonction définissant une direction. On regarde le produit 
 * scalaire avec l'orthogonal
 * base : une bon de l'espace tangent
 */
/*__inline*/ double fonc_angle(P3D x, P3D base[2], double the, 
			       P3D pos){
  P3D tmp, c;
  int i;
  double res;
  VECT_DEL(tmp, pos, x, i);
  coordonne_bon(tmp, base, c);
  res = -c[0]*sin(the) + c[1]*cos(the);
  return res;
}

/*__inline */ void fonc_angle_p(P3D x, P3D base[2], double the, 
				P3D pos, double *res){
  int i;
  for(i=0;i<3;i++){
    res[i] = (-sin(the)*base[0][i]
	      + cos(the)*base[1][i]);
  }
}	

/*__inline */ void prod_vect(P3D res, P3D x, P3D y){
  res[0] = x[1]*y[2]-x[2]*y[1];
  res[1] = x[2]*y[0]-x[0]*y[2];
  res[2] = x[0]*y[1]-x[1]*y[0];
}

/*__inline */ double f_boite_oeuf(double x, void *dummy){
  return AMPL_BOITE*cos(x);
}

/*__inline */ double f_boite_oeuf_p(double x, void *dummy){
  return -AMPL_BOITE*sin(x);
}
/*__inline */ double f1(double x){
  return pow(x*x-1,2);
}
/*__inline*/ double g1(double x){
  return -x*x;
}

void export_fonc_gnuplot3D(double **tab, double *X,
			   double *Y, int s1, int s2, char *path){
  FILE* f = fopen(path, "w+");
  int i, j;
  for(i = 0;i<s1;i++){
    for(j = 0;j<s1;j++){
      fprintf(f, "%lf %lf %lf\n", X[i], Y[j], tab[i][j]);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

void export_fonc_gnuplot2D(double *tab, double *X,
			   int s, char *path){
  FILE* f = fopen(path, "w+");
  int i;
  for(i = 0;i<s;i++){
    fprintf(f, "%lf %lf\n", X[i], tab[i]);
  }
  fclose(f);
}

/*
 * Affiche la différence entre 2 tableaux double 
 */
void dbg_delta(int a[SIZE_TAB][SIZE_TAB],
	       int b[SIZE_TAB][SIZE_TAB],
	       int s, int *s2){
  int i, j;
  for(i = 0;i<s;i++){
    for(j = 0;j<s2[i];j++){
      printf("%d\n", a[i][j] - b[i][j]);
    }
    printf("\n");
  }
}

/*
 * Exporte un ||gramme vers paraview
 */
void dbg_export(P3D p1, P3D p2, P3D p3, P3D p4, char *path){
  FILE *fvtk;
  char namestat[SIZE_TAB];
  
  sprintf(namestat, "%s", path);
  fvtk = fopen(namestat, "w+");
  fprintf(fvtk, "# vtk DataFile Version 2.0\n");
  fprintf(fvtk, "t = 0\n");
  fprintf(fvtk, "ASCII\nDATASET POLYDATA\n");
  fprintf(fvtk, "POINTS %d float\n", 4);
  
  fprintf(fvtk, "%lf %lf %lf\n", p1[0], p1[1], p1[2]);
  fprintf(fvtk, "%lf %lf %lf\n", p2[0], p2[1], p2[2]);
  fprintf(fvtk, "%lf %lf %lf\n", p3[0], p3[1], p3[2]);
  fprintf(fvtk, "%lf %lf %lf\n", p4[0], p4[1], p4[2]);
  fprintf(fvtk, "\n");
  fprintf(fvtk, "POLYGONS %d %d\n", 1, 6);
  fprintf(fvtk, "5 0 1 2 3 0\n");
  fprintf(fvtk, "\n");
  fclose(fvtk);
}

/*__inline */ void prod_mat(double m1[3][3], int nbl1, int nbc1, 
			    double m2[3][3], int nbc2,
			    double res[3][3]){
  int i, j, k;
  for(i = 0;i<nbl1;i++){
    for(j = 0;j<nbc2;j++){
      res[i][j] = 0;
      for(k = 0;k<nbc1;k++)
	res[i][j] += m1[i][k]*m2[k][j];
    }
  }
}

/*__inline */ void prod_mat_vect(double m[3][3], int nbl, int nbc,
				 double vect[3], double res[3]){
  int i, j;
  for(i = 0;i<nbl;i++){
    res[i] = 0.0;
    for(j = 0;j<nbc;j++){
      res[i] += m[i][j]*vect[j];
    }
  } 
}

/*__inline */ double trace(double m[3][3]){
  return m[0][0]+m[1][1]+m[2][2];
}

/*
 * Les solutions sont dans res
 * Retourne le nombre de solutions
 */
int solv_2nd_degre(double a, double b, double c, 
		   double res[2]){
  double del = b*b-4*a*c;
  if(estNulle(del)){
    res[0] = -b/(2*a);
    return 1;
  } else if(del>0){
    res[0] = (-b+sqrt(del))/(2*a);
    res[1] = (-b-sqrt(del))/(2*a);
    return 2;
  } else return 0;
}

/*
 */
/*__inline */ double intersection_segm_cercle(P3D p1, P3D p2, 
					      P3D c, double h){
  int i, nb_sol;
  double sol[2];
  P3D del;

  VECT_DEL(del, p1, p2, i);
  nb_sol = tmp_intersection_droite_cercle(p1, del, c, h, sol);
  for(i = 0;i<nb_sol;i++){
    if(sol[i]>0 && sol[i]<1){
      return sol[i];
    }
  }
  return -1;
}

/*
 */
/*__inline */ int intersection_droite_cercle(P3D p1, P3D vect, 
					     P3D c, double h, 
					     P3D res[2]){
  int i, j, nb_sol;
  double sol[2];

  nb_sol = tmp_intersection_droite_cercle(p1, vect, c, h, sol);
  for(i = 0;i<nb_sol;i++){
    for(j = 0;j<3;j++)
      res[i][j] = p1[j] + sol[i]*vect[j];
  }
  return nb_sol;
}

/*
 * Intersection entre une sphere et une droite 
 * (retourne la coordonnée sur la droite)
 */
/*__inline */ int tmp_intersection_droite_cercle(P3D pos, P3D vect, 
						 P3D c, double h, 
						 double sol[2]){
  int i, nb_sol;
  P3D tmp;
  VECT_DEL(tmp, c, pos, i);
  nb_sol = solv_2nd_degre(prod_scal(vect,vect), 
			  2*prod_scal(vect, tmp),
			  prod_scal(tmp,tmp)-h*h, sol);
  return nb_sol;
}


/*
 * Calcul une base locale (utilisé pour la construction 
 * des courbes initiales)
 * nor : la normale au point précédent
 * p1, p2 : les 2 points précédent de la courbe 
 * (=> le 1er vecteur de la base)
 */
void calc_nouv_base_contr(P3D nor, P3D v, P3D base[2]){
  int i;
  double a = prod_scal(v, nor);
  for(i = 0;i<3;i++)
    base[0][i] = v[i] - a*nor[i];
  normer(base[0]);
  prod_vect(base[1], base[0], nor);
  normer(base[1]);
}

void calc_base_dir(P3D grad[2], P3D metr[2], 
		   P3D v, P3D base[2]){
  int i, ind_min;
  double n = norme(v);
  double ps_min, ps;
  int ind = -1;
  P3D a;
  
  if(metr != NULL){
    if(estNulle(metr[0][0]))
      ind = 0;
    if(estNulle(metr[1][1]))
      ind = 1;
  }
  if(ind == -1){
    coordonne(v, grad, metr, a, EPS_TOL, 1, 0);
    for(i = 0;i<3;i++)
      base[0][i] = a[0]*grad[0][i]*sqrt(metr[0][0]) 
	+ a[1]*grad[1][i]*sqrt(metr[1][1]);
    normer(base[0]);
    
    if(fabs(a[0]) > fabs(a[1])){
      ps_min = a[1];
      ind_min = 1;
    } else {
      ps_min = a[0];
      ind_min = 0;
    }
    ps_min = prod_scal(base[0], grad[ind_min]);
    for(i = 0;i<3;i++)
      base[1][i] = grad[ind_min][i] - ps_min*base[0][i];
    normer(base[1]);
  } else {
    n = norme(v);
    for(i = 0;i<3;i++)
      base[0][i] = v[i]/n;
    ps = prod_scal(base[0], grad[(ind+1)%2]);
    for(i = 0;i<3;i++)
      base[1][i] = grad[(ind+1)%2][i] - ps*base[0][i];
    normer(base[1]);
  }
}

/*
 * Réalise le produit A^t*A d'une matrice A
 * m : la matrice
 * res : le résultat
 */
/*__inline */ void prod_mattransp_mat(double m[3][3], 
				      double res[3][3], 
				      int nbl, int nbc){
  int i, j, k;
  for(i = 0;i<nbl;i++){
    for(j = 0;j<nbl;j++){
      res[i][j] = 0.0;
      for(k = 0;k<nbc;k++)
	res[i][j] += m[k][i]*m[j][k];
    }
  }
}

/*
 * retourne l'othogonal d'un vecteur dans le plan tangent 
 * défini par la bon
 * vect : un vecteur du plan tangent
 * base : une bon du plan tangent
 * res : le retour implicite
 */
/*__inline */ void get_ortho(P3D vect, P3D base[2], P3D res){
  double ps1, ps2;
  int i;
  ps1 = prod_scal(vect, base[0]);
  ps2 = prod_scal(vect, base[1]);
  for(i = 0;i<3;i++)
    res[i] = ps1*base[1][i] - ps2*base[0][i];
}

/*__inline */ void coordonne_bon(P3D vect, P3D base[2], P3D res){
  res[0] = prod_scal(vect, base[0]);
  res[1] = prod_scal(vect, base[1]);
  res[2] = 0.0;
}

/*__inline */ int coordonne(P3D vect, P3D base[2], P3D metr[2], 
			    P3D res, double min_metr, 
			    int bool_norme, int bool_dege){
  P3D tmp;
  double det = deter(metr, 2);
  tmp[0] = prod_scal(vect, base[0]);
  tmp[1] = prod_scal(vect, base[1]);
  tmp[2] = 0.0;
  if(bool_norme){
    tmp[0] *= sqrt(metr[0][0]);
    tmp[1] *= sqrt(metr[1][1]);
  }
  if(bool_dege && estNulle(det)){
    VECT_CPY(res, tmp);
    return 0;
  } else {
    res[0] = (tmp[0]*metr[1][1] - metr[0][1]*tmp[1])/det;
    res[1] = (-tmp[0]*metr[0][1] + metr[0][0]*tmp[1])/det;
    res[2] = 0.0;
    return 1;
  }
}

/*__inline */ int normer(P3D vect){
  int i;
  double n = norme(vect);
  if(estNulle(n))
    return 0;
  for(i = 0;i<3;i++)
    vect[i] /= n;
  return 1;
}

/*__inline */ void orthonormer(P3D vect[2]){
  int i;
  double ps;
  normer(vect[0]);
  ps = prod_scal(vect[0], vect[1]);
  for(i = 0;i<3;i++)
    vect[1][i] -= vect[0][i]*ps;
  normer(vect[1]);
}

/*__inline */ void rotation_vect(P3D vect, P3D base[2], 
				 double the, P3D res){
  P3D coor, tmp;
  int i;
  coordonne_bon(vect, base, coor);
  tmp[0] = cos(the)*coor[0] - sin(the)*coor[1];
  tmp[1] = sin(the)*coor[0] + cos(the)*coor[1];
  for(i = 0;i<3;i++)
    res[i] = tmp[0]*base[0][i] + tmp[1]*base[1][i];
}

/*__inline */ void rotation_pt(P3D init, P3D pt, P3D base[2], 
			       double the, P3D res){
  P3D vect, tmp;
  int i;
  VECT_DEL(vect, init, pt, i);
  rotation_vect(vect, base, the, tmp);
  for(i = 0;i<3;i++)
    res[i] = init[i] + tmp[i];
}

/*__inline */ void ptcopy(Pt* p1, Pt *p2){
  VECT_CPY(p1->p, p2->p);
  VECT_CPY(p1->param, p2->param);
}

/*__inline */ void orienter_base(P3D base[2], int bool, 
				 P3D b_init[2]){
  int i;
  P3D N, N2;
  prod_vect(N, b_init[0], b_init[1]);
  prod_vect(N2, base[0], base[1]);
  if((2*bool-1)*prod_scal(N, N2) < 0){
    for(i = 0;i<3;i++)
      base[1][i] *= -1.0;
  }
}

Pt* createPtN(){
  return malloc(sizeof(Pt));
}

void delta_vect(P3D v1, P3D v2, P3D res){
  int i;
  for(i = 0;i<3;i++)
    res[i] = v2[i] - v1[i];
}

void export_courbe(Pt *pts, int nb, char *path){
  int i;
  FILE *f;

  f = fopen(path, "w+");
  for(i = 0;i<nb;i++){
    fprintf(f, "%lf %lf %lf\n", pts[i].p[0], 
	    pts[i].p[1], pts[i].p[2]);
  }
  fclose(f);
}

/*__inline */ double angle(P3D v1, P3D v2){
  return acos(prod_scal(v1, v2)/(norme(v1)*norme(v2)));
}

int read_liste_reel_fic(char *path, double *res){
  FILE* f;
  char l[1000];
  int i = 0;

  if(strcmp(path, "") != 0){
    f = fopen(path, "r");
    if(f != NULL){
      while(fgets(l, 1000, f) != NULL){
	sscanf(l, "%lf", res+i);
	i++;
      }
      return i;
    }
  } 
  return 0;
}

/*__inline */ int factorielle(int n){
  int res = 1;
  int i;
  for(i = 2;i<=n;i++)
    res *= i;
  return res;
}

/*__inline */ void set_ident(P3D mat[2]){
  mat[0][0] = 1.0; mat[0][0] = 0.0; mat[0][0] = 0.0;
  mat[1][0] = 0.0; mat[1][0] = 1.0; mat[1][0] = 0.0;
}

/*__inline */ void homothetie(P3D vect, double scal){
  int i;
  for(i = 0;i<3;i++)
    vect[i] *= scal;
}

void get_path_filelog(char *res, char *name){
#ifdef _COMPILE_WIN
  sprintf(res, "C:\\Users\\yannick.masson\\Desktop\\test\\%s", name);
#else
  sprintf(res, "tmp.log");
#endif
}

/*
 * Retourne l'indice du segment (pts[i] --- pts[i+1] --> indice i)
 *  (en positif si le point est à l'intérieur et en négatif 
 *   s'il est sur le segment)
 */
int appartient_quadr(PPt *pts, Pt *p, double borne[2][2]){
  /* P3D v1, v2, pv, pv_prec, p_proj;
     int i, j, res = 1;
     P3D del, dummy, base[2];
  
     // on projete p sur le plan
   
     VECT_DEL(base[0], pts[0]->p, pts[3]->p, i);
     VECT_DEL(base[1], pts[0]->p, pts[1]->p, i);
     VECT_DEL(del, pts[0]->p, p->p, i);
     projeter_vect(base, del, dummy);
     for(i = 0;i<3;i++)
     p_proj[i] = del[i]+pts[0]->p[i];

     memset(pv, 0, 3*sizeof(double));
     for(i = 0;i<4;i++){
     if(i == 0){
     VECT_DEL(v2, p_proj, pts[0]->p, j);
     if(!normer(v2))
     return -i-1;
     }
     VECT_CPY(v1, v2);
     VECT_DEL(v2, p_proj, pts[(i+1)%4]->p, j);
     if(!normer(v2))
     return -i-1;
     VECT_CPY(pv_prec, pv);
     prod_vect(pv, v1, v2);
     if(estNulle(norme(pv)))
     return -i-1;
     if(i != 0 && prod_scal(pv, pv_prec)<-EPS_TOL)
     res = 0;
     }
     return res;*/
  return est_dans_borne(p, borne);
}

int est_dans_borne(Pt *p, double borne[2][2]){
  return p->param[0] > borne[0][0]-EPS_TOL
    && p->param[0] < borne[0][1]+EPS_TOL
    && p->param[1] > borne[1][0]-EPS_TOL
    && p->param[1] < borne[1][1]+EPS_TOL;
}

int ajuste_borne(Pt *p1, Pt *p2, double borne[2][2]){
  if(p1->param[0] > borne[0][1]+EPS_TOL
     && p2->param[0] > borne[0][1]+EPS_TOL){
    return 0;
  } else if(p1->param[0] < borne[0][0]-EPS_TOL
	    && p2->param[0] < borne[0][0]-EPS_TOL){
    return 2;
  }else if(p1->param[1] > borne[1][1]+EPS_TOL
	   && p2->param[1] > borne[1][1]+EPS_TOL){
    return 3;
  }else if(p1->param[1] < borne[1][0]-EPS_TOL
	   && p2->param[1] < borne[1][0]-EPS_TOL){
    return 1;
  } else
    return -1;
}

/*
 * on s'est decalé dans la direction i. 
 * Retourne 0 si c'est une erreur 
 */
int ajuste_borne_dir(Pt *p1, Pt *p2, double borne[2][2], int i){
  if(i == 0){
    return p1->param[0] > borne[0][1]+EPS_TOL || p2->param[0] > borne[0][1]+EPS_TOL;
  } else if(i == 2){
    return p1->param[0] < borne[0][0]-EPS_TOL || p2->param[0] < borne[0][0]-EPS_TOL;
  } else if(i == 3){
    return p1->param[1] > borne[1][1]+EPS_TOL || p2->param[1] > borne[1][1]+EPS_TOL;
  } else if(i == 1){
    return p1->param[1] < borne[1][0]-EPS_TOL || p2->param[1] < borne[1][0]-EPS_TOL;
  } else
    return 1;
}

/*
 * Le segment est || avec un des bord du quadrangle
 * => on prend la direction se rapprochant le plus de p
 * retourne soit p soit p+2
 */
int dir_suiv_mauv_inter(Pt *p, double borne[2][2], int inter){
  int dir = inter%2;
  if(fabs(p->param[dir]-borne[dir][0]) 
     < fabs(p->param[dir]-borne[dir][1])){
    if(dir == 0)
      return 2;
    else 
      return 1;
  } else {
    if(dir==0)
      return 0;
    else
      return 3;
  }
}

/*
 * pos_min indique le point précédent dans les cas litigieux suivants :
 *    - cas où pt1 est près d'un segment
 *    - cas pourri où on est parallèle 
 */
int intersection_segm_quadr(PPt *pts, P3D pt1, P3D pt2, 
			    int *ind, double *prop, 
			    int *pos_prec, int *pos_min){
  P3D seg, v[2], coord_seg, coord_vect;
  int i, j,k;
  int nb_inter = 0;  
  P3D metr[3], vect, coord_pos, coord_el_seg;
  P3D mat[3], sm, el_seg, pos;
  P3D vect_del;
  P3D inv_mat[3], inter;
  int nbi = 4;
  double minnor, dist_min = 3.;
  double dist;
  int bool_dege = 0;
  int bool_norme = 0;
  double tol = 2.*pow(10.,-5);
  P3D pv;
  double /*n_seg, */ps, ang, tol_ang = 1.*pow(10.,-4);
  double n_coord_vect, n_coord_seg;
  double det_metr;
  //  double n_vect_del;

  for(k = 0;k<2;k++){
    VECT_DEL(v[0], pts[2*k]->p, pts[1]->p, j);
    VECT_DEL(v[1], pts[2*k]->p, pts[3]->p, j);
    metr[0][0] = prod_scal(v[0],v[0]);
    metr[1][1] = prod_scal(v[1],v[1]);
    metr[0][1] = prod_scal(v[1],v[0]);
    metr[1][0] = metr[0][1];

    /*  normer(v[0]);
	normer(v[1]);*/

    VECT_DEL(el_seg, pts[0]->p, pt1, j);
    coordonne(el_seg, v, metr, coord_el_seg, 
	      EPS_TOL,bool_norme, bool_dege);
    VECT_DEL(seg, pt1, pt2, j);
    coordonne(seg, v, metr, coord_seg, EPS_TOL,bool_norme,bool_dege);

    for(i = 0;i<nbi;i++){
      VECT_DEL(vect, pts[i]->p, pts[(i+1)%4]->p, j);
      VECT_DEL(pos, pts[0]->p, pts[i]->p, j);
      coordonne(vect, v, metr, coord_vect, EPS_TOL,
		bool_norme, bool_dege);
      coordonne(pos, v, metr, coord_pos, EPS_TOL,
		bool_norme, bool_dege);

      //  prod_vect(pv, vect, seg);

      VECT_DEL(sm, coord_el_seg, coord_pos, j);
      minnor = _fmin(norme(coord_seg), norme(coord_vect));
      for(j = 0;j<2;j++){
	mat[j][0] = coord_seg[j]/minnor;
	mat[j][1] = -coord_vect[j]/minnor;
	sm[j] /= minnor;
      }
      if(!estNulle(norme(sm))){
	n_coord_vect = norme_metr(coord_vect,metr);
	n_coord_seg = norme_metr(coord_seg,metr);
	ang = fabs(prod_scal_metr(coord_vect, coord_seg, metr))
	  /(n_coord_vect*n_coord_seg);
	if(fabs(1.-ang) > tol_ang && inverse(mat, 2, inv_mat)){
	  if(k==0 || nb_inter == 0 || ind[0] != i){
	    prod_mat_vect(inv_mat, 2, 2, sm, inter);
	    if(inter[0]>=-tol && inter[0] <=1.+tol
	       && inter[1]>=-tol && inter[1] <= 1.+tol){
	      dist = _fmin(fabs(inter[0]), fabs(1.-inter[0]));
	      if(inter[1]<=0.)
		inter[1] = 0.;
	      if(dist < dist_min){
		dist_min = dist;
		pos_min[0] = nb_inter;
	      }
	      prop[nb_inter] = inter[1];
	      ind[nb_inter] = i;
	      nb_inter++;
	      if(estNulle(inter[0]))
		pos_prec[0] = i;
	    }
	  }
	} else {
	  /* on est colinéaire à un segment : on regarde si 
	   * les 2 segments sont proche
	   */
	  //	n_seg = norme(seg);
	  //	VECT_DEL(vect_del, pts[i]->p, pt1, j);
	  //	prod_vect(pv, vect_del, seg);
	  VECT_DEL(vect_del, coord_pos, coord_el_seg, j);

	  prod_vect(pv, vect_del, coord_seg);
	  det_metr = deter(metr,2);
	  dist = det_metr*fabs(vect_del[0]*coord_seg[1]
			       - vect_del[1]*coord_seg[0])
	    /n_coord_seg;
	  if(dist < tol_ang*sqrt(det_metr)){
	    /* il y a une intersection
	     * et c'est le point du quadr le plus proche de pt2*/
	    ps = prod_scal_metr(vect_del, coord_vect, metr)
	      /prod_scal_metr(coord_vect,coord_vect, metr);
	    nb_inter = 0;
	    if(distance(pts[i]->p,pt2) < distance(pts[(i+1)%4]->p,pt2)){
	      ind[0] = (i+3)%4;
	      prop[0] = 1.;
	      nb_inter = 1;
	      if(ps>tol){
		if(i>ind[0]){
		  ind[1] = i;
		  prop[1] = _fmin(ps,1.0);
		  pos_min[0] = 1;
		} else {
		  ind[1] = ind[0];
		  ind[0] = i;
		  prop[1] = prop[0];
		  prop[0] = _fmin(ps,1.0);
		  pos_min[0] = 0;

		}
		nb_inter++;
	      }
	    } else {
	      if(ps<1.-tol){
		ind[0] = i;
		prop[0] = _fmax(0.,ps);
		nb_inter++;
	      }
	      if(nb_inter == 0){
		ind[0] = i;	
		prop[0] = 1.;
	      } else {
		if((i+1)%4>ind[0]){
		  ind[nb_inter] = (i+1)%4;	
		  prop[nb_inter] = 0.;
		  pos_min[0] = 0;
		} else {
		  ind[1] = ind[0];
		  ind[0] = (i+1)%4;
		  prop[1] = prop[0];
		  prop[0] = 0.;
		  pos_min[0] = 1;
		}
		nb_inter++;
	      }
	    }
	    break;
	  }
	}
      } else if(nb_inter == 0 || ind[nb_inter-1] != (i+3)%4){
	prop[nb_inter] = 1.0;
	ind[nb_inter] = (i+3)%4;
	nb_inter++;
	pos_prec[0] = i;
	if(i==0)
	  nbi--;
	else
	  i++;
      }
    }
    if(nb_inter == 2)
      return 2;
  }
  return nb_inter;
}

void projeter_vect(P3D v[2], P3D vect, P3D coor){
  P3D metr[2]; 
  int i,j;
  metr[0][0] = prod_scal(v[0],v[0]);
  metr[1][1] = prod_scal(v[1],v[1]);
  metr[0][1] = prod_scal(v[1],v[0]);
  metr[1][0] = metr[0][1];
  coordonne(vect, v, metr, coor, EPS_TOL,0,0);
  memset(vect, 0, 3*sizeof(double));
  for(i = 0;i<2;i++){
    for(j = 0;j<3;j++){
      vect[j] += coor[i]*v[i][j];
    }
  }
}

double prod_scal_metr(P3D v1, P3D v2, P3D metr[3]){
  return v1[0]*v2[0]*metr[0][0] + v1[1]*v2[1]*metr[1][1]
    + metr[0][1]*(v1[0]*v2[1]+v1[1]*v2[0]);
}

double norme_metr(P3D v, P3D metr[3]){
  return sqrt(prod_scal_metr(v,v,metr));
}

void exp_pt_liste(void *el, void *param){
  Pt* pt = (Pt*)el;
  printf("%lf;%lf\n", pt->param[0], pt->param[1]);
}

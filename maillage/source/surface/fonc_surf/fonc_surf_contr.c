#include "fonc_surf_contr.h"

double surf_sphere(P3D x, void *dummy){
  return 1 - pow(x[0], 2) - pow(x[1], 2)/TMP_BOULE
    - pow(x[2], 2)/TMP_BOULE2;
}
void surf_sphere_p(P3D x, void *dummy, double *res){
  int i;
  for(i = 0;i<3;i++)
    res[i] = -2*x[i];
  res[1]/=TMP_BOULE;
  res[2]/=TMP_BOULE2;
}

void surf_sphere_p2(double* x, void *dummy,
			   double* res){
  /*  int i, j;
  for(i = 0;i<3;i++){
    for(j = 0;j<3;j++)
      res[i][j] = 0;
    res[i][i] = -2;
    if(i==1)
      res[i][i]/=TMP_BOULE;
    if(i==2)
      res[i][i]/=TMP_BOULE2;
	  
      }*/
}

double surf_hyperb(P3D x, void *dummy){
  return 1 - pow(x[0], 2) - pow(x[1], 2)
    + pow(x[2], 2);
}
void surf_hyperb_p(P3D x, void *dummy, double *res){
  res[0] = -2*x[0];
  res[1] = -2*x[1];
  res[2] = 2*x[2];
}
void surf_hyperb_p2(double *x, void *dummy,
			   double *res)
{}


double surf_goursat(P3D x, void *dummy){
  return pow(x[2],4)+pow(x[1],4)+pow(x[0],4)
    -(pow(x[2],2)+pow(x[1],2)+pow(x[0],2));
}
void surf_goursat_p(P3D x, void *dummy, double *res){
  int i;
  for(i = 0;i<3;i++)
    res[i] = 4*pow(x[i],3)-2*x[i];
}

void surf_goursat_p2(double *x, void *dummy,
			    double *res){
  /* int i, j;
  for(i = 0;i<3;i++){
    for(j = 0;j<3;j++)
      res[i][j] = 0;
    res[i][i] = 12*pow(x[i],2)-2;
    }*/
}
  

double surf_tore(P3D x, void *dummy){
  return pow(pow(x[0], 2) + pow(x[1], 2)
	     + pow(x[2],2)+pow(TORE_R1, 2)-pow(TORE_R2, 2), 2)
    - 4 *TORE_R1*TORE_R1*(pow(x[0], 2) + pow(x[1], 2));
}

void surf_tore_p(P3D x, void *dummy, double *res){
  double d1,d2,tmp;
  d1 = x[0]*x[0] + x[1]*x[1];
  d2 = x[2]*x[2] + d1;
  tmp = d2+TORE_R1*TORE_R1-TORE_R2*TORE_R2;
  res[0] = (4*tmp*x[0]-8*TORE_R1*TORE_R1*x[0]);
  res[1] = (4*tmp*x[1]-8*TORE_R1*TORE_R1*x[1]);
  res[2] = (4*tmp*x[2]);
}
void surf_tore_p2(double* X, void *dummy, double *res){}


double surf_pot(P3D x, void *dummy){
  return x[2]-1.0/(pow(x[1],2)+pow(x[0],2));
}

void surf_pot_p(P3D x, void *dummy, double *res){
  double tmp = pow(pow(x[1],2)+pow(x[0],2),2);
  res[0] = 2*x[0]/tmp;
  res[1] = 2*x[1]/tmp;
  res[2] = 1;
}
void surf_pot_p2(double *x, void *dummy,
			double *res){
}

  
double surf_tore2T(P3D x, void *dummy){
  return pow(pow(pow(x[0], 2) + pow(x[1], 2),2)
	     - pow(x[0],2)+pow(x[1], 2),2)
    +pow(x[2], 2)-0.01;
}

void surf_tore2T_p(P3D x, void *dummy, double *res){
  double r, d;
  r = pow(x[0], 2) + pow(x[1], 2);
  d = pow(r,2) - pow(x[0],2) + pow(x[1], 2);
  res[0] = 4*x[0]*(r-1)*d;
  res[1] = 4*x[1]*(r+1)*d;
  res[2] = (2*x[2]);
}

void surf_tore2T_p2(double *X, void *dummy, double *res)
{}


double boule_enfoncee(P3D x, void *dummy){
  double coeff1 = COEFF_BOULE_ENF1;
  double coeff2 = COEFF_BOULE_ENF2;
  return 1-coeff1 - pow(x[0], 2) - pow(x[1], 2)
    - pow(x[2], 2) + coeff1*
    tanh(coeff2*(x[0]*x[0]+x[1]*x[1])-3);
}

void boule_enfoncee_p(P3D x, void *dummy, double *res){
  double coeff1 = COEFF_BOULE_ENF1;
  double coeff2 = COEFF_BOULE_ENF2;
  int i;
  double val = coeff2*(x[0]*x[0]+x[1]*x[1])-3;
  for(i = 0;i<3;i++){
    res[i] = -2*x[i];
    if(i!=2){
      res[i] += 2*coeff1*coeff2*x[i]/pow(cosh(val),2);
    }
  }
}

void boule_enfoncee_p2(double* x, void *dummy,
			      double *res){
  /*  double coeff1 = COEFF_BOULE_ENF1;
  double coeff2 = COEFF_BOULE_ENF2;
  int i, j;
  double val = coeff2*(x[0]*x[0]+x[1]*x[1])-3;
  double val2 = cosh(val);
  double val3 = sinh(val);
  
  for(i = 0;i<3;i++){
    res[i][2] = 0.0;
    res[2][i] = 0.0;
  }
  res[2][2] = -2;
  for(i = 0;i<2;i++){
    for(j = 0;j<2;i++){
      res[i][j] = -2*coeff1*pow(coeff2,2)*x[i]*x[j]*val3/pow(val2,3);
      if(i==j){
	res[i][j] += -2+2*coeff1*coeff2/pow(cosh(val),2);
      }
    }
    }*/
}


double conoide(P3D x, void *dummy){
  return (pow(x[0], 2) + pow(x[1], 2)
	  -pow(COEFF_CONOIDE, 2))*x[2]
    - COEFF_CONOIDE*(pow(x[0], 2)-pow(x[1], 2));
}

void conoide_p(P3D x, void *dummy, double *res){
  res[0] = 2*(x[0]*x[2]-COEFF_CONOIDE*x[0]);
  res[1] = 2*(x[1]*x[2]+COEFF_CONOIDE*x[1]);
  res[2] = (x[0]*x[0]+x[1]*x[1]-pow(COEFF_CONOIDE, 2));
}

void conoide_p2(double* x, void *dummy, double *res){}


double plan(P3D x, void *dummy){
  return x[2];
}

void plan_p(P3D x, void *dummy, double *res){
  res[0] = 0;
  res[1] = 0;
  res[2] = 1;
}
void plan_p2(P3D x, void *dummy, double *res){
  /*  int i,j;
  for(i = 0;i<3;i++){
    for(j = 0;j<3;j++){
      res[i][j] = 0;
    }
    }*/
}

Champ_r* create_champ_sphere(P3D r){
  double *pr = malloc(3*sizeof(double));
  memcpy(pr, r, 3*sizeof(double));
  return create_champ_r(surf_sphere, surf_sphere_p, 
			pr);
}
Champ_r* create_champ_tore(double r1, double r2){
  double *pr = malloc(3*sizeof(double));
  pr[0] = r1;pr[1] = r2;
  return create_champ_r(surf_tore, surf_tore_p, 
			pr);
}
Champ_r* create_champ_goursat(){
  return create_champ_r(surf_goursat, surf_goursat_p, 
			NULL);
}
Champ_r* create_champ_hyperb(){
  return create_champ_r(surf_hyperb, surf_hyperb_p, 
			NULL);
}
Champ_r* create_champ_tore2T(){
  return create_champ_r(surf_tore2T, surf_tore2T_p, 
			NULL);
}
Champ_r* create_champ_boule_enf(double r1, double r2){
  double *pr = malloc(3*sizeof(double));
  pr[0] = r1;pr[1] = r2;
  return create_champ_r(boule_enfoncee, boule_enfoncee_p, 
			pr);
}
Champ_r* create_champ_conoide(double c){
  double *pr = malloc(sizeof(double));
  pr[0] = c;
  return create_champ_r(conoide, conoide_p, pr);
}
Champ_r* create_champ_plan(){
  return create_champ_r(plan, plan_p, NULL);
}

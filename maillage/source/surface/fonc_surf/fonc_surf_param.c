#include "fonc_surf_param.h"

Param_tore* create_param_tore(double r_int, Fonc_r *r_ext){
  Param_tore *res = malloc(sizeof(Param_tore));
  res->r_int = r_int;
  res->r_ext = r_ext;
  return res;
}

void surf_catenoid_param(P3D x, void *par, P3D res){
  res[0] = cosh(x[0])*cos(x[1]);
  res[1] = cosh(x[0])*sin(x[1]);
  res[2] = x[0];
}

void surf_catenoid_param_p(P3D x, void *par, P3D* res){
  res[0][0] = sinh(x[0])*cos(x[1]);
  res[1][0] = sinh(x[0])*sin(x[1]);
  res[2][0] = 1.0;  

  res[0][1] = -cosh(x[0])*sin(x[1]);
  res[1][1] = cosh(x[0])*cos(x[1]);
  res[2][1] = 0.0;
}

void surf_boite_oeuf_param(P3D x, void *par, P3D res){
  res[0] = x[0];
  res[1] = x[1];
  res[2] = sin(x[0]) + sin(x[1]);
}

void surf_boite_oeuf_param_p(P3D x, void *par, P3D* res){
  res[0][0] = 1.;
  res[1][0] = 0.;
  res[2][0] = cos(x[0]);  

  res[0][1] = 0.;
  res[1][1] = 1.;
  res[2][1] = cos(x[1]);
}

void surf_catenoid_param_p2(P3D x, void *par, P3D* res){
  res[0][0] = cosh(x[0])*cos(x[1]);
  res[1][0] = cosh(x[0])*sin(x[1]);
  res[2][0] = 0.0;  

  res[0][1] = -cosh(x[0])*cos(x[1]);
  res[1][1] = -cosh(x[0])*sin(x[1]);
  res[2][1] = 0.0; 

  res[0][2] = -sinh(x[0])*sin(x[1]);
  res[1][2] = sinh(x[0])*cos(x[1]);
  res[2][2] = 0.0;
}
void surf_catenoid_param_p3(P3D x, void *par, P3D* res){
  res[0][0] = -cosh(x[0])*sin(x[1]);
  res[1][0] = cosh(x[0])*cos(x[1]);
  res[2][0] = 0.0;  

  res[0][1] = -sinh(x[0])*cos(x[1]);
  res[1][1] = -sinh(x[0])*sin(x[1]);
  res[2][1] = 0.0; 
}

void surf_cube_param(P3D x, void *par, P3D res){
  double *coeff = (double*)par;

  double tmp = 1.0 - 0.5*pow(sin(2*x[0]), 2) 
    - 0.5*pow(cos(x[0]), 4)*pow(sin(2*x[1]), 2);
  double r = pow(coeff[0]*tmp, -1.0/(4.0-coeff[1]));
  res[0] = r*cos(x[0])*cos(x[1]);
  res[1] = r*cos(x[0])*sin(x[1]);
  res[2] = r*sin(x[0]);
}

void surf_cube_param_p(P3D x, void *par, P3D* res){
  double *coeff = (double*)par;
  double a = cos(2.*x[0]);
  double b = sin(2.*x[0]);
  double c = cos(x[0]);
  double d = sin(x[0]);
  double e = cos(x[1]);
  double f = sin(x[1]);
  double g = cos(2.*x[1]);
  double tmp = 1.0 - 0.5*pow(b, 2) - 0.5*pow(c, 4)*pow(g, 2);
  double alpha = -1.0/(4.0-coeff[1]);
  double r = pow(tmp, alpha);
  double tmp2 = r/tmp;
  double dr1 = 2*a*b - 2*pow(c, 3)*d*pow(g, 2);
  double dr2 = 2*pow(c, 4)*g*sin(2*x[1]);
  dr1 *= -tmp2*alpha;
  dr2 *= -tmp2*alpha;
  res[0][0] = dr1*c*e - r*d*e;
  res[0][1] = dr2*c*e - r*f;
  res[1][0] = dr1*c*f - r*d*f;
  res[1][1] = dr2*c*f + r*c*e;
  res[2][0] = dr1*d + r*c;
  res[2][1] = dr2*d;
}

void surf_catenoid2_param(P3D x, void *par, P3D res){
  if(x[0] > 0){
    surf_catenoid_param(x, NULL, res);
  } else {
    surf_sphere_param(x,par,res);
  }
}

void surf_catenoid2_param_p(P3D x, void *par, P3D* res){
  if(x[0] > 0){
    surf_catenoid_param_p(x, NULL, res);
  } else {
    surf_sphere_param_p(x,par,res);
  }
}

void surf_catenoid2_param_p2(P3D x, void *par, P3D* res){
  if(x[0] > 0){
    surf_catenoid_param_p2(x, NULL, res);
  } else {
    surf_sphere_param_p2(x,par,res);
  }
}
void surf_catenoid2_param_p3(P3D x, void *par, P3D* res){
  if(x[0] > 0){
    surf_catenoid_param_p3(x, NULL, res);
  } else {
    surf_sphere_param_p3(x,par,res);
  }
}


void surf_enneper2_param(P3D x, void *par, P3D res){
  double *R = par;
  if(x[0] < PI/2.){
    double gam0, gam1; 
    if(x[0] < 0){
      gam0 = R[0]*cos(x[0]);
      gam1 = R[1] - R[0]*sin(x[0]);
    } else{
      gam0 = R[0]+R[1] - R[1]*cos(x[0]);
      gam1 = R[1] - R[1]*sin(x[0]);
    }
    res[0] = gam0*cos(x[1]);
    res[1] = gam0*sin(x[1]);
    res[2] = gam1;
  } else {
    int tmp = (int)R[2];
    P3D _x = {x[0]-PI/2.,x[1],0.0};
    surf_enneper_param(_x, &tmp, res);
    res[0] += (R[0]+R[1])*cos(x[1]);
    res[1] += (R[0]+R[1])*sin(x[1]);
  }
}

void surf_enneper2_param_p(P3D x, void *par, P3D* res){
  double *R = par;
  if(x[0] < PI/2.){
    double gam0; 
    double gamp0, gamp1; 
    if(x[0] < 0){
      gam0 = R[0]*cos(x[0]);
      gamp0 = -R[0]*sin(x[0]);
      gamp1= -R[0]*cos(x[0]);
    } else{
      gam0 = R[0]+R[1] -R[1]*cos(x[0]);
      gamp0 = R[1]*sin(x[0]);
      gamp1 = -R[1]*cos(x[0]);
    }
    res[0][0] = gamp0*cos(x[1]);
    res[1][0] = gamp0*sin(x[1]);
    res[2][0] = gamp1;
    res[0][1] = -gam0*sin(x[1]);
    res[1][1] = gam0*cos(x[1]);
    res[2][1] = 0.;
  } else {
    int tmp = (int)R[2];
    P3D _x = {x[0]-PI/2.,x[1],0.0};
    surf_enneper_param_p(_x, &tmp, res);
    res[0][1] += -(R[0]+R[1])*sin(x[1]);
    res[1][1] += (R[0]+R[1])*cos(x[1]);
  }
}

void surf_enneper2_param_p2(P3D x, void *par, P3D* res){ 
  double *R = par;
  if(x[0] < PI/2.){
    double gam0; 
    double gamp0; 
    double gampp0, gampp1; 
    if(x[0] < 0){
      gam0 = R[0]*cos(-x[0]);
      gamp0 = -R[0]*sin(x[0]);
      gampp0 = -R[0]*cos(x[0]);
      gampp1= R[0]*sin(x[0]);
    } else{
      gam0 = R[0]+R[1] -R[1]*cos(x[0]);
      gamp0 = R[1]*sin(x[0]);
      gampp0 = R[1]*cos(x[0]);
      gampp1=  R[1]*sin(x[0]);
    }
    res[0][0] = gampp0*cos(x[1]);
    res[1][0] = gampp0*sin(x[1]);
    res[2][0] = gampp1;
    res[0][1] = -gam0*cos(x[1]);
    res[1][1] = -gam0*sin(x[1]);
    res[2][1] = 0.;
    res[0][2] = -gamp0*sin(x[1]);
    res[1][2] = gamp0*cos(x[1]);
    res[2][2] = 0.;
  } else {
    int tmp = (int)R[2];
    P3D _x = {x[0]-PI/2.,x[1],0.0};
    surf_enneper_param_p2(_x, &tmp, res);
    res[0][1] += -(R[0]+R[1])*cos(x[1]);
    res[1][1] += -(R[0]+R[1])*sin(x[1]);
  }
}
void surf_enneper2_param_p3(P3D x, void *par, P3D* res){
  double *R = par;
  if(x[0] < PI/2.){
    double gamp0; 
    double gampp0; 
    if(x[0] < 0){
      gamp0 = -R[0]*sin(x[0]);
      gampp0 = -R[0]*cos(x[0]);
    } else{
      gamp0 = R[1]*sin(x[0]);
      gampp0 = R[1]*cos(x[0]);
    }
    res[0][0] = -gampp0*sin(x[1]);
    res[1][0] = gampp0*cos(x[1]);
    res[2][0] = 0.;
    res[0][1] = -gamp0*cos(x[1]);
    res[1][1] = -gamp0*sin(x[1]);
    res[2][1] = 0.;
  } else {
    int tmp = (int)R[2];
    P3D _x = {x[0]-PI/2.,x[1],0.0};
    surf_enneper_param_p3(_x, &tmp, res);
  }
}

void surf_sphere_param(P3D x, void *par, P3D res){
  double *p = (double*) par;
  res[0] = p[0]*cos(x[0])*cos(x[1]);
  res[1] = p[1]*cos(x[0])*sin(x[1]);
  res[2] = p[2]*sin(x[0]);
}

void surf_sphere_param_p(P3D x, void *par, P3D *res){
  double *p = (double*) par;
  res[0][0] = -p[0]*sin(x[0])*cos(x[1]);
  res[1][0] = -p[1]*sin(x[0])*sin(x[1]);
  res[2][0] = p[2]*cos(x[0]);

  res[0][1] = -p[0]*cos(x[0])*sin(x[1]);
  res[1][1] = p[1]*cos(x[0])*cos(x[1]);
  res[2][1] = 0;
}

void surf_sphere_param_p2(P3D x, void *par, P3D *res){
  double *p = (double*) par;
  res[0][0] = -p[0]*cos(x[0])*cos(x[1]);
  res[1][0] = -p[1]*cos(x[0])*sin(x[1]);
  res[2][0] = -p[2]*sin(x[0]);

  res[0][1] = -p[0]*cos(x[0])*cos(x[1]);
  res[1][1] = -p[1]*cos(x[0])*sin(x[1]);
  res[2][1] = 0.;

  res[0][2] = p[0]*sin(x[0])*sin(x[1]);
  res[1][2] = -p[1]*sin(x[0])*cos(x[1]);
  res[2][2] = 0.;
}

void surf_sphere_param_p3(P3D x, void *par, P3D *res){
  double *p = (double*) par;
  res[0][0] = p[0]*cos(x[0])*sin(x[1]);
  res[1][0] = -p[1]*cos(x[0])*cos(x[1]);
  res[2][0] = 0.;

  res[0][1] = p[0]*sin(x[0])*cos(x[1]);
  res[1][1] = p[1]*sin(x[0])*sin(x[1]);
  res[2][1] = 0.;
}

void surf_tmp_param(double* x, void *par, double* res){
  double* n = (double*) par;
  res[0] = x[0]*cos(x[1]);
  res[1] = x[0]*sin(x[1]);
   if(x[0] < 1)
    res[2] = n[1]*exp(-1.0/(1.0-x[0]*x[0]))*cos(n[0]*x[1]);
   else
    res[2] = 0.0;
}

void surf_tmp_param_p(double* x, void *par, P3D *res){
  double* n = (double*) par;
  double d = 1.0-x[0]*x[0];

  res[0][0] = cos(x[1]);
  res[1][0] = sin(x[1]);

  res[0][1] = -x[0]*sin(x[1]);
  res[1][1] = x[0]*cos(x[1]);  

  if(x[0] < 1){
    res[2][0] = -2.0*n[1]*x[0]*exp(-1.0/d)
      *cos(n[0]*x[1])/pow(d,2);
    res[2][1] = -n[0]*n[1]*exp(-1.0/d)*sin(n[0]*x[1]);
  }
  else{
    res[2][0] = 0.0;res[2][1] = 0.0;
  }
}

void surf_tore_param(double* x, void *par, double* res){
  Param_tore *_p = (Param_tore*) par;
  double bu = image_fr(_p->r_ext, x[0]);
  double tmp = (_p->r_int + bu*cos(x[1]));
  res[0] = tmp*cos(x[0]);
  res[1] = tmp*sin(x[0]);
  res[2] = bu*sin(x[1]);
}

void surf_tore_param_p(double* x, void *par, P3D *res){
  Param_tore *_p = (Param_tore*) par;
  double bu = image_fr(_p->r_ext, x[0]);
  double bpu = image_fr_p(_p->r_ext, x[0]);
  double tmp = (_p->r_int + bu*cos(x[1]));
  res[0][0] = bpu*cos(x[0])*cos(x[1])-tmp*sin(x[0]);
  res[1][0] = bpu*sin(x[0])*cos(x[1])+tmp*cos(x[0]);
  res[2][0] = bpu*sin(x[1]);

  res[0][1] = -bu*cos(x[0])*sin(x[1]);
  res[1][1] = -bu*sin(x[0])*sin(x[1]);
  res[2][1] = bu*cos(x[1]);
}

void plan_param(P3D x, void *par, P3D res){
  res[0] = x[0];
  res[1] = x[1];
  res[2] = 0.0;
}

void plan_param_p(P3D x, void *par, P3D *res){
  res[0][0] = 1.0;res[0][1] = 0.0;
  res[1][0] = 0.0;res[1][1] = 1.0;
  res[2][0] = 0.0;res[2][1] = 0.0;
}
void plan_param_p2(P3D x, void *par, P3D *res){
  res[0][0] = 0.0;res[0][1] = 0.0;
  res[1][0] = 0.0;res[1][1] = 0.0;
  res[2][0] = 0.0;res[2][1] = 0.0;
  res[0][2] = 0.0;res[1][2] = 0.0;
  res[2][2] = 0.0;
}
void plan_param_p3(P3D x, void *par, P3D *res){
  res[0][0] = 0.0;res[0][1] = 0.0;
  res[1][0] = 0.0;res[1][1] = 0.0;
  res[2][0] = 0.0;res[2][1] = 0.0;
}

void plan_param2(P3D x, void *par, P3D res){
  res[0] = x[0]*cos(x[1]);
  res[1] = x[0]*sin(x[1]);
  res[2] = 0.0;
}

void plan_param2_p(P3D x, void *par, P3D *res){
  res[0][0] = cos(x[1]);res[0][1] = -x[0]*sin(x[1]);
  res[1][0] = sin(x[1]);res[1][1] = x[0]*cos(x[1]);
  res[2][0] = 0.0;res[2][1] = 0.0;
}

#ifndef _WINDOWS
void surf_trinoid_param(P3D x, void *par, P3D res){
	double complex z = x[0] + I*x[1];
	double complex tmp = (1./9.)*(3.*z/(cpow(z,2)+z+1)
			     +2.*clog(1+z+cpow(z,2))-4.*clog(z-1));
  res[0] = creal(tmp);
  tmp = -z*(z+1)/(3*(cpow(z,3)-1))
    + (4.*sqrt(3)/9.)*catan((2*z+1)/sqrt(3));
  res[1] = cimag(tmp);
  res[2] = creal(2./(3-3*cpow(z,3))-1./3);
}

void surf_trinoid_param_p(P3D x, void *par, P3D *res){
  double complex z = x[0] + I*x[1];
  double complex tmp = (1-cpow(z,4))/(cpow(cpow(z,3)-1,2));
  res[0][0] = creal(tmp);
  res[0][1] = -cimag(tmp);
  tmp = (1+cpow(z,4))/(cpow(cpow(z,3)-1,2));
  res[1][0] = cimag(tmp);
  res[1][1] = creal(tmp);
  tmp = cpow(z,2)/(cpow(cpow(z,3)-1,2));
  res[2][0] = 2*creal(tmp);
  res[2][1] = -2*cimag(tmp);
}
#endif

void surf_enneper_param(P3D x, void *par, P3D res){
  int n = ((int*)par)[0];
  int n2 = 2*n-1;
  res[0] = x[0]*cos(x[1]) - pow(x[0], n2)*cos(n2*x[1])/n2;
  res[1] = x[0]*sin(x[1]) + pow(x[0], n2)*sin(n2*x[1])/n2;
  res[2] = 2*pow(x[0], n)*cos(n*x[1])/n;
}


void surf_enneper_param_p(P3D x, void *par, P3D *res){
  int n = ((int*)par)[0];
  int n2 = 2*n-1;
  res[0][0] = cos(x[1]) - pow(x[0], n2-1)*cos(n2*x[1]);
  res[1][0] = sin(x[1]) + pow(x[0], n2-1)*sin(n2*x[1]);
  res[2][0] = 2*pow(x[0], n-1)*cos(n*x[1]);

  res[0][1] = -x[0]*sin(x[1]) + pow(x[0], n2)*sin(n2*x[1]);
  res[1][1] = x[0]*cos(x[1]) + pow(x[0], n2)*cos(n2*x[1]);
  res[2][1] = -2*pow(x[0], n)*sin(n*x[1]);
}

void surf_enneper_param_p2(P3D x, void *par, P3D *res){
  int n = ((int*)par)[0];
  int n2 = 2*n-1;
  res[0][0] = - pow(x[0], n2-2)*(n2-1)*cos(n2*x[1]);
  res[1][0] = pow(x[0], n2-2)*(n2-1)*sin(n2*x[1]);
  res[2][0] = 2*(n-1)*pow(x[0], n-2)*cos(n*x[1]);

  res[0][1] = -x[0]*cos(x[1]) + n2*pow(x[0], n2)*cos(n2*x[1]);
  res[1][1] = -x[0]*sin(x[1]) - n2*pow(x[0], n2)*sin(n2*x[1]);
  res[2][1] = -2*n*pow(x[0], n)*cos(n*x[1]);

  res[0][2] = -sin(x[1]) + n2*pow(x[0], n2-1)*sin(n2*x[1]);
  res[1][2] = cos(x[1]) + n2*pow(x[0], n2-1)*cos(n2*x[1]);
  res[2][2] = -2*n*pow(x[0], n-1)*sin(n*x[1]);
}

void surf_enneper_param_p3(P3D x, void *par, P3D *res){
  int n = ((int*)par)[0];
  int n2 = 2*n-1;
  res[0][0] = (n2-1)*n2*pow(x[0],n2-2)*sin(n2*x[1]);
  res[1][0] = n2*(n2-1)*pow(x[0],n2-2)*cos(n2*x[1]);
  res[2][0] = -2*(n-1)*n*pow(x[0], n-2)*sin(n*x[1]);

  res[0][1] = -cos(x[1]) + n2*n2*pow(x[0], n2-1)*cos(n2*x[1]);
  res[1][1] = -sin(x[1]) - n2*n2*pow(x[0], n2-1)*sin(n2*x[1]);
  res[2][1] = -2*n*n*pow(x[0], n-1)*cos(n*x[1]);
}

Champ_vect* create_champ_plan_param(){
  return create_champ_vect(plan_param, plan_param_p,
			   plan_param_p2, plan_param_p3, NULL);
}
Champ_vect* create_champ_plan_param2(){
  return create_champ_vect(plan_param2, plan_param2_p, 
			   NULL, NULL, NULL);
}

Champ_vect* create_champ_sphere_param(P3D r){
  double *pr = malloc(3*sizeof(double));
  memcpy(pr, r, 3*sizeof(double));
  return create_champ_vect(surf_sphere_param, 
			   surf_sphere_param_p, surf_sphere_param_p2,
			   surf_sphere_param_p3, pr);
}
#ifndef _WINDOWS
Champ_vect* create_champ_trinoid_param(){
  return create_champ_vect(surf_trinoid_param, 
			   surf_trinoid_param_p, NULL, NULL, NULL);
}
#endif

Champ_vect* create_champ_enneper_param(int n){
  int *pn = malloc(sizeof(int));
  pn[0] = n;
  return create_champ_vect(surf_enneper_param, 
			   surf_enneper_param_p, 
			   surf_enneper_param_p2, 
			   surf_enneper_param_p3, pn);
}
Champ_vect* create_champ_enneper2_param(double R1, double R2, int n){
  double *pn = malloc(3*sizeof(double));
  pn[0] = R1;
  pn[1] = R2;
  pn[2] = (double)n;
  return create_champ_vect(surf_enneper2_param, 
			   surf_enneper2_param_p, 
			   surf_enneper2_param_p2, 
			   surf_enneper2_param_p3, pn);
}

Champ_vect* create_champ_tmp_param(int n, double d){
  double *pn = malloc(2*sizeof(double));
  pn[0] = n;
  pn[1] = d;
  return create_champ_vect(surf_tmp_param, 
			   surf_tmp_param_p, 
			   NULL, NULL, pn);
}

Champ_vect* create_champ_catenoid_param(){
  return create_champ_vect(surf_catenoid_param, 
			   surf_catenoid_param_p, 
			   surf_catenoid_param_p2, 
			   surf_catenoid_param_p3, NULL);
}

Champ_vect* create_champ_catenoid2_param(){
  double *pr = malloc(3*sizeof(double));
  pr[0] = 1.0;  pr[1] = 1.0;  pr[2] = 1.0;
  return create_champ_vect(surf_catenoid2_param, 
			   surf_catenoid2_param_p, 
			   surf_catenoid2_param_p2, 
			   surf_catenoid2_param_p3, pr);
}
Champ_vect* create_champ_cube_param(double coeff1, 
				    double coeff2){
  double *pr = malloc(2*sizeof(double));
  pr[0] = coeff1;  pr[1] = coeff2;
  return create_champ_vect(surf_cube_param, 
			   surf_cube_param_p, 
			   NULL, NULL, pr);
}
Champ_vect* create_champ_tore_param(double a, Fonc_r *b){
  Param_tore *p = create_param_tore(a, b);
  return create_champ_vect(surf_tore_param, 
			   surf_tore_param_p, 
			   NULL, NULL, p);
}

Champ_vect* create_champ_boite_oeuf_param(){
    return create_champ_vect(surf_boite_oeuf_param, 
			     surf_boite_oeuf_param_p, 
			     NULL, NULL, NULL);
}

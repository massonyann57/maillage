#include "test_vecteurs.h"

void exp_pts_surf(Surf_param *s, char *path){
  int i, j;
  FILE *fvtk;
  char name[SIZE_TAB];
  P3D pt, par;
  int N = 50;
  static P3D vect[SIZE_TAB*100][3];
  double deltaI = (s->fin[0] - s->init[0])/N;
  double deltaJ = (s->fin[1] - s->init[1])/N;
  
  sprintf(name, "%stest.vtk", path);
  fvtk = fopen(name, "w+");
  fprintf(fvtk, "# vtk DataFile Version 2.0\n");
  fprintf(fvtk, "t = 0\n");
  fprintf(fvtk, "ASCII\nDATASET POLYDATA\n");
  fprintf(fvtk, "POINTS %d float\n", N*N);
  par[0] = s->init[0];
  for(i = 0;i<N;i++){
    par[1] = s->init[1];
    for(j = 0;j<N;j++){
      image_cvect(s->f, par, pt);
      image_cvect_p(s->f, par,vect[j+N*i]);
      
      // get_base_surf_param(s, par, vect[j+N*i], dummy);
      fprintf(fvtk, "%lf %lf %lf\n", 
	      pt[0], pt[1], pt[2]);
      par[1] += deltaJ;
    }
    par[0] += deltaI;
  }
  fprintf(fvtk, "\n");
  fprintf(fvtk, "POINT_DATA %d\n", N*N);
  fprintf(fvtk, "VECTORS tangent_x float\n");
  for(i = 0;i<N*N;i++){
      fprintf(fvtk, "%lf %lf %lf\n", 
	      vect[i][0][0], vect[i][1][0], vect[i][2][0]);
  }
  fprintf(fvtk, "\n");
  fprintf(fvtk, "VECTORS tangent_y float\n");
  for(i = 0;i<N*N;i++){
      fprintf(fvtk, "%lf %lf %lf\n", 
	      vect[i][0][1], vect[i][1][1], vect[i][2][1]);
  }
  fprintf(fvtk, "\n");
  fclose(fvtk);
}

void test_vect_cheb(Surf_param *s, char *path, double a){
  int i, j;
  FILE *fvtk;
  char name[SIZE_TAB];
  P3D pt, par;
  int N = 50;
  static P3D vect[SIZE_TAB*100][3];
  double deltaI = (s->fin[0] - s->init[0])/N;
  double deltaJ = (s->fin[1] - s->init[1])/N;
  
  sprintf(name, "%stest_cheb.vtk", path);
  fvtk = fopen(name, "w+");
  fprintf(fvtk, "# vtk DataFile Version 2.0\n");
  fprintf(fvtk, "t = 0\n");
  fprintf(fvtk, "ASCII\nDATASET POLYDATA\n");
  fprintf(fvtk, "POINTS %d float\n", N*N);
  par[0] = s->init[0];
  for(i = 0;i<N;i++){
    par[1] = s->init[1];
    for(j = 0;j<N;j++){
      image_cvect(s->f, par, pt);
      //test_get_vect(s, par, vect[j+N*i], a);
      test_get_vect2(s, pt, vect[j+N*i]);
      fprintf(fvtk, "%lf %lf %lf\n", 
	      pt[0], pt[1], pt[2]);
      par[1] += deltaJ;
    }
    par[0] += deltaI;
  }
  fprintf(fvtk, "\n");
  fprintf(fvtk, "POINT_DATA %d\n", N*N);
  fprintf(fvtk, "VECTORS tangent_x float\n");
  for(i = 0;i<N*N;i++){
      fprintf(fvtk, "%lf %lf %lf\n", 
	      vect[i][0][0], vect[i][0][1], vect[i][0][2]);
  }
  fprintf(fvtk, "\n");
  fprintf(fvtk, "VECTORS tangent_y float\n");
  for(i = 0;i<N*N;i++){
      fprintf(fvtk, "%lf %lf %lf\n", 
	      vect[i][1][0], vect[i][1][1], vect[i][1][2]);
  }
  fprintf(fvtk, "\n");
  fclose(fvtk);
}

void test_get_vect(Surf_param *s, P3D p, P3D res[3], double a){
  P3D der[3];
  int i;
  double k = sqrt(a*a-pow(cosh(p[0]), 2))/cosh(p[0]);
  image_cvect_p(s->f, p, der);
  for(i = 0;i<3;i++){
    res[0][i] = der[i][0]*k/a - der[i][1]/a;
    res[1][i] = der[i][0]*k/a + der[i][1]/a;
  }
}

void test_get_vect2(Surf_param *s, P3D p, P3D res[3]){
  // P3D der[3];
  int i;
  /*  P3D p1 = {-1.0,0.0,0.0};
  P3D p2 = {1.0,0.0,0.0};*/
  /*P3D p1 = {1.0/2.0,sqrt(3.0/2.0),0.0};
  P3D p2 = {-1.0/2.0,sqrt(3.0/2.0),0.0};
  P3D p3 = {1.0,0.0,0.0};*/
  P3D p1 = {-1.0,0.0,0.0};
  P3D p2 = {1.0,0.0,0.0};
  P3D p3 = {0.0,-1.0,0.0};
  P3D p4  = {0.0,1.0,0.0};
  double r1 = distance(p, p1);
  double r2 = distance(p, p2);
  double r3 = distance(p, p3);
  double r4 = distance(p, p4);
  P3D v1[2], v2[2], v3[2], v4[2];

  for(i = 0;i<3;i++){
    v1[0][i] = p[i] - p1[i];
    v2[0][i] = p[i] - p2[i];
    v3[0][i] = p[i] - p3[i];
    v4[0][i] = p[i] - p4[i];
  }
  v1[1][0] = v1[0][1];
  v1[1][1] = -v1[0][0];
  v2[1][0] = v2[0][1];
  v2[1][1] = -v2[0][0];
  v3[1][0] = v3[0][1];
  v3[1][1] = -v3[0][0];
  v4[1][0] = v4[0][1];
  v4[1][1] = -v4[0][0];
  for(i = 0;i<3;i++){
    res[0][i] = v1[0][i]/r1 + v2[0][i]/r2
      + v3[0][i]/r3 + v4[0][i]/r4;
    res[1][i] = v1[1][i]/r1 + v2[1][i]/r2
      + v3[1][i]/r3 + v4[1][i]/r4;
  }
}

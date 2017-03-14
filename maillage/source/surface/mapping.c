#include "mapping.h"

void mapping_stereo_inv(P3D pt, P3D res, void* param){
  double no2 = pt[0]*pt[0] + pt[1]*pt[1];
  res[0] = 2*pt[0]/(1+no2);
  res[1] = 2*pt[1]/(1+no2);
  res[2] = (1-no2)/(1+no2);
}

void mapping_homothetie(P3D pt, P3D res, void *param){
  double R[3] = {1.,2.,1.};
  int i;
  for(i = 0;i<3;i++)
    res[i] = pt[i]*R[i];
}

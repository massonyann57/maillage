#include "courbe_plan.h"

void create_ellipse(Pt *centre, P3D r, int n, int boo, Pt *res){
  int i;
  double del = PI/n;
  if(boo) 
    del *=-1.0;
  for(i = 0;i<n;i++){
    res[i].p[0] = centre->p[0] + r[0]*cos(i*del);
    res[i].p[1] = centre->p[1] + r[1]*sin(i*del);
  VECT_CPY(&res[i].param, &res[i].p);
  }
}

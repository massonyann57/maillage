#include "call_sing.h"

Param_call_sing* 
create_param_call_sing(ListeD *pointe, 
		       double eps, int dir, 
		       int dir_sing, int nb_dual, 
		       int *nbpts, PPt *pts_prec, 
		       PPPt *new_pts, Pint *ind_npts,
		       Param_maillage *param_m, 
		       Surf_param *s, double h){
  Param_call_sing *res = malloc(sizeof(Param_call_sing));
  P3D dummy[2];
  int i, j;
  double pas;
  P3D del1, del2;
  double theta;
  int maj_ind;
  Pt *cur = getvect(pointe);
  int _i;

  for(i = 0;i<3;i++){
    dummy[0][i] = 0.0;
    dummy[1][i] = 0.0;
  }

  res->dir = dir;
  res->eps = eps;
  res->dir_sing = dir_sing;
  res->param_m = param_m;

  for(i = 0;i<nb_dual;i++){
    res->nbpts[i] = nbpts[i];
    res->pointes[2*i] = pointe;
    res->pointes[2*i+1] = pointe;
    res->pts[i] = pts_prec[i];
    res->new_pts[i] = new_pts[i];
    res->ind_npts[i] = ind_npts[i];
  }
  
  res->param_new = create_param_new2(dummy[0], dummy, dummy[0], 
				     0.0, 0.0, s);
  for(i = 0;i<nb_dual;i++){
    _i = (i+1)%nb_dual;
    VECT_DEL(del1, cur->p, pts_prec[i][1].p, j);
    VECT_DEL(del2, cur->p, pts_prec[_i][1].p, j);
    res->psi[i] = angle(del1, del2);
  }
  for(i = 0;i<nb_dual;i++){
    theta = PI-res->psi[_max(0,i-1)]+eps;
    pas = 2.0*sin(theta/2.0)*h;
    res->ind_npts[i][0] = 0;
    new_pts[i][0] = createPtN();
    new_pts[i][1] = createPtN();
    ptcopy(new_pts[i][0], &pts_prec[i][0]);
    
    maj_ind = calc_nouv_pt_courbe(s, nbpts[i]-1, &pts_prec[i][0], 
				  NULL, &pts_prec[i][1], pas, pas,
				  res->param_new, new_pts[i][1],
				  param_m, 0);
    if(maj_ind == -10)
      return NULL;
    res->ind_npts[i][1] = maj_ind+1;
  }
  res->ind = 1;
  res->s = s;
  return res;
}

void free_param_call_sing(Param_call_sing *p){
  free(p);
}

void callback_sing(Maillage *m, int nb_dual, 
		   int *bool_term, void *param){
  Param_call_sing *p = (Param_call_sing*) param;
  Pt *pt1, *pt2, *pt_cur;
  ListeD *v1, *v2;
  double theta;
  P3D del1, del2;
  int i, j;
  double ndiscr, ntheta_i[2], new_the;
  int ind_cur, _ind = p->ind;
  int maj_ind;
  double thetai_prec;
  int dir_tmp;
  double _psi;
  int k;
  int ind1, ind2;

  for(i = 0;i<nb_dual;i++){
    if(!bool_term[i]){
      for(k = 0;k<2;k++){
	ind1 = i-1+k;
	if(ind1 == -1)
	  ind1 = nb_dual-1;
	_psi = p->psi[ind1];
	ind2 = 2*i+k;
	v1 = p->pointes[ind2];
	if(p->pointes[ind2]->vois != NULL)
	  dir_tmp = p->dir_sing;
	else 
	  dir_tmp = p->dir;
	dir_tmp += 2*i+k;

	p->pointes[ind2] = liste_getVois(p->pointes[ind2], dir_tmp);
	if(p->pointes[ind2] == NULL){
	  bool_term[i] = 1;
	} else {
	  if(k == 0)
	    dir_tmp = p->dir+1+2*i;
	  else
	    dir_tmp = p->dir+2*i;
	  v2 = liste_getVois(p->pointes[ind2], dir_tmp);
	  pt_cur = getvect(p->pointes[ind2]);
	  pt1 = getvect(v1);
	  pt2 = getvect(v2);
	  VECT_DEL(del1, pt_cur->p, pt1->p, j);
	  VECT_DEL(del2, pt_cur->p, pt2->p, j);
	  theta = acos(prod_scal(del1, del2)/(m->h*m->h));

	  /* on calcul la nouvelle metrique de la courbe */
	  VECT_DEL(del1, p->new_pts[i][_ind-1]->p, 
		   p->new_pts[i][_ind]->p, j);
	  thetai_prec = 2.0*asin(norme(del1)/(2.0*m->h));
	  ind_cur = p->ind_npts[i][_ind];
	  ntheta_i[k] = thetai_prec + PI - _psi - theta + p->eps;
	}
      }
      new_the = _fmax(ntheta_i[0], ntheta_i[1]);
      if(new_the<p->eps)
	new_the = p->eps;
      ndiscr = 2.0*sin(new_the/2.0)*m->h;

      /* on modifie maintenant les points de la courbe */
      p->new_pts[i][_ind+1] = createPtN();
      maj_ind = calc_nouv_pt_courbe(m->s, p->nbpts[i]-ind_cur-1, 
				    p->new_pts[i][_ind], NULL, 
				    &p->pts[i][ind_cur+1], ndiscr, 
				    ndiscr, p->param_new, 
				    p->new_pts[i][_ind+1],
				    p->param_m, 0);
      if(maj_ind >= -1)
	p->ind_npts[i][_ind+1] = ind_cur+1+maj_ind;
      else 
	bool_term[i] = 1;
      
    }
  }
  p->ind++;
}

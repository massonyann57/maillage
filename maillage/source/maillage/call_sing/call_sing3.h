#include "call_sing.h"

Param_call_sing* create_param_call_sing(ListeD *pointe, 
					double eps, int dir, 
					int dir_sing, int nb_dual, 
					int *nbpts, PPt *pts_prec, 
					PPt *new_pts, Pint *ind_npts,
					Param_maillage *param_m, 
					Surf *s, double h){
  Param_call_sing *res = malloc(sizeof(Param_call_sing));
  P3D dummy[2];
  int i, j;
  PPt pt_tmp[3];
  double pas;
  P3D del1, del2;
  double theta;
  int maj_ind;
  Pt *cur = getvect(pointe);
  int nb_tmp;

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
    res->pointes[i] = pointe;
    res->pts[i] = pts_prec[i];
    res->new_pts[i] = new_pts[i];
    res->ind_npts[i] = ind_npts[i];
  }
  
  res->param_new = create_param_new2(dummy[0], dummy, dummy[0], 
				     0.0, 0.0, s);
  for(i = 0;i<nb_dual-1;i++){
    VECT_DEL(del1, cur->p, pts_prec[i][1].p, j);
    VECT_DEL(del2, cur->p, pts_prec[i+1][1].p, j);
    res->psi[i] = angle(del1, del2);
  }
  for(i = 0;i<nb_dual;i++){
    theta = PI-res->psi[_max(0,i-1)]+eps;
    pas = 2.0*sin(theta/2.0)*h;
    res->ind_npts[i][0] = 0;
    ptcopy(&new_pts[i][0], &pts_prec[i][0]);
    nb_tmp = 3;
    if(nbpts[i]<3)
      nb_tmp = 2;
    for(j = 0;j<nb_tmp;j++)
      pt_tmp[j] = &pts_prec[i][j];
    maj_ind = calc_nouv_pt_courbe(s, nb_tmp, pt_tmp, pas, 
				  res->param_new, &new_pts[i][1],
				  param_m);
    if(maj_ind<0)
      return NULL;
    res->ind_npts[i][1] = maj_ind;
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
  double ndiscr, ntheta_i;
  int ind_cur, _ind = p->ind;
  int maj_ind;
  double thetai_prec;
  PPt pt_tmp[3];
  int dir_tmp, nb_tmp;
  double _psi;

  for(i = 0;i<nb_dual;i++){
    if(!bool_term[i]){
      _psi = p->psi[0];
      if(i>0)
	_psi = p->psi[i-1];

      v1 = p->pointes[i];
      if(p->pointes[i]->vois != NULL)
	dir_tmp = p->dir_sing;
       else 
	 dir_tmp = p->dir;
      if(i>0)
	dir_tmp+=1+2*_max(0, i-1);
      p->pointes[i] = liste_getVois(p->pointes[i], dir_tmp);
      if(p->pointes[i] == NULL){
	  bool_term[i] = 1;
      } else {
	if(i==0)
	  dir_tmp = p->dir+3;
	else
	  dir_tmp = p->dir+2+2*_max(0, i-1);
	  
	v2 = liste_getVois(p->pointes[i], dir_tmp);
	pt_cur = getvect(p->pointes[i]);
	pt1 = getvect(v1);
	pt2 = getvect(v2);
	VECT_DEL(del1, pt_cur->p, pt1->p, j);
	VECT_DEL(del2, pt_cur->p, pt2->p, j);
	theta = acos(prod_scal(del1, del2)/(m->h*m->h));

	/* on calcul la nouvelle metrique de la courbe */
	VECT_DEL(del1, p->new_pts[i][_ind-1].p, 
		 p->new_pts[i][_ind].p, j);
	thetai_prec = 2.0*asin(norme(del1)/(2.0*m->h));
	ind_cur = p->ind_npts[i][_ind];
	ntheta_i = thetai_prec + PI - _psi - theta + p->eps;
	if(ntheta_i<p->eps)
	  ntheta_i = p->eps;
	ndiscr = 2.0*sin(ntheta_i/2.0)*m->h;

	/* on modifie maintenant les points de la courbe */
	nb_tmp = 2;
	pt_tmp[0] = &p->new_pts[i][_ind];
	pt_tmp[1] = &p->pts[i][ind_cur+1];
	if(p->nbpts[i]>=ind_cur+3){
	  pt_tmp[2] = &p->pts[i][ind_cur+2];
	  nb_tmp++;
	}
	maj_ind = calc_nouv_pt_courbe(p->s, nb_tmp, pt_tmp, ndiscr, 
				      p->param_new, 
				      &p->new_pts[i][_ind+1],
				      p->param_m);
	if(maj_ind >= 0){
	  p->ind_npts[i][_ind+1] = ind_cur;
	  if(maj_ind)
	    p->ind_npts[i][_ind+1]++;   
	} else {
	  bool_term[i] = 1;
	}
      }
    } 
  }
  p->ind++;
}

int calc_nouv_pt_courbe(Surf *s, int nb, PPt *pts, double pas,  
			Param_new2 *param_new, Pt *res, 
			Param_maillage *param_m){
  P3D del;
  Pt* centre;
  int i;
  double dist;
  int ret = 0;
  VECT_DEL(del, pts[0]->p, pts[1]->p, i);
  dist = norme(del);
  if(pas > dist){
    if(nb<3) 
      return -1;
    pas -= dist;
    VECT_DEL(del, pts[1]->p, pts[2]->p, i);
    centre = pts[1];
    ret = 1;
  } 
  else
    centre = pts[0];
  nouv_pt_courbe(s, centre, del, pas, 
		 param_new, res, param_m);
  return ret;
}

void nouv_pt_courbe(Surf *s, Pt *pt1, P3D dir, double pas,  
		    Param_new2 *param_new, Pt *res, 
		    Param_maillage *param_m){
  P3D base[2];
  int nbMax = param_m->nbMax_new;
  calc_base(s, pt1, dir, base, 1);
  VECT_CPY(param_new->pos, pt1);
  VECT_CPY(param_new->centre, pt1);
  VECT_CPY(param_new->base[0], base[0]);
  VECT_CPY(param_new->base[1], base[1]);
  param_new->the = 0.0;
  param_new->R = pas;

  point_suivant_courbeG(pt1, NULL, param_new, nbMax, res, 
			param_m, NULL, NULL);
}

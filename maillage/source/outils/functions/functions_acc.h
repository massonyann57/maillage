#ifndef FUNCTIONS_ACC_H

#define FUNCTIONS_ACC_H

#include "functions_struct.h"

Champ_vect* 
create_champ_vect(void (*f)(P3D, void*, P3D),
		  void (*f_p)(P3D, void*, P3D*), 
		  void (*f_p2)(P3D, void*, P3D*), 
		  void (*f_p3)(P3D, void*, P3D*), 
		  void *param
);

Champ_r* create_champ_r(double (*f)(P3D, void*),
			void (*f_p)(P3D, void*, P3D), 
			void *param);

Fonc_vect* create_fonction_vect(void (*f)(double, void*, P3D),
				void (*f_p)(double, void*, P3D), 
				void *param);
Fonc_r* create_fonc_r(double (*f)(double, void*),
		      double (*f_p)(double, void*),
		      void *param);

double image_cr(Champ_r* f, P3D pt);
void image_cr_p(Champ_r* f, P3D pt, P3D res);

double image_fr(Fonc_r* f, double d);
double image_fr_p(Fonc_r* f, double d);

void image_cvect(Champ_vect* f, P3D pt, P3D res);
void image_cvect_p(Champ_vect* f, P3D pt, P3D *res);
void image_cvect_p2(Champ_vect* f, P3D pt, P3D *res);
void image_cvect_p3(Champ_vect* f, P3D pt, P3D *res);

void image_fvect(Fonc_vect* f, double d, P3D res);
void image_fvect_p(Fonc_vect* f, double d, P3D res);

void free_champ_vect(Champ_vect *c);
void free_fonc_vect(Fonc_vect *f);
void free_champ_r(Champ_r *c);

#endif

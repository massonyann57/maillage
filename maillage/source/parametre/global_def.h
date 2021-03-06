#ifndef GLOBAL_DEF_H

#define GLOBAL_DEF_H

#ifndef __func__
#define __func__ __FUNCTION__
#endif

#define NB_SAVE_MAX 3

#define NB_AJ_MAX 100000
#define NB_BRANCHES_MAX 30
#define NB_SING_MAX 10
#define NB_PART_MAX 3

#define SIZE_TAB 2000
#define NB_COUR_MAX 4
#define NB_PTS_BEZIER_MAX 10
#define NB_PTS_SING_MAX 40
#define NB_MULT_COURBE_MAX 40

#define EPS 1.E-18
#define EPS_TOL_NEW 1.E-7
#define EPS_TOL 1.E-6
#define EPS_TOL_PETIT 7.E-4
#define EPS_TOL_INTER 1.E-5
#define TOL_EPS_VECT_TAN 0.05
#define PREM_SING_ETOILE 2

#define TOL_EPS_TAN 0.03

#define PI 3.14159265359

#define AMPL_BOITE 1
#define TORE_R1 1
#define TORE_R2 0.3
#define COEFF_BOULE_ENF1 0.3
#define COEFF_BOULE_ENF2 12
#define N_ENNEPER 6
#define COEFF_CONOIDE 10
#define TMP_BOULE 1
#define TMP_BOULE2 1

#define NB_GTAB 15

#define SIZE_GTAB 4*SIZE_TAB
#define SIZE_SUPPR_GTAB 15*SIZE_TAB

#define VECT_CPY(R, V) (memcpy(R,V, 3*sizeof(double)))
#define VECT_NULL(X) (memset(X,0, 3*sizeof(double)))
#define VECT_DEL(R, X, Y, I) for(I = 0;I<3;I++){	\
    R[I] = Y[I] - X[I];}


typedef void* Pvoid;
typedef Pvoid* PPvoid;
typedef int* Pint;
typedef double P3D[3];
typedef double P2D[2];


typedef void (*Newton_f)(double*,void*,double*);
typedef void (*Newton_f_p)(double*,void*,P3D*,double*);
typedef void (*Mapping)(double*,double*,void*);
typedef int (*Mapping_new)(double*,double*,void*,int);
//typedef int _QUADR[10];

/*
 * Un point d'une surface (position + paramétrage)
 */
typedef struct PT{
  P3D p; /* la position */
  P3D param; /* le paramétrage */
}Pt;

typedef Pt* PPt;
typedef Pt** PPPt;

typedef Pt TabPT[SIZE_TAB];
typedef PPt TabPPT[SIZE_TAB];
typedef int TabInt[SIZE_TAB];
//#ifdef _DBG
#define PRINT_ERR_LOG(param, mess, ...)   if(param != NULL && param->file_log != NULL){  print_err_log(param, __LINE__, __func__); \
  fprintf(param->file_log, mess, ##__VA_ARGS__);			\
  flush_log(param); }							\
  else {printf("ERREUR : fonction %s  ligne %d\n", __func__, __LINE__); \
    printf(mess, ##__VA_ARGS__);}

#define PRINT_WAR_LOG(param, mess, ...)    if(param != NULL && param->file_log != NULL){  print_warning_log(param, __LINE__, __func__); \
    fprintf(param->file_log, mess, ##__VA_ARGS__);			\
    flush_log(param);}							\
  else {/*printf("fonction %s  ligne %d.   ", __func__, __LINE__); */	\
    printf(mess, ##__VA_ARGS__);}
/*#define PRINT_MESSAGE_LOG(param, mess, ...)    if(param->file_log != NULL){  fprintf(param->file_log, mess, ##__VA_ARGS__); \
									flush_log(param);}*/
/*#else 
#define PRINT_ERR_LOG(param, mess, ...) 
#define PRINT_WAR_LOG(param, mess, ...)   
#define PRINT_MESSAGE_LOG(param, mess, ...)   
#endif*/

#endif

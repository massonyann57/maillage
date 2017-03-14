#ifndef ERREUR_STRUCT

#define ERREUR_STRUCT

typedef enum ERROR_TYPE{
	IO_ERROR, 
	SIZE_MAX_ERROR, 
	NULL_POINTEUR_ERROR, 
	WRONG_PARAM_USER, 
	NO_ERR,
	PB_CONV,
	HORS_BORNE
}Error_type;

#endif

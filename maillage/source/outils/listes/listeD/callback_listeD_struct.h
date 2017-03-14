#ifndef CALLBACK_LISTED_STRUCT

#define CALLBACK_LISTED_STRUCT

/*
 * Structure utilisé pour le callback du parcours
 * utilisé si l'on souhaite supprimer des éléments 
 * de la liste (par exemple tous les éléments dépassant
 * une singularitée)
 */
typedef struct Param_isUsed{
  int (*is_used)(void*,void*); /* Callback disant si on 
				* doit supprimer un élément
				* ou non*/
  void *param_is_used; /* paramètre du callback is_used*/
  void (*callback_suppr)(void*,void*); 
  /* callback permettant la suppression d'un élément */
  void* param_suppr; /* paramètre du callback suppr*/
}Param_isUsed;

#endif

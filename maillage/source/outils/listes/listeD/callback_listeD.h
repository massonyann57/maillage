#include <stdlib.h>

#include "callback_listeD_struct.h"
#include "listeD_struct.h"
#include "listeD_acc.h"
#include "../../tab/tab_acc.h"
#include "../../../parametre/global_def.h"
#include "../../general_functions_acc.h"

Param_isUsed* create_param_isUsed(int (*is_used)(void*,void*), 
				  void* param, 
				  void (*callback_suppr)(void*,
							 void*), 
				  void *param_suppr);

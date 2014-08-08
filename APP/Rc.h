#ifndef RC_H
#define RC_H

#include "sysconfig.h"

#define RC_MIN			900
#define RC_MAX			2100
#define RC_FUN_MIN	1200
#define RC_FUN_MAX	1800
#define	RC_FUN_CNT	100


void RC_Analyse(T_RC_Data *rc_data,T_Control *ctl_data);

#endif



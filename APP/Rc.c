#include "rc.h"

void RC_Analyse(T_RC_Data *rc_data,T_Control *ctl_data)
{
	static u16 fun_cnt=0;
	if(rc_data->THROTTLE<RC_FUN_MIN && rc_data->THROTTLE>RC_MIN && rc_data->YAW<RC_FUN_MIN  && rc_data->YAW>RC_MIN)
	{
		if(fun_cnt<RC_FUN_CNT)
			fun_cnt++;
		else
		{
			fun_cnt = 0;
			ctl_data->ARMED = 1;	//½âËø
//			LED_STA = 1;
		}
	}
	else if(rc_data->THROTTLE<RC_FUN_MIN && rc_data->THROTTLE>RC_MIN && rc_data->YAW>RC_FUN_MAX && rc_data->YAW<RC_MAX)
	{
		if(fun_cnt<RC_FUN_CNT)
			fun_cnt++;
		else
		{
			fun_cnt = 0;
			ctl_data->ARMED = 0;
//			LED_STA = 0;
		}
	}
	else
		fun_cnt = 0;
}

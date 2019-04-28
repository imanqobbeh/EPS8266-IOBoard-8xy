#include "header_iot.h"

#define DEF_RETRY_CONNECT_WIFI  100    // 100 * 50 = 5 second
// ------------------------------------------------------------------------------------------------------

data_iot data_iot_current, data_iot_last;
sts_led _sts_led = _led_off;

void setup()
{
	init_uart();
	init_data_struct_value(&data_iot_current);
	init_data_struct_value(&data_iot_last);
	led_blinker(_led_off);
	_sts_led = _led_off;
}

void loop()
{
	int ctrled = 0;
	
	int retry_connect = 0, sts = 0;
	int ctr_timer_send_imalive = 0, ctr_timer_check_input = 0;
	int ctr_go_to_pair = 0;
	int pair_condition = 1;
	
	while(1)
	{
		if(check_sts_wifi() == _sts_wifi_disconnected)	
		{
			if(sts == 1)
			{
				led_blinker(_led_off);
				sts = 0;
			}
			
			if(++retry_connect > DEF_RETRY_CONNECT_WIFI)
			{
				connect_to_wifi(_wifi_num_1_run);
				retry_connect = 0;
			}

			if(pair_condition == 1)
			{
				if(++ctr_timer_check_input > 4)
				{
					ctr_timer_check_input = 0;
					if(handker_modbus() == _changed)
						send_data_to_server(data_iot_current, _json_resp_without_cmdid);
				}
			}
			delay(50);
		}
		else
		{
			if(++ctrled > 6)	// Blinker 
			{
				ctrled = 0;
				if(_sts_led == _led_on)
				{
					led_blinker(_led_off);
					_sts_led = _led_off;
				}
				else
				{
					led_blinker(_led_on);
					_sts_led = _led_on;
				}
			}
			
			if(sts == 0)		// this state accur Ones after exit from disconnect state to connect state .
			{
				udp_start();
				send_data_to_server(data_iot_current, _json_alive);
				ctr_timer_send_imalive = 0;
				sts = 1;
			}
			
			if(handler_wifi(&data_iot_current) == _changed)
				send_data_to_server(data_iot_current, _json_resp_with_cmdid);

			if(++ctr_timer_check_input > 4)
			{
				ctr_timer_check_input = 0;
				if(handker_modbus() == _changed)
					send_data_to_server(data_iot_current, _json_sts_change);
			}

			if(++ctr_timer_send_imalive > 400)
			{
				ctr_timer_send_imalive = 0;
				send_data_to_server(data_iot_current, _json_alive);
			}
			
			delay(50);
		}
	}
}


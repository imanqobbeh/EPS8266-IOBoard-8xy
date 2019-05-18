#include "header_iot.h"

#define DEF_RETRY_CONNECT_WIFI  100    // 100 * 50 = 5 second
// ------------------------------------------------------------------------------------------------------

data_iot data_iot_current, data_iot_received;
sts_led _sts_led = _led_off;

void setup()
{
	init_digital_io();
	init_uart();
	init_data_struct_value(&data_iot_current);
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
				init_wifi(_wifi_num_1_run);
				retry_connect = 0;
			}
			
			if(pair_condition == 1)
			{
				if(++ctr_timer_check_input > 4)
				{
					ctr_timer_check_input = 0;
					handler_modbus(&data_iot_current, &data_iot_received, 0);
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
			
			if(handler_wifi(&data_iot_current, &data_iot_received) == _changed)
			{
				//relay_control_total(0x06, 0x10);	
				
				int retry = 0;
				uint8_t relay_data_register;
				relay_data_register = convert_sts_relay_to_reg_modbus(data_iot_received);
				while (1)
				{
					if(relay_control_total(relay_data_register, 0x10) == PROCESS_OK)
					{
						for(int ctr = 0; ctr < 8; ctr++)
							data_iot_current.out[ctr] = data_iot_received.out[ctr];
						break;
					}
					else
					{
						if (retry > RETRY_NUM)
						{
							delay(1000);
							break;
						}
						retry++;
						delay(200);
					}
				}
				send_data_to_server(data_iot_current, _json_response);
			}
			if(++ctr_timer_check_input > 4)
			{
				ctr_timer_check_input = 0;
				if(handler_modbus(&data_iot_current, &data_iot_received, 0) == _changed)
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


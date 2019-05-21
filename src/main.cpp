#include "header_iot.h"

#define DEF_RETRY_CONNECT_WIFI  100    // 100 * 50 = 5 second
// ------------------------------------------------------------------------------------------------------

data_iot data_iot_current, data_iot_received;
system_config_data_struct system_config_data;
modbus_data_struct modbus_data;
irda_data_struct irda_data;

sts_led _sts_led = _led_off;

void setup()
{
	init_digital_io();
	init_memory(_check_inited);
	init_uart();
	init_data_struct_value(&data_iot_current);

	data_iot_current.irda_data = &irda_data;
	data_iot_current.system_config_data = &system_config_data;
	data_iot_current.modbus_data = &modbus_data;

    system_config_data_struct* _system_config_data = data_iot_current.system_config_data; 


	read_data_memory(_system_config_data->ssid, _ssid);
	//sprintf(_system_config_data->ssid, "ovio"); //%s",str_tmp);
	read_data_memory(_system_config_data->pass, _pass);
	//sprintf(_system_config_data->pass, "40800930912");//%s",str_tmp);

	led_blinker(_led_off);
	_sts_led = _led_off;
}

void loop()
{
	int ctrled = 0;
	int ctr_setup_key = 0;
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
				init_wifi(_wifi_num_1_run, &data_iot_current);
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
				send_data_to_server(&data_iot_current, _json_alive);
				ctr_timer_send_imalive = 0;
				sts = 1;
			}
			
			if(handler_wifi(&data_iot_current, &data_iot_received) == _changed)
			{
				system_config_data_struct* _system_config_data = data_iot_current.system_config_data; 
				switch(data_iot_received.type_contents)
				{
					case _type_data:
						handler_modbus(&data_iot_current, &data_iot_received, 1);
						send_data_to_server(&data_iot_current, _json_response);
						break;
					case _type_sysconfig:
						char str_tmp[30];
						
						write_data_memory(_system_config_data->ssid, _ssid);
						//sprintf(_system_config_data->ssid, "%d", str_tmp);
						write_data_memory(_system_config_data->pass, _pass);
						//sprintf(_system_config_data->pass, "%d", str_tmp);

						send_data_to_server(&data_iot_current, _json_system_conf);
						delay(500);
						WiFi.disconnect(true);
						break;
					case _type_modbus_requset:
						break;
					case _type_irda:
						break;
				}
			}

			if(++ctr_timer_check_input > 4)
			{
				ctr_timer_check_input = 0;
				if(handler_modbus(&data_iot_current, &data_iot_received, 0) == _changed)
					send_data_to_server(&data_iot_current, _json_sts_change);
			}

			if(++ctr_timer_send_imalive > 400)
			{
				ctr_timer_send_imalive = 0;
				send_data_to_server(&data_iot_current, _json_alive);
			}
		}

		if(data_iot_current.key[0] == 1)
		{
			if(++ctr_setup_key > 50)
			{
				system_config_data_struct* _system_config_data = data_iot_current.system_config_data; 
				sprintf(_system_config_data->ssid, "%s", "Netware2");
				sprintf(_system_config_data->pass, "%S", "40800930912");
				WiFi.disconnect(true);
			}
		}
		else
			ctr_setup_key = 0;

		delay(50);
	}
}


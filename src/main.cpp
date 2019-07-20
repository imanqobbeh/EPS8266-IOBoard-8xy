#include "header_iot.h"

#define DEF_RETRY_CONNECT_WIFI  100    // 100 * 50 = 5 second
// ------------------------------------------------------------------------------------------------------

data_iot data_iot_current, data_iot_received;
system_config_data_struct system_config_data;
pair_config_data_struct pair_config_data;
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
	data_iot_current.pair_config_data = &pair_config_data;
	
    system_config_data_struct* _system_config_data = data_iot_current.system_config_data;
	read_data_memory(_system_config_data->ssid, _ssid);
	read_data_memory(_system_config_data->pass, _pass);
	read_data_memory(_system_config_data->sip, _sip);
	
	// pair read from memory ---------------------------------------------------
	pair_config_data_struct* _pair_config_data = data_iot_current.pair_config_data;
	read_data_memory(_pair_config_data->pair_active, _pair_active);
	read_data_memory(_pair_config_data->pair_type, _pair_type);
	read_data_memory(_pair_config_data->io1_sts, _pair_io_1);
	read_data_memory(_pair_config_data->io2_sts, _pair_io_2);
	read_data_memory(_pair_config_data->io3_sts, _pair_io_3);
	read_data_memory(_pair_config_data->io4_sts, _pair_io_4);
	read_data_memory(_pair_config_data->io5_sts, _pair_io_5);
	read_data_memory(_pair_config_data->io6_sts, _pair_io_6);
	read_data_memory(_pair_config_data->io7_sts, _pair_io_7);
	read_data_memory(_pair_config_data->io8_sts, _pair_io_8);

	// -------------------------------------------------------------------------
	
	/*
		pair_config_data_struct *_pair_config_tmp = data_input->pair_config_data;
		_pair_config_tmp->total_sts = _deactivate;
		_pair_config_tmp->type = _toggle;
		for(int ctr = 0; ctr < 8; ctr++)
			_pair_config_tmp->io_sts[ctr] = _deactivate;
	*/

	led_blinker(_led_off);
	_sts_led = _led_off;
}

void led_handler(int _sts_led_handler)
{
	static int ctr_led = 0;
	if((_sts_led_handler & _led_disconnect_handler) == _led_disconnect_handler)
	{
		if((_sts_led_handler & _led_run_handler)  == _led_run_handler)
		{
			_sts_led = _led_off;
			led_blinker(_led_off);
		}
		else if((_sts_led_handler & _led_setup_handler) == _led_setup_handler)
		{
			if(_sts_led == _led_off)
			{
				_sts_led = _led_on;
				led_blinker(_led_on);
			}
			else
			{
				_sts_led = _led_off;
				led_blinker(_led_off);
			}
		}
	}
	else if((_sts_led_handler & _led_connnect_handler) == _led_connnect_handler)
	{
		if((_sts_led_handler & _led_run_handler) == _led_run_handler)
		{
			_sts_led = _led_on;
			led_blinker(_led_on);
		}
		else if((_sts_led_handler & _led_setup_handler) == _led_setup_handler)
		{
			if(++ctr_led > 6)
			{
				ctr_led = 0;
				if(_sts_led == _led_off)
				{
					_sts_led = _led_on;
					led_blinker(_led_on);
				}
				else
				{
					_sts_led = _led_off;
					led_blinker(_led_off);
				}
			}
		}
	}
}

void loop()
{
	int _led_handler = (_led_disconnect_handler | _led_run_handler);
	int ctrled = 0, led_freq = 6;
	int ctr_setup_key = 0, setup_status = 0, ctr_stay_in_setup = 0;
	int retry_connect = 0, sts = 0;
	int ctr_timer_send_imalive = 0, ctr_timer_check_input = 0;
	int ctr_go_to_pair = 0;
	int pair_condition = 1;

	while(1)
	{
		if(check_sts_wifi() == _sts_wifi_disconnected)
		{
			if(sts == 1)
				sts = 0;

			if(setup_status == 0)
				_led_handler = (_led_disconnect_handler | _led_run_handler);

			if(setup_status == 3)
			{
				setup_status = 0;
				_led_handler = (_led_disconnect_handler | _led_run_handler);
			}

			if(setup_status == 1)
			{
				setup_status = 2;
				_led_handler = (_led_disconnect_handler | _led_setup_handler);
			}
			
			led_handler(_led_handler);
			
			if(++retry_connect > DEF_RETRY_CONNECT_WIFI)
			{
				if(setup_status == 2)
					init_wifi(_wifi_num_2_setup, &data_iot_current);
				else
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
			if(setup_status == 2)
			{
				setup_status = 3;
				_led_handler = (_led_connnect_handler | _led_setup_handler);
			}

			else if(setup_status == 0)
				_led_handler = (_led_connnect_handler | _led_run_handler);
			
			led_handler(_led_handler);
			
			if(sts == 0)		// this state accur Ones after exit from disconnect state to connect state .
			{
				udp_start();
				send_data_to_server(&data_iot_current, _json_alive);
				ctr_timer_send_imalive = 0;
				sts = 1;
			}

			if(handler_wifi(&data_iot_current, &data_iot_received) == _changed)
			{
				switch(data_iot_received.type_contents)
				{
					case _type_data:
						handler_modbus(&data_iot_current, &data_iot_received, 1);
						send_data_to_server(&data_iot_current, _json_response);
						break;
					case _type_sysconfig:
						system_config_data_struct* _system_config_data = data_iot_current.system_config_data;
						write_data_memory(_system_config_data->ssid, _ssid);
						write_data_memory(_system_config_data->pass, _pass);
						write_data_memory(_system_config_data->sip, _sip);
						send_data_to_server(&data_iot_current, _json_system_conf);
						_sts_led = _led_off;
						led_blinker(_led_off);
						setup_status = 0;
						_led_handler = (_led_disconnect_handler | _led_run_handler);
						delay(500);
						while(WL_CONNECTED == WiFi.disconnect(true))
							delay(500);
						break;
					//case _pair_config:
					//	break;
					//case _type_modbus_requset:
					//	break;
					//case _type_irda:
					//	break;
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

		if((data_iot_current.key[0] == 1) && (setup_status == 0))
		{
			if(++ctr_setup_key > 60)
			{
				_sts_led = _led_off;
				led_blinker(_led_off);
				delay(2000);
				
				ctr_setup_key = 0;
				
				if(setup_status == 0)
					setup_status = 1;
				
				while(WL_CONNECTED == WiFi.disconnect(true))
					delay(500);
			}
		}
		else
			ctr_setup_key = 0;

		if(setup_status != 0)
		{
			if(++ctr_stay_in_setup > 5000)
			{
				_sts_led = _led_off;
				led_blinker(_led_off);
				setup_status = 0;
				WiFi.disconnect(true);
				delay(500);
			}
		}
		else
			ctr_stay_in_setup = 0;
		
		delay(50);
	}
}


#include "server_interface.h"
#include "header_iot.h"

char ip_server_ovio[16] = "192.168.1.255";

byte mac[6];
WiFiUDP Udp;
unsigned int localUdpPort = 6364;					  // local port to listen on
char incomingPacket[255];							  // buffer for incoming packets

void udp_start(void)
{
	Udp.begin(localUdpPort);
}

sts_packet_udp get_packet_udp(char *str)
{
	if(check_sts_wifi() == _sts_wifi_connceted)
	{
		int packetSize = Udp.parsePacket();
		if(packetSize)
		{
			int len = Udp.read(incomingPacket, 255);
			if(len > 0)
			{
				strcpy(str, incomingPacket);
				return _sts_packet_success;
			}
		}
	}
	else
		return _sts_packet_faild;
}

void send_packet_udp(char *str)
{
	Udp.beginPacket(ip_server_ovio, localUdpPort);
	Udp.write(str);
	Udp.endPacket();
}

change_sts handler_wifi(data_iot* _data_iot_input, data_iot* _data_iot_output)
{
	change_sts _change_sts = _unchanged;
	data_iot tmp_data_iot;
	_data_iot_output->type_contents = _type_empty;
	for(int ctr = 0; ctr <= 7; ctr++)
		_data_iot_output->out[ctr] = _data_iot_input->out[ctr];
	
	char str_test[255];
	StaticJsonBuffer<200> jsonBuffer;
	
	if(get_packet_udp(str_test) != _sts_packet_success)
		return _change_sts;


	JsonObject& root = jsonBuffer.parseObject(str_test);
	if(root.success())
	{
		if(root.containsKey("id"))
		{
			double _ovio_device_id = root["id"];
			if(_ovio_device_id == _data_iot_input->uid)
			{
				_change_sts = _changed;	// edit for replay on every packet with my id...... khalil

				if(root.containsKey("cmdid"))
					_data_iot_output->cmd_id = root["cmdid"];

				if(root.containsKey("data"))
				{
					_data_iot_output->type_contents = _type_data;
					JsonObject& root2 = root["data"];
					char str_tmp[10];
					for(int ctr = 0; ctr < 8; ctr++)
					{
						sprintf(str_tmp, "o%d", ctr + 1);
						if(root2.containsKey(str_tmp))
							_data_iot_output->out[ctr] = root["data"][str_tmp];
					}
				}
				
				if(root.containsKey("sys_config"))
				{
					_data_iot_output->type_contents = _type_sysconfig;
   					JsonObject& root2 = root["sys_config"];
   					
					system_config_data_struct* ptr_system_config;
					ptr_system_config = _data_iot_input->system_config_data;
					
					if(root2.containsKey("ssid"))
					{
						String tmp_ssid = root2["ssid"];
						tmp_ssid.toCharArray(ptr_system_config->ssid,30);
					}
					if(root2.containsKey("pass"))
					{
						String tmp_pass = root2["pass"];  
						tmp_pass.toCharArray(ptr_system_config->pass,30);
					}
					
				}
			}
		}
	}

	return _change_sts;
}

void send_data_to_server(data_iot* _data_input, json_builder_mode _Builder_mode)
{
	char packet_for_send[256];
	json_packet_builder(packet_for_send, _data_input, _Builder_mode);
	send_packet_udp(packet_for_send);
}

void json_packet_builder(char *str_out, data_iot* _data_input, json_builder_mode _Builder_mode)
{
	memset(str_out, 0, 255);

	sprintf(str_out, "{");
	sprintf(str_out + strlen(str_out), "\"id\":%.0f",_data_input->uid);
	sprintf(str_out + strlen(str_out), ",\"model\":\"IO8A-01\"");
	sprintf(str_out + strlen(str_out), ",\"ver\":\"esp-8xy v2.0\"");

	if((_Builder_mode == _json_response) || (_Builder_mode == _json_sts_change) || (_Builder_mode == _json_alive))
	{
		switch(_Builder_mode)
		{
			case _json_response:
				sprintf(str_out + strlen(str_out), ",\"type\":\"reply\",\"cmdid\":%d", _data_input->cmd_id);
				break;
			case _json_sts_change:
				sprintf(str_out + strlen(str_out), ",\"type\":\"stchng\"");
				break;
			case _json_alive:
				sprintf(str_out + strlen(str_out), ",\"type\":\"imalive\"");
				break;
		}

		sprintf(str_out + strlen(str_out), ",\"data\":{");

		for(int ctr = 0; ctr < 8; ctr++)
			sprintf(str_out + strlen(str_out), "\"o%d\":%d,", ctr + 1,_data_input->out[ctr]);

		for(int ctr = 0; ctr < 8; ctr++)
			sprintf(str_out + strlen(str_out), "\"i%d\":%d,", ctr + 1,_data_input->input[ctr]);

		sprintf(str_out + strlen(str_out), "\"k1\":%d",_data_input->key[0]);

		sprintf(str_out + strlen(str_out), "}}");
	}
	else if(_Builder_mode == _json_system_conf)
	{
		system_config_data_struct* ptr_system_config;
		ptr_system_config = _data_input->system_config_data;

		sprintf(str_out + strlen(str_out), ",\"type\":\"reply\",\"cmdid\":%d", _data_input->cmd_id);
		sprintf(str_out + strlen(str_out), ",\"sys_config\":{");
		sprintf(str_out + strlen(str_out), "\"ssid\":%s,\"pass\":%s", ptr_system_config->ssid, ptr_system_config->pass);
		sprintf(str_out + strlen(str_out), "}}");
	}
}
//WiFi.macAddress(mac);

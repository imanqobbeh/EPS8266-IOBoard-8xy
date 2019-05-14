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

change_sts handler_wifi(data_iot* _data_iot)
{
	change_sts _change_sts = _unchanged;
	data_iot tmp_data_iot;


	tmp_data_iot.out1 = _data_iot->out1;
    tmp_data_iot.out2 = _data_iot->out2;
	tmp_data_iot.out3 = _data_iot->out3;
	tmp_data_iot.out4 = _data_iot->out4;
	tmp_data_iot.out5 = _data_iot->out5;
	tmp_data_iot.out6 = _data_iot->out6;
	tmp_data_iot.out7 = _data_iot->out7;
	tmp_data_iot.out8 = _data_iot->out8;
	
	char str_test[255];
	StaticJsonBuffer<200> jsonBuffer;
	
	if(get_packet_udp(str_test) == _sts_packet_success)
	{
		#ifdef _DEBUG_CODE
		Serial.print("yegohi resid");
		#endif
		JsonObject& root = jsonBuffer.parseObject(str_test);
		if(root.success())
		{
			#ifdef _DEBUG_CODE
			Serial.print(" oon gohe json bood ");
			#endif
			if(root.containsKey("id"))
			{
				double _ovio_device_id = root["id"];
				if(_ovio_device_id == ESP.getChipId())
				{
					// edit for replay on every packet with my id...... khalil ;/
					_change_sts = _changed;	/////////////////////////////////////
					// ..........................................................
					
					#ifdef _DEBUG_CODE
					Serial.print(" id khodam boooooooood :) ");
					#endif
					
					if(root.containsKey("cmdid"))
					{
						_data_iot->cmd_id = root["cmdid"];
					}
					
					_change_sts = _changed;
					
					if(root.containsKey("data"))
					{
						#ifdef _DEBUG_CODE
						Serial.print(" data boooooooood :) ");
						#endif
						JsonObject& root2 = root["data"];
						if(root2.containsKey("o1"))
						{
							//relay_control_total(0x04, 0x10);
							#ifdef _DEBUG_CODE
							Serial.print(" o1 tooshe !!! :) ");
							#endif
							if(root["data"]["o1"] == 1)
							{
								if(_data_iot->out1 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o1_on ");
									#endif
									tmp_data_iot.out1 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out1 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o1_off ");
									#endif
									tmp_data_iot.out1 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}

						if(root2.containsKey("o2"))
						{
							#ifdef _DEBUG_CODE
							Serial.print(" o2 tooshe !!! :) ");
							#endif
							if(root["data"]["o2"] == 1)
							{
								if(_data_iot->out2 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o2_on ");
									#endif
									tmp_data_iot.out2 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out2 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o2_off ");
									#endif
									tmp_data_iot.out2 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}
						
						if(root2.containsKey("o3"))
						{
							#ifdef _DEBUG_CODE
							Serial.print(" o3 tooshe !!! :) ");
							#endif
							if(root["data"]["o3"] == 1)
							{
								if(_data_iot->out3 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o3_on ");
									#endif
									tmp_data_iot.out3 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out3 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o3_off ");
									#endif
									tmp_data_iot.out3 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}
						if(root2.containsKey("o4"))
						{
							#ifdef _DEBUG_CODE
							Serial.print(" o4 tooshe !!! :) ");
							#endif
							if(root["data"]["o4"] == 1)
							{
								if(_data_iot->out4 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o4_on ");
									#endif
									tmp_data_iot.out4 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out4 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o4_off ");
									#endif
									tmp_data_iot.out4 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}
						if(root2.containsKey("o5"))
						{
							#ifdef _DEBUG_CODE
							Serial.print(" o5 tooshe !!! :) ");
							#endif
							if(root["data"]["o5"] == 1)
							{
								if(_data_iot->out5 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o5_on ");
									#endif
									tmp_data_iot.out5 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out5 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o5_off ");
									#endif
									tmp_data_iot.out5 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}
						if(root2.containsKey("o6"))
						{
							#ifdef _DEBUG_CODE
							Serial.print(" o6 tooshe !!! :) ");
							#endif
							if(root["data"]["o6"] == 1)
							{
								if(_data_iot->out6 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o6_on ");
									#endif
									tmp_data_iot.out6 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out6 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o6_off ");
									#endif
									tmp_data_iot.out6 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}
						if(root2.containsKey("o7"))
						{
							#ifdef _DEBUG_CODE
							Serial.print(" o6 tooshe !!! :) ");
							#endif
							if(root["data"]["o7"] == 1)
							{
								if(_data_iot->out7 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o7_on ");
									#endif
									tmp_data_iot.out7 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out7 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o7_off ");
									#endif
									tmp_data_iot.out7 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}
						if(root2.containsKey("o8"))
						{
							#ifdef _DEBUG_CODE
							Serial.print(" o6 tooshe !!! :) ");
							#endif
							
							if(root["data"]["o8"] == 1)
							{
								if(_data_iot->out8 == RELAY_OFF)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o7_on ");
									#endif
									tmp_data_iot.out8 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(_data_iot->out8 == RELAY_ON)
								{
									#ifdef _DEBUG_CODE
									Serial.print("o7_off ");
									#endif
									tmp_data_iot.out8 = RELAY_OFF;
									_change_sts = _changed;
								}
							}
						}
					}
				}
				#ifdef _DEBUG_CODE
				else
				{
					Serial.print(" vase man nabood oon gohe ! ");
				}
				#endif
			}
			//_change_sts = _changed;
		}
		#ifdef _DEBUG_CODE
		else
			Serial.println("ye gohi resid ! vali json nabood !! ");
		#endif
	}

	if(_change_sts == _changed)
	{
		//relay_control_total(0x06, 0x10);
		int retry = 0;
		uint8_t relay_data_register;
		relay_data_register = convert_sts_relay_to_reg_modbus(tmp_data_iot);
		while (1)
		{
			if (relay_control_total(relay_data_register, 0x10) == PROCESS_OK)
			{
				_data_iot->out1 = tmp_data_iot.out1;
				_data_iot->out2 = tmp_data_iot.out2;
				_data_iot->out3 = tmp_data_iot.out3;
				_data_iot->out4 = tmp_data_iot.out4;
				_data_iot->out5 = tmp_data_iot.out5;
				_data_iot->out6 = tmp_data_iot.out6;
				_data_iot->out7 = tmp_data_iot.out7;
				_data_iot->out8 = tmp_data_iot.out8;
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
	}
	return _change_sts;
}

void send_data_to_server(data_iot _data_input, json_builder_mode _Builder_mode)
{
	char packet_for_send[256];
	json_packet_builder(packet_for_send, _data_input, _Builder_mode);
	send_packet_udp(packet_for_send);
}

void json_packet_builder(char *str_out, data_iot _data_input, json_builder_mode _Builder_mode)
{
	memset(str_out, 0, 255);

	sprintf(str_out, "{");
	sprintf(str_out + strlen(str_out), "\"id\":%.0f",_data_input.uid);
	sprintf(str_out + strlen(str_out), ",\"model\":\"IO8A-01\"");
	sprintf(str_out + strlen(str_out), ",\"ver\":\"esp-8xy v2.0\"");
	switch(_Builder_mode)
	{
		case _json_response:
			sprintf(str_out + strlen(str_out), ",\"type\":\"reply\",\"cmdid\":%d", _data_input.cmd_id);
			break;
		case _json_sts_change:
			sprintf(str_out + strlen(str_out), ",\"type\":\"stchng\"");
			break;
		case _json_alive:
			sprintf(str_out + strlen(str_out), ",\"type\":\"imalive\"");
			break;
	}

	sprintf(str_out + strlen(str_out), ",\"data\":{");

	sprintf(str_out + strlen(str_out), "\"o1\":%d", _data_input.out1);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"o2\":%d", _data_input.out2);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"o3\":%d", _data_input.out3);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"o4\":%d", _data_input.out4);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"o5\":%d", _data_input.out5);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"o6\":%d", _data_input.out6);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"o7\":%d", _data_input.out7);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"o8\":%d", _data_input.out8);
	sprintf(str_out + strlen(str_out), ",");
	
	sprintf(str_out + strlen(str_out), "\"i1\":%d", _data_input.input1);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i2\":%d", _data_input.input2);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i3\":%d", _data_input.input3);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i4\":%d", _data_input.input4);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i5\":%d", _data_input.input5);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i6\":%d", _data_input.input6);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i7\":%d", _data_input.input7);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i8\":%d", _data_input.input8);
	sprintf(str_out + strlen(str_out), ",");
	sprintf(str_out + strlen(str_out), "\"i9\":%d", _data_input.input9);

	sprintf(str_out + strlen(str_out), "}}");
}
//WiFi.macAddress(mac);

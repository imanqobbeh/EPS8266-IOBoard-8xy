#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "server_interface.h"
#include <ArduinoJson.h>

//#define DEVICE_MODEL 	"SW3A-01"
#include "EEPROM.h"
#include "mainboard_interface.h"
#include "memory.h"
// ------------------------------------------------------------------------------------------------------
extern int out1, out2, out3, out4, out5, out6, out6, out7, out8;
int last_out1, last_out2, last_out3, last_out4, last_out5, last_out6, last_out7, last_out8;
int last_input1, last_input2, last_input3, last_input4, last_input5, last_input6, last_input7, last_input8;
extern int input1, input2, input3, input4, input5, input6, input7, input7, input8, input9, sensor_pir, temperature;
uint16_t last_sts_input = 0;
uint16_t last_sts_output = 0;
uint16_t last_temperature = 0;
uint8_t sts_pair_io = 0;


//#define _DEBUG_CODE
#define _RELEASE_CODE

enum change_sts
{
	_changed,
	_unchanged
};

change_sts handker_modbus(void);
change_sts handler_wifi(int* _cmdid);

void status_key_json_packet(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8);
void i_am_alive_json_packet(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8);
void status_key_json_packet_1(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8, int _i9);
void status_key_json_packet_2(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8, int _i9, int _cmdid);
uint8_t convert_sts_relay_to_reg_modbus(int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8);
change_sts pair_handler(void);

double ovio_device_id;

int addr = 0; //////////////

void setup()
{
	char str_test[255];
	///
	pinMode(2, OUTPUT);
	///
	// Init I/O touch key and relay controller ------------------------------------------------
	ovio_device_id = ESP.getChipId();
	out1 = 0; out2 = 0; out3 = 0;
	last_out1 = 0; last_out2 = 0; last_out3 = 0; last_out4 = 0; last_out5 = 0; last_out6 = 0; last_out7 = 0; last_out8 = 0;
	last_input1 = 0; last_input2 = 0; last_input3 = 0; last_input4 = 0; last_input5 = 0; last_input6 = 0; last_input7 = 0; last_input8 = 0;
	Serial.begin(115200);
	Serial.println();
	while (!Serial) continue;

	/*
	Serial.begin(115200);
	Serial.println();
	while (!Serial) continue;
	while(1)
	{
		relay_control_total(0x07, 0x10);
		delay(500);
		relay_control_total(0x00, 0x10);
		delay(500);
	}
	*/
	// Init Serial port for debug -------------------------------------------------------------
	/*
	while(1)
	{
		delay(500);
		if(connect_to_wifi(_wifi_num_1, 10) == _connect_success)
		{
			blinker(10);
			udp_start();
			break;
		}
	}
	*/
	#ifdef _DEBUG_CODE
	Serial.begin(115200);
	Serial.println();
	while (!Serial) continue;
	
	/////////////////////
	EEPROM.begin(512);///////////////////
	int val = 0;
	/*
	for(int ctr = 0; ctr < 512; ctr++)
	{
		EEPROM.write(addr++, ctr + 3);
	}
	*/

    addr = 0;
    //EEPROM.commit();
	
	for(int ctr = 0; ctr < 100; ctr++)
	{
		val = EEPROM.read(addr);
		Serial.print(addr++);
		Serial.print("\t---");
		Serial.print(val, DEC);
		Serial.println();
		delay(100);
	}
	
	//////////////////
	connect_to_wifi(_wifi_num_1);
	while(1)
	{
		delay(500);
		Serial.print("disonnected");
		if(check_sts_wifi() == _sts_wifi_connceted)
		{
			Serial.println("");
			Serial.print("Connected");
			Serial.print("IP address: ");
			Serial.println(WiFi.localIP());  			//IP address assigned to your ESP
			Serial.print("MAC address: ");
			Serial.print(WiFi.macAddress());
			Serial.printf("The ESP8266 chip ID as a 32-bit integer:\t%d\n", ESP.getChipId());
			udp_start();
			Serial.print("-----------------------------------");
			
			
			while(1)
			{
				handler_wifi();
				delay(50);
			}
			
			
			while(1)
			{
				delay(50);
				if(get_packet_udp(str_test) == _sts_packet_success)
				{
					StaticJsonBuffer<200> jsonBuffer;
					JsonObject& root2 = jsonBuffer.parseObject(str_test);
					if(!root2.success())
					{
						Serial.println("parseObject() failed");
						return;
					}
					else
					{
						const char* sensor = root2["sensor"];
						long time = root2["time"];
						double latitude = root2["data"]["o1"];
						double longitude = root2["data"]["o2"];
						
						// Print values.
						Serial.println(sensor);
						Serial.println(time);
						Serial.println(latitude, 6);
						Serial.println(longitude, 6);
					}
				}
			}
		}
	}
	#endif
}

void loop()
{
	int ctrled = 0, stsled = 0;
	int retry_connect = 0, sts = 0;
	char packet_for_send[256];
	int ctr_timer_send_imalive = 0, ctr_timer_check_input = 0;
	int test_ = 0;
	int ctr_go_to_pair = 0;
	int _cmdid = 0;
	digitalWrite(2, HIGH);
	/////////////////
	int pair_condition = 1;
	
	////////////////
	while(1)
	{
		if(check_sts_wifi() == _sts_wifi_disconnected)
		{
			if(sts == 1)
			{
				digitalWrite(2, HIGH);
				sts = 0;
			}

			if(++retry_connect > 100)
			{
				connect_to_wifi(_wifi_num_1);
				retry_connect = 0;
			}

			if(pair_condition == 1)
			{
				if(++ctr_timer_check_input > 4)
				{
					ctr_timer_check_input = 0;
					if(handker_modbus() == _changed)
					{
						status_key_json_packet_1(packet_for_send, out1, out2, out3, out4, out5, out6, out7, out8, input1, input2, input3, input4, input5, input6, input7, input8, input9);
						send_packet_udp(packet_for_send);
					}
				}
			}
			// handler_touch();
			delay(50);
		}
		else
		{
			if(++ctrled > 6)
			{
				ctrled = 0;
				if(stsled == 0)
				{
					digitalWrite(2, HIGH);
					stsled = 1;
				}
				else
				{
					digitalWrite(2, LOW);
					stsled = 0;
				}
			}
			
			if(sts == 0)
			{
				//blinker(10);
				udp_start();
				i_am_alive_json_packet(packet_for_send, out1, out2, out3, out4, out5, out6, out7, out8, input1, input2, input3, input4, input5, input6, input7, input8);
				send_packet_udp(packet_for_send);
				ctr_timer_send_imalive = 0;
				sts = 1;
			}
			
			if(handler_wifi(&_cmdid) == _changed)
			{
				status_key_json_packet_2(packet_for_send, out1, out2, out3, out4, out5, out6, out7, out8, input1, input2, input3, input4, input5, input6, input7, input8, input9, _cmdid);
				send_packet_udp(packet_for_send);
			}
			
			if(++ctr_timer_check_input > 4)
			{
				ctr_timer_check_input = 0;
				if(handker_modbus() == _changed)
				{
					status_key_json_packet_1(packet_for_send, out1, out2, out3, out4, out5, out6, out7, out8, input1, input2, input3, input4, input5, input6, input7, input8, input9);
					send_packet_udp(packet_for_send);
				}
			}
			if(input9 == 1)
			{
				if(++ctr_go_to_pair > 100)
				{
					if(sts_pair_io == 0)
						sts_pair_io = 1;
					else
						sts_pair_io = 0;
				}
			}
			else
				ctr_go_to_pair = 0;
			
			if(++ctr_timer_send_imalive > 400)
			{
				ctr_timer_send_imalive = 0;
				i_am_alive_json_packet(packet_for_send, out1, out2, out3, out4, out5, out6, out7, out8, input1, input2, input3, input4, input5, input6, input7, input8);
				send_packet_udp(packet_for_send);
			}
			delay(50);
		}
	}
}
/*
change_sts pair_handler(void)
{
	if(handker_modbus() == _changed)
	{
		//status_key_json_packet_1(packet_for_send, out1, out2, out3, out4, out5, out6, out7, out8, input1, input2, input3, input4, input5, input6, input7, input8, input9);
		if(last_input1 != input1)
		{
			if(last_out1 == RELAY_OFF)
			{
				
				last_out1 = RELAY_ON;
				_change_sts = _changed;
			}
			last_input1 = input1;
		}



		; last_input2 = 0; last_input3 = 0; last_input4 = 0; last_input5 = 0; last_input6 = 0; last_input7 = 0; last_input8 = 0;
		send_packet_udp(packet_for_send);
	}
}
*/
change_sts handler_wifi(int* _cmdid)
{
	change_sts _change_sts = _unchanged;
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
			//relay_control_total(0x01, 0x10);
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
					//relay_control_total(0x02, 0x10);
					#ifdef _DEBUG_CODE
					Serial.print(" id khodam boooooooood :) ");
					#endif
					if(root.containsKey("cmdid"))
					{
						*_cmdid = root["cmdid"];
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
								if(last_out1 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_1, _switch_on);
									//switch_control(_num_switch_4, _switch_on);
									#endif

									#ifdef _DEBUG_CODE
									Serial.print("o1_on ");
									#endif
									last_out1 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out1 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_1, _switch_off);
									//switch_control(_num_switch_4, _switch_off);
									#endif

									#ifdef _DEBUG_CODE
									Serial.print("o1_off ");
									#endif
									last_out1 = RELAY_OFF;
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
								if(last_out2 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_2, _switch_on);
									#endif

									#ifdef _DEBUG_CODE
									Serial.print("o2_on ");
									#endif
									last_out2 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out2 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_2, _switch_off);
									#endif

									#ifdef _DEBUG_CODE
									Serial.print("o2_off ");
									#endif
									last_out2 = RELAY_OFF;
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
								if(last_out3 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_on);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o3_on ");
									#endif
									last_out3 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out3 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_off);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o3_off ");
									#endif
									last_out3 = RELAY_OFF;
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
								if(last_out4 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_on);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o4_on ");
									#endif
									last_out4 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out4 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_off);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o4_off ");
									#endif
									last_out4 = RELAY_OFF;
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
								if(last_out5 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_on);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o5_on ");
									#endif
									last_out5 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out5 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_off);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o5_off ");
									#endif
									last_out5 = RELAY_OFF;
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
								if(last_out6 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_on);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o6_on ");
									#endif
									last_out6 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out6 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_off);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o6_off ");
									#endif
									last_out6 = RELAY_OFF;
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
								if(last_out7 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_on);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o7_on ");
									#endif
									last_out7 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out7 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_off);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o7_off ");
									#endif
									last_out7 = RELAY_OFF;
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
								if(last_out8 == RELAY_OFF)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_on);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o7_on ");
									#endif
									last_out8 = RELAY_ON;
									_change_sts = _changed;
								}
							}
							else
							{
								if(last_out8 == RELAY_ON)
								{
									#ifdef _RELEASE_CODE
									//switch_control(_num_switch_3, _switch_off);
									#endif
									
									#ifdef _DEBUG_CODE
									Serial.print("o7_off ");
									#endif
									last_out8 = RELAY_OFF;
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
		relay_data_register = convert_sts_relay_to_reg_modbus(last_out1, last_out2, last_out3, last_out4, last_out5, last_out6, last_out7, last_out8);
		while (1)
		{

			if (relay_control_total(relay_data_register, 0x10) == PROCESS_OK)
			{
				out1 = last_out1;
				out2 = last_out2;
				out3 = last_out3;
				out4 = last_out4;
				out5 = last_out5;
				out6 = last_out6;
				out7 = last_out7;
				out8 = last_out8;

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

uint8_t convert_sts_relay_to_reg_modbus(int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8)
{
	uint8 reg_data = 0;

	if(_o1 == RELAY_ON)
		reg_data |= 0x01;
	
	if(_o2 == RELAY_ON)
		reg_data |= 0x02;
	
	if(_o3 == RELAY_ON)
		reg_data |= 0x04;

	if(_o4 == RELAY_ON)
		reg_data |= 0x08;

	if(_o5 == RELAY_ON)
		reg_data |= 0x10;

	if(_o6 == RELAY_ON)
		reg_data |= 0x20;

	if(_o7 == RELAY_ON)
		reg_data |= 0x40;

	if(_o8 == RELAY_ON)
		reg_data |= 0x80;
	
	return reg_data;
}

change_sts handker_modbus(void)
{
	uint16_t register_modbus[10];
	
    change_sts _change_sts = _unchanged, _change_sts_input = _unchanged;
	if (read_register(0, 2, 0x10, register_modbus) == PROCESS_OK)
	{
		if (register_modbus[0] != last_sts_input)
		{	
			if(/*sts_pair_io*/0)
			{
				uint16_t tmp_last_sts_input;
				tmp_last_sts_input = register_modbus[0];
				tmp_last_sts_input ^= last_sts_input;
				
				if((tmp_last_sts_input & 0x01) == 0x01)
				{
					if(last_out1 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o1_on ");
						#endif
						last_out1 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o1_off ");
						#endif
						last_out1 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if((tmp_last_sts_input & 0x02) == 0x02)
				{
					if(last_out2 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o2_on ");
						#endif
						last_out2 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o2_off ");
						#endif
						last_out2 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if((tmp_last_sts_input & 0x04) == 0x04)
				{
					if(last_out3 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o3_on ");
						#endif
						last_out3 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o3_off ");
						#endif
						last_out3 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if((tmp_last_sts_input & 0x08) == 0x08)
				{
					if(last_out4 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o4_on ");
						#endif
						last_out4 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o4_off ");
						#endif
						last_out4 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x10) == 0x10)
				{
					if(last_out5 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o5_on ");
						#endif
						last_out5 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o5_off ");
						#endif
						last_out5 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x20) == 0x20)
				{
					if(last_out6 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o6_on ");
						#endif
						last_out6 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o6_off ");
						#endif
						last_out6 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x40) == 0x40)
				{
					if(last_out7 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o7_on ");
						#endif
						last_out7 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o7_off ");
						#endif
						last_out7 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x80) == 0x80)
				{
					if(last_out8 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o8_on ");
						#endif
						last_out8 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o8_off ");
						#endif
						last_out8 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if(_change_sts_input == _changed)
				{
					//relay_control_total(0x06, 0x10);
					int retry = 0;
					uint8_t relay_data_register;
					relay_data_register = convert_sts_relay_to_reg_modbus(last_out1, last_out2, last_out3, last_out4, last_out5, last_out6, last_out7, last_out8);
					while (1)
					{

						if (relay_control_total(relay_data_register, 0x10) == PROCESS_OK)
						{
							out1 = last_out1;
							out2 = last_out2;
							out3 = last_out3;
							out4 = last_out4;
							out5 = last_out5;
							out6 = last_out6;
							out7 = last_out7;
							out8 = last_out8;

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
				///

			}

			last_sts_input = register_modbus[0];
			if((last_sts_input & 0x01) == 0x01)
				input1 = INPUT_HIGH;
			else
				input1 = INPUT_LOW;

			if((last_sts_input & 0x02) == 0x02)
				input2 = INPUT_HIGH;
			else
				input2 = INPUT_LOW;

			if((last_sts_input & 0x04) == 0x04)
				input3 = INPUT_HIGH;
			else
				input3 = INPUT_LOW;

			if((last_sts_input & 0x08) == 0x08)
				input4 = INPUT_HIGH;
			else
				input4 = INPUT_LOW;

			if((last_sts_input & 0x10) == 0x10)
				input5 = INPUT_HIGH;
			else
				input5 = INPUT_LOW;

			if((last_sts_input & 0x20) == 0x20)
				input6 = INPUT_HIGH;
			else
				input6 = INPUT_LOW;

			if((last_sts_input & 0x40) == 0x40)
				input7 = INPUT_HIGH;
			else
				input7 = INPUT_LOW;

			if((last_sts_input & 0x80) == 0x80)
				input8 = INPUT_HIGH;
			else
				input8 = INPUT_LOW;

			if((last_sts_input & 0x0100) == 0x0100)
				input9 = INPUT_HIGH;
			else
				input9 = INPUT_LOW;

			_change_sts = _changed;
		}
	}
	
	uint16_t tmp;
	if (register_modbus[2] >= last_temperature)
		tmp = register_modbus[2] - last_temperature;
	else
		tmp = last_temperature - register_modbus[2];
	if (tmp >= 60)
	{
		last_temperature = register_modbus[2];
		temperature = last_temperature;
		_change_sts = _changed;
	}
	return _change_sts;
}

//#define DEVICE_MODEL 	"SW3A-01" ---------------------------------------------------

void status_key_json_packet(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8)
{
	memset(str_out, 0, 255);
	sprintf(str_out, "{\"id\":%.0f,\"type\":\"stchng\",\"model\":\"IO8A-01\",\"ver\":\"1.01\",\"data\":{\"o1\":%d,\"o2\":%d,\"o3\":%d,\"o4\":%d,\"o5\":%d,\"o6\":%d,\"o7\":%d,\"o8\":%d,\"i1\":%d,\"i2\":%d,\"i3\":%d,\"i4\":%d,\"i5\":%d,\"i6\":%d,\"i7\":%d,\"i8\":%d}}", ovio_device_id, _o1, _o2, _o3, _o4, _o5, _o6, _o7, _o8, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8);
}

void status_key_json_packet_1(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8, int _i9)
{
	memset(str_out, 0, 255);
	sprintf(str_out, "{\"id\":%.0f,\"type\":\"stchng\",\"model\":\"IO8A-01\",\"ver\":\"1.01\",\"data\":{\"o1\":%d,\"o2\":%d,\"o3\":%d,\"o4\":%d,\"o5\":%d,\"o6\":%d,\"o7\":%d,\"o8\":%d,\"i1\":%d,\"i2\":%d,\"i3\":%d,\"i4\":%d,\"i5\":%d,\"i6\":%d,\"i7\":%d,\"i8\":%d,\"i9\":%d}}", ovio_device_id, _o1, _o2, _o3, _o4, _o5, _o6, _o7, _o8, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9);
}

void status_key_json_packet_2(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8, int _i9, int _cmdid)
{
	memset(str_out, 0, 255);
	sprintf(str_out, "{\"id\":%.0f,\"type\":\"reply\",\"cmdid\":%d,\"model\":\"IO8A-01\",\"ver\":\"1.01\",\"data\":{\"o1\":%d,\"o2\":%d,\"o3\":%d,\"o4\":%d,\"o5\":%d,\"o6\":%d,\"o7\":%d,\"o8\":%d,\"i1\":%d,\"i2\":%d,\"i3\":%d,\"i4\":%d,\"i5\":%d,\"i6\":%d,\"i7\":%d,\"i8\":%d,\"i9\":%d}}", ovio_device_id, _cmdid, _o1, _o2, _o3, _o4, _o5, _o6, _o7, _o8, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8, _i9);
}

void i_am_alive_json_packet(char *str_out, int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8, int _i1, int _i2, int _i3, int _i4, int _i5, int _i6, int _i7, int _i8)
{
	memset(str_out, 0, 255);
	sprintf(str_out, "{\"id\":%.0f,\"type\":\"imalive\",\"model\":\"IO8A-01\",\"ver\":\"1.01\",\"data\":{\"o1\":%d,\"o2\":%d,\"o3\":%d,\"o4\":%d,\"o5\":%d,\"o6\":%d,\"o7\":%d,\"o8\":%d,\"i1\":%d,\"i2\":%d,\"i3\":%d,\"i4\":%d,\"i5\":%d,\"i6\":%d,\"i7\":%d,\"i8\":%d}}", ovio_device_id, _o1, _o2, _o3, _o4, _o5, _o6, _o7, _o8, _i1, _i2, _i3, _i4, _i5, _i6, _i7, _i8);
}

// ---------------------------------------------------------------------------------

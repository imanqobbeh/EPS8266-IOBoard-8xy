#include "server_interface.h"

//#define VAHED_42
#define VAHED_66

#ifdef VAHED_42

const char *ssid1 = "ovio";
const char *password1 = "40800930912";

const char *ssid2 = "Netware2";
const char *password2 = "40800930912";

const char *ssid3 = "ovio";
const char *password3 = "testpass";


const char *ip_brodcast = "192.168.1.255";
char ip_server_ovio[16] = "192.168.1.255";
#endif

#ifdef VAHED_66

const char *ssid1 = "ovio";
const char *password1 = "40800930912";

const char *ssid2 = "Netware2";
const char *password2 = "40800930912";

const char *ssid3 = "ovio";
const char *password3 = "testpass";

const char *ip_brodcast = "192.168.1.255";
char ip_server_ovio[16] = "192.168.1.255";
#endif

byte mac[6];
WiFiUDP Udp;
unsigned int localUdpPort = 6364;					  // local port to listen on
char incomingPacket[255];							  // buffer for incoming packets

//const char *ssid = "Iman";
//const char *password = "12345678";

//const char *ssid = "Netware1";
//const char *password = "40800930912";

/*
unsigned char str_setout[] = "setout";

void str_builder(char *str_out, uint8_t id, uint16_t reg_add, uint16_t reg_data)
{
	memset(str_out, 0, 100);
	sprintf((char *)str_out, "===>stchng;OPG-3510;d49238434-%d;ec:3d:fd:f5:03:69;1.10.0;{\"r%04d\":\"%04x\"}<===", id, reg_add, reg_data);
}

void str_builder_2(char *str_out, int _o1, int _o2, int _o3)
{
	memset(str_out, 0, 100);
	sprintf((char *)str_out, "===>stchng;OPG-3510;d49238434;%02x:%02x:%02x:%02x:%02x:%02x;{\"o1\":\"%d\",\"o2\":\"%d\",\"o3\":\"%d\"}<===", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0], _o1, _o2, _o3);
}

int get_command()
{
	uint8_t eth_packet[100], ctr_eth_packet = 0;
	char str_1[20], str_2[20];
	static int test_ctr = 0;
	ctr_eth_packet = 0;
	
	int packetSize = Udp.parsePacket();
	if (packetSize)
	{
		Serial.println("resid ye gohi !");
		// 	receive incoming UDP packets
		//	Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
		int len = Udp.read(incomingPacket, 255);
		if (len > 0)
		{
			incomingPacket[len] = 0x00;
			Serial.write(incomingPacket);
			str_cpy_by_pos(str_1, incomingPacket, 0, 3);
			str_cpy_by_pos(str_2, incomingPacket, len - 4, len - 1);
			if (str_compare((char *)"===>", str_1) == 0 && str_compare((char *)"<===", str_2) == 0)
			{
				Serial.println("packet level 1 parssing");
				memset(str_1, 0, 20);
				str_cpy_by_pos(str_1, incomingPacket, 4, 9);
				
				if (str_compare((char *)"setout", str_1) == 0)
				{
					Serial.println("packet level 2 parssing");
					eth_cmd_->function_code_ = FUNC_ETH_SETOUT;
					memset(str_1, 0, 20);
					str_cpy_by_pos(str_1, incomingPacket, 11, 13);
					eth_cmd_->device_id_ = (uint8_t)str_to_int(str_1);
					memset(str_1, 0, 20);
					str_cpy_by_pos(str_1, incomingPacket, 16, 19);
					eth_cmd_->address_register_ = (uint16_t)str_to_int(str_1);
					memset(str_1, 0, 20);
					str_cpy_by_pos(str_1, incomingPacket, 21, 24);
					eth_cmd_->data_register_ = (uint16_t)str_hex_to_int(str_1);
					return RES_ETH_RECIEVED;
				}
				else if (str_compare((char *)"config", str_1) == 0)
				{
					eth_cmd_->function_code_ = FUNC_ETH_CONFIG;
					memset(str_1, 0, 20);
					str_cpy_by_pos(str_1, incomingPacket, 11, 13);
					eth_cmd_->device_id_ = (uint8_t)str_to_int(str_1);
					memset(str_1, 0, 20);
					str_cpy_by_pos(str_1, incomingPacket, 16, 19);
					eth_cmd_->address_register_ = (uint16_t)str_to_int(str_1);
					memset(str_1, 0, 20);
					str_cpy_by_pos(str_1, incomingPacket, 21, 24);
					eth_cmd_->data_register_ = (uint16_t)str_hex_to_int(str_1);
					return RES_ETH_RECIEVED;
				}
			}
		}
		return RES_ETH_FAILD;
		// 		send back a reply, to the IP address and port we got the packet from
		//		Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
		//		Udp.write(replyPacket);
		//		Udp.endPacket();
	}
	return RES_ETH_WATING;
}

void command_handl(void)
{
	uint8_t retry;
	int result_get_cmd_eth;
	char packet_for_send[100];
	ethernet_cmd_struct eth_cmd_t;
	result_get_cmd_eth = get_command_eth(&eth_cmd_t);
	if (result_get_cmd_eth == RES_ETH_RECIEVED && eth_cmd_t.device_id_ == 0)//device_id_bus)
	{
		switch (eth_cmd_t.function_code_)
		{
		case FUNC_ETH_SETOUT:

			if(eth_cmd_t.data_register_ & 0x01)
			{
				//digitalWrite(relay_1, HIGH);///////////////
				//digitalWrite(relay_4, LOW);///////////////////////////
				//io1 = 1;/////////////////////////////
			}
			else
			{
				//digitalWrite(relay_1, LOW);///////////////
				//digitalWrite(relay_4, HIGH);///////////////////////////
				//io1 = 0;/////////////////////////
			}

			if(eth_cmd_t.data_register_ & 0x02)
			{
				//digitalWrite(relay_2, HIGH);///////////////
				//io2 = 1;//////////////////////////
			}
			else
			{
				//digitalWrite(relay_2, LOW);
				//io2 = 0;
			}

			if (eth_cmd_t.data_register_ & 0x04)
			{
				//digitalWrite(relay_3, HIGH);//////////////////////
				//io3 = 1;//////////////
			}
			else
			{
				//digitalWrite(relay_3, LOW);////////////
				//io3 = 0;///////////////////////////////
			}

			//str_builder_2(packet_for_send, io1, io2, io3);//////////////
			Udp.beginPacket("192.168.15.255", 6364);
			Udp.write(packet_for_send);
			Udp.endPacket();

			
			//retry = 0;
			//while (1)
			//{
			//	if(relay_control_total(eth_cmd_t.data_register_, eth_cmd_t.device_id_) == PROCESS_OK)
			//		break;
			//	else
			//	{
			//		if(retry > RETRY_NUM)
			//		{
			//			delay(1000);
			//			break;
			//		}
			//		retry++;
			//		delay(200);
			//	}
			//}
			//last_sts_output = eth_cmd_t.data_register_;
			
			break;

		case FUNC_ETH_CONFIG:
			break;
		}
	}
}

*/


void connect_to_wifi(wifi_num _wifi_num)
{
	WiFi.hostname("Ovio Shatoonbala" );
	switch(_wifi_num)
	{
		case _wifi_num_1:
			WiFi.begin(ssid1, password1);
			break;
		case _wifi_num_2:
			WiFi.begin(ssid2, password2);
			break;
		case _wifi_num_3:
			WiFi.begin(ssid3, password3);
			break;
	}
}

sts_wifi check_sts_wifi(void)
{
	if(WiFi.status() == WL_CONNECTED)
		return _sts_wifi_connceted;
	else
		return _sts_wifi_disconnected;
}

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


//WiFi.macAddress(mac);

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "iman_lib.h"

#ifndef SERVER_INTERFACE_DEF
#define SERVER_INTERFACE_DEF

#define RETRY_NUM 3

#define START_ID_SLAVE 11
#define LEN_ID_SLAVE 3

typedef struct
{
	uint8_t function_code_;
	uint8_t device_id_;
	uint16_t address_register_;
	uint16_t data_register_;
}ethernet_cmd_struct;


#define RES_ETH_RECIEVED 0
#define RES_ETH_TIME_OUT 1
#define RES_ETH_FAILD 2
#define RES_ETH_WATING 3

#define FUNC_ETH_SETOUT 0
#define FUNC_ETH_GETSTS 1
#define FUNC_ETH_GETALL 2
#define FUNC_ETH_CONFIG 3

// -------------------------------------------------------------------------------------------------

enum sts_wifi{
	_sts_wifi_connceted,
	_sts_wifi_disconnected
};

enum sts_connect_to_wifi{
	_connect_success,
	_connect_fail
};

enum wifi_num{
	_wifi_num_1,
	_wifi_num_2,
	_wifi_num_3
};

enum sts_packet_udp{
	_sts_packet_faild,
	_sts_packet_success
};



#define NUM_OF_RETRY_TO_CONNECT_WIFI		2
#define TIM_INTERVAL_RETRY_TO_CONNECT		500


void connect_to_wifi(wifi_num _wifi_num);
sts_wifi check_sts_wifi(void);
void udp_start(void);
sts_packet_udp get_packet_udp(char *str);
void send_packet_udp(char *str);

#endif

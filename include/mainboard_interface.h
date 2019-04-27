#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "iman_lib.h"

#ifndef MAINBOARD_INTERFACE_DEF_
#define MAINBOARD_INTERFACE_DEF_

#define TIME_OUT_VALUE 50

enum result_process
{
	PROCESS_OK,
	PROCESS_FAILD
};

enum relay_number
{
	RELAY_0 = 0x01,
	RELAY_1 = 0x02,
	RELAY_2 = 0x04,
	RELAY_3 = 0x08,
	RELAY_4 = 0x10,
	RELAY_5 = 0x20,
	RELAY_6 = 0x40,
	RELAY_7 = 0x80
};

enum input_number
{
	INPUT_0 = 0x01,
	INPUT_1 = 0x02,
	INPUT_2 = 0x04,
	INPUT_3 = 0x08,
	INPUT_4 = 0x10,
	INPUT_5 = 0x20,
	INPUT_6 = 0x40,
	INPUT_7 = 0x80
};


enum STS_RELAY
{
	RELAY_OFF = 0,
	RELAY_ON = 1
};

enum STS_INPUT
{
	INPUT_HIGH = 1,
	INPUT_LOW = 0
};


static inline uint16_t modbus_calcCRC(uint8_t *buf, uint8_t start, uint8_t len);
result_process read_register(uint8_t _address_reg, uint8_t _number_of_reg, uint8_t _id, uint16_t *_reg_16);
result_process relay_control_one(uint8_t _relay_number, uint8_t _sts_relay, uint8_t id);
result_process relay_control_total(uint8_t total_relay_sts, uint8_t id);
change_sts handker_modbus(void);

uint8_t convert_sts_relay_to_reg_modbus(int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8);
change_sts pair_handler(void);
#endif

#include "header_iot.h"

#ifndef MAINBOARD_INTERFACE_DEF_
#define MAINBOARD_INTERFACE_DEF_

#define TIME_OUT_VALUE 50

enum result_process
{
	PROCESS_OK,
	PROCESS_FAILD
};

#define OUTPUT_X 0x01
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

#define INPUT_X 0x01
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
change_sts handler_modbus(data_iot* _data_iot_current, data_iot* _data_iot_new, int handl_mode);
uint8_t convert_sts_relay_to_reg_modbus(data_iot _data_iot);
change_sts pair_handler(void);

#endif

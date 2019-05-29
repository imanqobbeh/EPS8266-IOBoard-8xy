#include "header_iot.h"

#ifndef DEF_CONF_ESP8266
#define DEF_CONF_ESP8266


enum sts_led
{
    _led_on,
    _led_off
};

enum sts_wifi{
	_sts_wifi_connceted,
	_sts_wifi_disconnected
};

enum sts_connect_to_wifi{
	_connect_success,
	_connect_fail
};

enum wifi_num{
	_wifi_num_1_run,
	_wifi_num_2_setup,
	_wifi_num_3_alter_run
};

enum sts_led_handler{
	_led_setup_handler = 0x01,
	_led_connnect_handler = 0x02,
	_led_disconnect_handler = 0x04,
	_led_run_handler = 0x08
};

void init_wifi(wifi_num _wifi_num, data_iot* _data_input);
sts_wifi check_sts_wifi(void);

void init_digital_io(void);
void init_analog_io(void);
void init_uart(void);

double get_uid(void);

void led_blinker(sts_led _sts_led);

#endif

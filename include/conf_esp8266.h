
enum sts_led
{
    _led_on,
    _led_off
};


enum wifi_num{
	_wifi_num_1_run,
	_wifi_num_2_setup,
	_wifi_num_3_alter_run
};

void connect_to_wifi(wifi_num _wifi_num);
void init_wifi(wifi_num _wifi_num);
sts_wifi check_sts_wifi(void);

void init_digital_io(void);
void init_analog_io(void);
void init_uart(void);

double get_uid(void);

void LED_BLINKER(sts_led _sts_led);

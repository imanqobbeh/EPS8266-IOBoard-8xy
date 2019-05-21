#include "conf_esp8266.h"

const char *ssid2_setup = "ovio";
const char *password2_setup = "40800930912";

void init_wifi(wifi_num _wifi_num, data_iot* _data_input)
{
	system_config_data_struct* _system_config_data;
	_system_config_data = _data_input->system_config_data;
	WiFi.hostname("Ovio Shatoonbala");
	switch(_wifi_num)
	{
		case _wifi_num_1_run:
			WiFi.begin(_system_config_data->ssid, _system_config_data->pass);
			break;
		case _wifi_num_2_setup:
			WiFi.begin(ssid2_setup, password2_setup);
			break;
		case _wifi_num_3_alter_run:
			WiFi.begin(_system_config_data->ssid_2, _system_config_data->pass_2);
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

void init_digital_io(void)
{
    pinMode(2, OUTPUT);         // init led on board esp8266 12f module
}

void init_analog_io(void)
{

}

void init_uart(void)
{
	Serial.begin(115200);
	while (!Serial) continue;
}

double get_uid(void)
{
    return ESP.getChipId();
}

void led_blinker(sts_led _sts_led)
{
    switch(_sts_led)
    {
        case _led_on:
            digitalWrite(2, LOW);
            break;
        case _led_off:
            digitalWrite(2, HIGH);
            break;
    }
}
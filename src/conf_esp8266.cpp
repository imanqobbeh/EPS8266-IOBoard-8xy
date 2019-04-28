#include "conf_esp8266.h"


char ssid1_run[30] = "ovio";
char password1_run[30] = "40800930912";

const char *ssid2_setup = "Netware2";
const char *password2_setup = "40800930912";

char ssid3_alter_run[30] = "ovio";
char password_alter_run[30] = "testpass";


void init_wifi(wifi_num _wifi_num)
{
	WiFi.hostname("Ovio Shatoonbala" );
	switch(_wifi_num)
	{
		case _wifi_num_1_run:
			WiFi.begin(ssid1_run, password1_run);
			break;
		case _wifi_num_2_setup:
			WiFi.begin(ssid2_setup, password2_setup);
			break;
		case _wifi_num_3_alter_run:
			WiFi.begin(ssid3_alter_run, password_alter_run);
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
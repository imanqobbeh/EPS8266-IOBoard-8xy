
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


// -------------------------------------------------------------------------------------------------

enum sts_wifi{
	_sts_wifi_connceted,
	_sts_wifi_disconnected
};

enum sts_connect_to_wifi{
	_connect_success,
	_connect_fail
};


enum sts_packet_udp{
	_sts_packet_faild,
	_sts_packet_success
};


enum json_builder_mode
{
	_json_resp_with_cmdid,
	_json_resp_without_cmdid,
	_json_sts_change,
	_json_alive
};

#define NUM_OF_RETRY_TO_CONNECT_WIFI		2
#define TIM_INTERVAL_RETRY_TO_CONNECT		500



void udp_start(void);
sts_packet_udp get_packet_udp(char *str);
void send_packet_udp(char *str);

change_sts handler_wifi(int* _cmdid);
void send_data_to_server(data_iot _data_input, json_builder_mode _Builder_mode);
void json_packet_builder(char *str_out, data_iot _data_input, json_builder_mode _Builder_mode);

#endif

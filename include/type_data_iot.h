#include "header_iot.h"

#ifndef DEF_TYPE_DATA_IOT
#define DEF_TYPE_DATA_IOT.

enum sts_io{
    STS_IO_ON = 1,
    STS_IO_OFF = 0
};

struct data_iot
{
    /* data */
    uint8_t out[36];
    uint8_t input[36];
    uint8_t key[36];
    int cmd_id;
    int temperatur;
    double uid;
};

enum change_sts
{
	_changed,
	_unchanged
};


enum sts_packet
{
    _packet_received_data,
    _packet_received_config,
    _packet_wating,
    _packet_faild
};
void init_data_struct_value(data_iot* data_input);


#endif
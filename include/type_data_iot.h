#include "header_iot.h"

#ifndef DEF_TYPE_DATA_IOT
#define DEF_TYPE_DATA_IOT.

enum sts_io{
    STS_IO_ON = 1,
    STS_IO_OFF = 0
};

enum type_contents
{
    _type_data,
    _type_sysconfig,
    _pair_config,
    _type_modbus_requset,
    _type_irda,
    _type_empty
};

struct modbus_data_struct
{
    uint8_t func_modbus;
    uint16_t address_register;
    uint16_t number_of_register;
    uint16_t a16[100];
};

struct irda_data_struct
{
    int ir_cmdid;
    char a8[100];
};

struct system_config_data_struct
{
    char sts_memory[10];
    char sip[20];
    char ssid[30];
    char pass[30];
    char uniq_id[15];
    char pair_config[100];
    char cip[20];
    char gatway[20];
    char subnet[20];
    char dns1[20];
    char dns2[20];
    char dhcp[10];
    char port_udp[10];
    char ssid_2[30];
    char pass_2[30];
    char key_3des[120];
};

struct pair_config_data_struct
{
    uint8_t pair_type;
    uint8_t io[36];
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
    uint8_t type_contents;
    system_config_data_struct* system_config_data;
    modbus_data_struct* modbus_data;
    irda_data_struct* irda_data;
    pair_config_data_struct* pair_config_data;
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
#include "header_iot.h"

#ifndef _MEMORY_H
#define _MEMORY_H

#define SIZE_OF_EMULATION_EEPROM 512

//  enum key_value
//  {

#define _sts_memory     0
#define _sip            10                  // start address 10 - len 20
#define _ssid           30                  // start address 30 - len 30
#define _pass           60                  // start address 60 - len 30
#define _uniq_id        90                  // start address 90 - len 15
#define _pair_config    105                 // start address 105 - len 40
#define _cip            145                 // start address 145 - len 20
#define _gateway        165                 // start address 165 - len 20
#define _subnet         185                 // start address 185 - len 20
#define _dns1           205                 // start address 205 - len 20
#define _dns2           225                 // start address 225 - len 20
#define _dhcp           235                 // start address 235 - len 10
#define _port_udp       245                 // start address 245 - len 10
#define _ssid_2         255                 // start address 255 - len 30
#define _pass_2         285                 // start address 285 - len 30
#define _key_3des       315                 // start address 375 - len 80

//  };

enum init_memory_mode
{
    _reset_factory,
    _check_inited
};

void read_data_memory(char* str_out, int _key_value);
void write_data_memory(char* str_in, int _key_value);
void init_memory(init_memory_mode _sts_init_memory);

#endif

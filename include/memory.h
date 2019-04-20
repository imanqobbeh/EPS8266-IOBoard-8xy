#ifndef _MEMORY_H
#define _MEMORY_H

#include "EEPROM.h"
#include <Arduino.h>

#define SIZE_OF_EMULATION_EEPROM 512

//enum key_value
//{
#define _sts_memory     0
#define _sip            10                  // start address 10 - len 20
#define _ssid           30                 // start address 30 - len 30
#define _pass           60                  // start address 60 - len 30
#define _uniq_id        90                  // start address 90 - len 15
#define _pair_config    105                 // start address 105 - len 100
#define _cip            205                     // start address 205 - len 20
#define _gateway        225               // start address 225 - len 20
#define _subnet         245                // start address 245 - len 20
#define _dns1           265                  // start address 265 - len 20
#define _dns2           285                 // start address 285 - len 20
#define _dhcp           295                // start address 295 - len 10
#define _port_udp       305            // start address 305 - len 10
#define _ssid_2         315              // start address 315 - len 30
#define _pass_2         345              // start address 345 - len 30
#define _key_3des       375            // start address 365 - len 120   
//};


void read_data_memory(char* str_out, int _key_value);
void write_data_memory(char* str_in, int _key_value);
void init_memory(void);

#endif

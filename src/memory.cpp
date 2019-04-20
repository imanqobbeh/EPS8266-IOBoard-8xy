#include "memory.h"

const char* _sts_eep_inited = "inited";
const char* _sts_eep_free = "free";
const char* _dhcp_on = "dhcp_on";
const char* _dhcp_off = "dhcp_off";

void read_data_memory(char* str_out, int _key_value)
{
    char value;
    int ctr = 0;
    do{
        value = EEPROM.read(_key_value + ctr);
        str_out[ctr] = value;
        ctr++;
    }while(value != 0x00);
    str_out[ctr] = 0x00;
}

void write_data_memory(char* str_in, int _key_value)
{
    int ctr = 0;
    int address_eep = _key_value;
    while(str_in[ctr] != 0)
    {
        EEPROM.write(address_eep, str_in[ctr]);
        address_eep++;
        ctr++;
    }
    EEPROM.write(address_eep, 0);
    
    EEPROM.commit();
}

void init_memory(void)
{
    char sts_eep_ary_tmp[10];
    int ctr = 0;

    EEPROM.begin(SIZE_OF_EMULATION_EEPROM);
    while(1)
    {
        
        for(int ctr = 0; ctr < 7; ctr++)
            sts_eep_ary_tmp[ctr] = EEPROM.read(_sts_memory + ctr);
        sts_eep_ary_tmp[7] = 0;
        if(strcmp(sts_eep_ary_tmp, "inited"))
        {
            write_data_memory("inited", _sts_memory);
            write_data_memory("192.168.1.255", _sip);
            write_data_memory("Netware2", _ssid);
            write_data_memory("40800930912", _pass);
            write_data_memory("free", _uniq_id);
            write_data_memory("free", _pair_config);
            write_data_memory("free", _cip);
            write_data_memory("free", _gateway);
            write_data_memory("free", _subnet);
            write_data_memory("free", _dns1);
            write_data_memory("free", _dns2);
            write_data_memory("dhcp_on", _dhcp);
            write_data_memory("free", _key_3des);
            delay(50);
        }
        else
            break;
    }
}

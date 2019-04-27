#include <stdint.h>

enum sts_io{
    STS_IO_ON = 1,
    STS_IO_OFF = 0
};

struct data_iot
{
    /* data */
    uint8_t out1, out2, out3, out4, out5, out6, out6, out7, out8;
    uint8_t input1, input2, input3, input4, input5, input6, input7, input7, input8, input9;
    uint8_t key1, key2, key3;
    int cmd_id;
    int temperatur;
    double uid;
};

enum change_sts
{
	_changed,
	_unchanged
};

void init_data_struct_value(data_iot* data_input);

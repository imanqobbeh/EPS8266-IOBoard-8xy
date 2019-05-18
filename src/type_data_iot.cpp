#include "type_data_iot.h"

void init_data_struct_value(data_iot* data_input)
{
	data_input->uid = get_uid();
	for(int ctr = 0; ctr < 8; ctr++)
		data_input->out[ctr] = STS_IO_OFF;
	for(int ctr = 0; ctr < 8; ctr++)
		data_input->input[ctr] = STS_IO_OFF;

	data_input->key[0] = STS_IO_OFF;
}
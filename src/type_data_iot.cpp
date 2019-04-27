#include "type_data_iot.h"

void init_data_struct_value(data_iot* data_input);
{
	data_input->uid = get_uid();
	data_input->out1 = STS_IO_OFF;
	data_input->out2 = STS_IO_OFF;
	data_input->out3 = STS_IO_OFF;
	data_input->out4 = STS_IO_OFF;
	data_input->out5 = STS_IO_OFF;
	data_input->out6 = STS_IO_OFF;
	data_input->out7 = STS_IO_OFF;
	data_input->out8 = STS_IO_OFF;
	data_input->input1 = STS_IO_OFF;
	data_input->input2 = STS_IO_OFF;
	data_input->input3 = STS_IO_OFF;
	data_input->input4 = STS_IO_OFF;
	data_input->input5 = STS_IO_OFF;
	data_input->input6 = STS_IO_OFF;
	data_input->input7 = STS_IO_OFF;
	data_input->input8 = STS_IO_OFF;
	data_input->key1 = STS_IO_OFF;
}
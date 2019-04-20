#include "mainboard_interface.h"

/*
* @brief
* This method build packet for send
* relay_control
* 
*/

//uint16_t register_modbus[10];
int out1, out2, out3, out4, out5, out6, out7, out8;
int input1, input2, input3, input4, input5, input6, input7, input8, input9, sensor_pir, temperature;

result_process relay_control_total(uint8_t total_relay_sts, uint8_t _id)
{
	uint8_t send_packet[10];
	uint8_t recieved_packet[10];
	uint32_t time_out = 0;
	uint16_t crc_tmp;
	send_packet[0] = _id;
	send_packet[1] = 0x06;
	send_packet[2] = 0;
	send_packet[3] = 1;
	send_packet[4] = 0;
	send_packet[5] = total_relay_sts;
	crc_tmp = modbus_calcCRC(send_packet, 0, 6);
	send_packet[6] = low_byte(crc_tmp);
	send_packet[7] = high_byte(crc_tmp);

	for (int ctr = 0; ctr < 8; ctr++)
		Serial.write(send_packet[ctr]);

	for (int ctr = 0; ctr < 8;)
	{
		if (Serial.available() > 0)
		{
			recieved_packet[ctr] = Serial.read();
			ctr++;
			time_out = 0;
		}

		delay(1);
		if (time_out < TIME_OUT_VALUE)
			time_out++;
		else
			return PROCESS_FAILD;
	}

	for (int ctr = 0; ctr < 8; ctr++)
	{
		if (recieved_packet[ctr] != send_packet[ctr])
			return PROCESS_FAILD;
	}

	return PROCESS_OK;
}

result_process read_register(uint8_t _address_reg, uint8_t _number_of_reg, uint8_t _id, uint16_t *_reg_16)
{
	uint8_t send_packet[10];
	uint8_t recieved_packet[10];
	uint32_t time_out = 0;
	uint16_t crc_tmp, crc_tmp_2;
	send_packet[0] = _id;
	send_packet[1] = 0x03;
	send_packet[2] = 0;
	send_packet[3] = _address_reg;
	send_packet[4] = 0;
	send_packet[5] = _number_of_reg;

	crc_tmp = modbus_calcCRC(send_packet, 0, 6);

	send_packet[6] = low_byte(crc_tmp);
	send_packet[7] = high_byte(crc_tmp);

	for (int ctr = 0; ctr < 8; ctr++)
		Serial.write(send_packet[ctr]);

	for (int ctr = 0; ctr < (_number_of_reg * 2 + 3 + 2);)
	{
		if (Serial.available() > 0)
		{
			recieved_packet[ctr] = Serial.read();
			ctr++;
			time_out = 0;
		}

		delay(1);
		if (time_out < TIME_OUT_VALUE)
			time_out++;
		else
			return PROCESS_FAILD;
	}

	crc_tmp = modbus_calcCRC(recieved_packet, 0, (_number_of_reg * 2) + 3);
	crc_tmp_2 = merge_byte(recieved_packet[_number_of_reg * 2 + 3], recieved_packet[_number_of_reg * 2 + 3 + 1]);
	if (crc_tmp == crc_tmp_2)
	{
		int ctr_2 = 0;
		for (int ctr = 3; ctr < (_number_of_reg * 2 + 3); ctr += 2)
		{
			_reg_16[ctr_2] = merge_byte(recieved_packet[ctr], recieved_packet[ctr + 1]);
			ctr_2++;
		}
		return PROCESS_OK;
	}
	else
		return PROCESS_FAILD;
	// crc_tmp_2 = merge_byte( , );
}

result_process relay_control_one(uint8_t _relay_number, uint8_t _sts_relay, uint8_t _id)
{
	uint8_t send_packet[10];
	uint8_t recieved_packet[10];
	uint32_t time_out = 0;
	uint16_t crc_tmp;
	send_packet[0] = _id;
	send_packet[1] = 0x05;
	send_packet[2] = 0;

	switch (_relay_number)
	{
	case RELAY_0:
		send_packet[3] = 0;
		break;
	case RELAY_1:
		send_packet[3] = 1;
		break;
	case RELAY_2:
		send_packet[3] = 2;
		break;
	case RELAY_3:
		send_packet[3] = 3;
		break;
	case RELAY_4:
		send_packet[3] = 4;
		break;
	case RELAY_5:
		send_packet[3] = 5;
		break;
	case RELAY_6:
		send_packet[3] = 6;
		break;
	case RELAY_7:
		send_packet[3] = 7;
		break;
	}

	send_packet[4] = _sts_relay;
	send_packet[5] = 0;

	crc_tmp = modbus_calcCRC(send_packet, 0, 6);

	send_packet[6] = low_byte(crc_tmp);
	send_packet[7] = high_byte(crc_tmp);
	
	for (int ctr = 0; ctr < 8; ctr++)
		Serial.write(send_packet[ctr]);

	for (int ctr = 0; ctr < 8;)
	{
		if (Serial.available() > 0)
		{
			recieved_packet[ctr] = Serial.read();
			ctr++;
			time_out = 0;
		}

		delay(1);
		if (time_out < TIME_OUT_VALUE)
			time_out++;
		else
			return PROCESS_FAILD;
	}

	for (int ctr = 0; ctr < 8; ctr++)
	{
		if (recieved_packet[ctr] != send_packet[ctr])
			return PROCESS_FAILD;
	}

	return PROCESS_OK;
}


/*
 * @brief
 * This method calculates CRC
 *
 * @return uint16_t calculated CRC value for the message
 * @ingroup buffer
 */

static inline uint16_t modbus_calcCRC(uint8_t *buf, uint8_t start, uint8_t len)
{
	int crc = 0xFFFF;
	for (int pos = start; pos < start + len; pos++)
	{
		crc ^= (uint16_t)buf[pos & 0xff];
		for (int i = 8; i != 0; i--)
		{
			if ((crc & 0x0001) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
				crc >>= 1;
		}
	}
	return (uint16_t)crc;
}

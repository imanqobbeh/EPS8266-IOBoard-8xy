#include "mainboard_interface.h"
#include "header_iot.h"


uint16_t last_sts_input = 0;
uint16_t last_sts_output = 0;
uint16_t last_temperature = 0;

uint8_t sts_pair_io = 0;

/*
 * @brief
 * This method relay_control_total MODBUS by UART
 *
 * @return result_process relay control total
 */


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

/*
 * @brief
 * This method relay_control_one MODBUS by UART
 *
 * @return result_process relay control 
 */

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
 * This method read_register MODBUS by UART
 *
 * @return result_process read_register 
 */


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


uint8_t convert_sts_relay_to_reg_modbus(int _o1, int _o2, int _o3, int _o4, int _o5, int _o6, int _o7, int _o8)
{
	uint8 reg_data = 0;

	if(_o1 == RELAY_ON)
		reg_data |= 0x01;
	
	if(_o2 == RELAY_ON)
		reg_data |= 0x02;
	
	if(_o3 == RELAY_ON)
		reg_data |= 0x04;

	if(_o4 == RELAY_ON)
		reg_data |= 0x08;

	if(_o5 == RELAY_ON)
		reg_data |= 0x10;

	if(_o6 == RELAY_ON)
		reg_data |= 0x20;

	if(_o7 == RELAY_ON)
		reg_data |= 0x40;

	if(_o8 == RELAY_ON)
		reg_data |= 0x80;
	
	return reg_data;
}

change_sts handker_modbus(void)
{
	uint16_t register_modbus[10];
	
    change_sts _change_sts = _unchanged, _change_sts_input = _unchanged;
	if (read_register(0, 2, 0x10, register_modbus) == PROCESS_OK)
	{
		if (register_modbus[0] != last_sts_input)
		{	
			if(/*sts_pair_io*/0)
			{
				uint16_t tmp_last_sts_input;
				tmp_last_sts_input = register_modbus[0];
				tmp_last_sts_input ^= last_sts_input;
				
				if((tmp_last_sts_input & 0x01) == 0x01)
				{
					if(last_out1 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o1_on ");
						#endif
						last_out1 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o1_off ");
						#endif
						last_out1 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if((tmp_last_sts_input & 0x02) == 0x02)
				{
					if(last_out2 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o2_on ");
						#endif
						last_out2 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o2_off ");
						#endif
						last_out2 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if((tmp_last_sts_input & 0x04) == 0x04)
				{
					if(last_out3 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o3_on ");
						#endif
						last_out3 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o3_off ");
						#endif
						last_out3 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if((tmp_last_sts_input & 0x08) == 0x08)
				{
					if(last_out4 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o4_on ");
						#endif
						last_out4 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o4_off ");
						#endif
						last_out4 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x10) == 0x10)
				{
					if(last_out5 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o5_on ");
						#endif
						last_out5 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o5_off ");
						#endif
						last_out5 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x20) == 0x20)
				{
					if(last_out6 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o6_on ");
						#endif
						last_out6 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o6_off ");
						#endif
						last_out6 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x40) == 0x40)
				{
					if(last_out7 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o7_on ");
						#endif
						last_out7 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o7_off ");
						#endif
						last_out7 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}
				if((tmp_last_sts_input & 0x80) == 0x80)
				{
					if(last_out8 == RELAY_OFF)
					{
						#ifdef _DEBUG_CODE
						Serial.print("o8_on ");
						#endif
						last_out8 = RELAY_ON;
					}
					else
					{	
						#ifdef _DEBUG_CODE
						Serial.print("o8_off ");
						#endif
						last_out8 = RELAY_OFF;
					}
					_change_sts_input = _changed;
				}

				if(_change_sts_input == _changed)
				{
					//relay_control_total(0x06, 0x10);
					int retry = 0;
					uint8_t relay_data_register;
					relay_data_register = convert_sts_relay_to_reg_modbus(last_out1, last_out2, last_out3, last_out4, last_out5, last_out6, last_out7, last_out8);
					while (1)
					{

						if (relay_control_total(relay_data_register, 0x10) == PROCESS_OK)
						{
							out1 = last_out1;
							out2 = last_out2;
							out3 = last_out3;
							out4 = last_out4;
							out5 = last_out5;
							out6 = last_out6;
							out7 = last_out7;
							out8 = last_out8;

							break;
						}
						else
						{
							if (retry > RETRY_NUM)
							{
								delay(1000);
								break;
							}
							retry++;
							delay(200);
						}
					}
				}
				///

			}

			last_sts_input = register_modbus[0];
			if((last_sts_input & 0x01) == 0x01)
				input1 = INPUT_HIGH;
			else
				input1 = INPUT_LOW;

			if((last_sts_input & 0x02) == 0x02)
				input2 = INPUT_HIGH;
			else
				input2 = INPUT_LOW;

			if((last_sts_input & 0x04) == 0x04)
				input3 = INPUT_HIGH;
			else
				input3 = INPUT_LOW;

			if((last_sts_input & 0x08) == 0x08)
				input4 = INPUT_HIGH;
			else
				input4 = INPUT_LOW;

			if((last_sts_input & 0x10) == 0x10)
				input5 = INPUT_HIGH;
			else
				input5 = INPUT_LOW;

			if((last_sts_input & 0x20) == 0x20)
				input6 = INPUT_HIGH;
			else
				input6 = INPUT_LOW;

			if((last_sts_input & 0x40) == 0x40)
				input7 = INPUT_HIGH;
			else
				input7 = INPUT_LOW;

			if((last_sts_input & 0x80) == 0x80)
				input8 = INPUT_HIGH;
			else
				input8 = INPUT_LOW;

			if((last_sts_input & 0x0100) == 0x0100)
				input9 = INPUT_HIGH;
			else
				input9 = INPUT_LOW;

			_change_sts = _changed;
		}
	}
	
	uint16_t tmp;
	if(register_modbus[2] >= last_temperature)
		tmp = register_modbus[2] - last_temperature;
	else
		tmp = last_temperature - register_modbus[2];
	if(tmp >= 60)
	{
		last_temperature = register_modbus[2];
		temperature = last_temperature;
		_change_sts = _changed;
	}
	return _change_sts;
}

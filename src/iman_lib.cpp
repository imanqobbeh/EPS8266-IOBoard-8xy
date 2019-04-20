
#include "iman_lib.h"

void str_cpy_by_pos(char *str_1, char *str_2, int start_pos, int end_pos)
{
	int ctr = 0;
	for (int ptr = start_pos; ptr <= end_pos; ptr++, ctr++)
		str_1[ctr] = str_2[ptr];
	str_1[ctr] = 0x00;
}

int str_to_int(char *str)
{
	return atoi((const char *)str);
}

int str_hex_to_int(char *str)
{
	int intVal;
	sscanf((const char *)str, "%x", &intVal);

	return intVal;
}

int str_compare(char *str_1, char *str_2)
{
	return strcmp((const char *)str_1, (const char *)str_2);
}

uint8_t high_byte(uint16_t input_data)
{
	return (uint8_t)(input_data >> 8);
}

uint8_t low_byte(uint16_t input_data)
{
	return (uint8_t)(input_data & 0x00ff);
}

uint16_t merge_byte(uint8_t high_byte, uint8_t low_byte)
{
	uint16_t tmp_merge_word = 0;
	tmp_merge_word = (uint16_t)high_byte;
	tmp_merge_word <<= 8;
	tmp_merge_word |= (uint16_t)low_byte & 0x00ff;
	return tmp_merge_word;
}

uint16_t byte_to_bcd_word(uint8_t byte_input)
{
	uint16_t tmp_bcd_word;
	uint8_t tmp_ascii;
	tmp_ascii = byte_input & 0xf0;
	tmp_ascii >>= 4;
	tmp_ascii += 0x30;
	tmp_bcd_word = (uint16_t)tmp_ascii;
	tmp_bcd_word <<= 8;
	tmp_ascii = byte_input & 0x0f;
	tmp_ascii += 0x30;
	tmp_bcd_word |= (uint16_t)tmp_ascii;
	return tmp_bcd_word;
}




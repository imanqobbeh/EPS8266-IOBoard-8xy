#include <Arduino.h>

#ifndef IMAN_LIB_DEF
#define IMAN_LIB_DEF

uint16_t byte_to_bcd_word(uint8_t byte_input);
uint16_t merge_byte(uint8_t high_byte, uint8_t low_byte);
uint8_t low_byte(uint16_t input_data);
uint8_t high_byte(uint16_t input_data);
int str_to_int(char *str);
void str_cpy_by_pos(char *str_1, char *str_2, int start_pos, int end_pos);
int str_compare(char *str_1, char *str_2);
int str_hex_to_int(char *str);

#endif
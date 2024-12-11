/*
 * lcd_lib.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Kuliev
 */

#ifndef INC_LCD_LIB_H_
#define INC_LCD_LIB_H_
#include "main.h"
#define CURSOR_ON 0x0F
#define CURSOR_OFF 0x0C
uint8_t DEVICE_ADDRESS;
I2C_HandleTypeDef *halI2C;
uint8_t cursor;
uint8_t backlightReg[4];
uint8_t backlightStatus;
void send_command(char command);
void send_data(char _data);
void lcd_begin(I2C_HandleTypeDef *hi2c1, uint8_t address);
void lcd_init();
void createChar(uint8_t *arr, uint8_t location);
uint8_t lcd_ready();
void cursorOn();
void cursorOff();
void backlight();
void noBacklight();
void lcd_clear();
void lcd_home();
void lcd_print_String(char *str);
void lcd_print_Integer(int num);
void lcd_print_Long(long num);
void lcd_print_Float(float num);
void lcd_print_Char(uint8_t address);
void set_Cursor(int x, int y);


#endif /* INC_LCD_LIB_H_ */

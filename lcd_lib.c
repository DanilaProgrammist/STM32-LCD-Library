/*
 * lcd_lib.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Kuliev
 */

#include "lcd_lib.h"
#include "stdio.h"
void send_command(char command){//rs = 0
	uint8_t low, high;
	uint8_t data[4];
	high = command & 0xF0;
	low = (command << 4) & 0xF0;
//	if (backlightStatus){
//		data[0] |= (high|0x0C) | (1 << 3);
//		data[1] |= (high|0x08) | (1 << 3);
//		data[2] |= (high|0x0C) | (1 << 3);
//		data[3] |= (high|0x08) | (1 << 3);
//	}
//	else {
//		data[0] |= (high|0x0D) & ~(1 << 3);
//		data[1] |= (high|0x09) & ~(1 << 3);
//		data[2] |= (high|0x0D) & ~(1 << 3);
//		data[3] |= (high|0x09) & ~(1 << 3);
//	}
	data[0] = high|backlightReg[0];
	data[1] = high|backlightReg[1];
	data[2] = low|backlightReg[0];
	data[3] = low|backlightReg[1];
	HAL_I2C_Master_Transmit(halI2C, DEVICE_ADDRESS, (uint8_t*)data, sizeof(data), 100);
}
void send_data(char _data){//rs = 1
	uint8_t low, high;
	uint8_t data[4];
	high = _data & 0xF0;
	low = (_data << 4) & 0xF0;
//	if (backlightStatus){
//		data[0] |= (high|0x0D) | (1 << 3);
//		data[1] |= (high|0x09) | (1 << 3);
//		data[2] |= (high|0x0D) | (1 << 3);
//		data[3] |= (high|0x09) | (1 << 3);
//	}
//	else {
//		data[0] |= (high|0x0D) & ~(1 << 3);
//		data[1] |= (high|0x09) & ~(1 << 3);
//		data[2] |= (high|0x0D) & ~(1 << 3);
//		data[3] |= (high|0x09) & ~(1 << 3);
//	}
	data[0] = (high|backlightReg[2]);
	data[1] = high|backlightReg[3];
	data[2] = (low|backlightReg[2]);
	data[3] = low|backlightReg[3];
	HAL_I2C_Master_Transmit(halI2C, DEVICE_ADDRESS , (uint8_t*)data, sizeof(data), 100);
}
void lcd_begin(I2C_HandleTypeDef *hi2c1, uint8_t address){
	DEVICE_ADDRESS = address << 1;
	halI2C = hi2c1;
	cursor = CURSOR_OFF;
	backlightStatus = 1;
	backlightReg[0] = 0x0C;
	backlightReg[1] = 0x08;
	backlightReg[2] = 0x0D;
	backlightReg[3] = 0x09;
}
void createChar(uint8_t *arr, uint8_t location){
    send_command(0x40 | (location << 3));
    for(uint8_t line = 0; line < 8; line++) {
  	  send_data(*(arr+line));
    }
    //send_command(0b11000000);0xC0
}
uint8_t lcd_ready(){
	if (HAL_I2C_IsDeviceReady(halI2C, DEVICE_ADDRESS, 5, 100)){
		return 1;
	}
	else {
		return 0;
	}
}
void cursorOn(){
	cursor = CURSOR_ON;
	send_command(cursor);
}
void cursorOff(){

	cursor = CURSOR_OFF;
	send_command(cursor);
}
void backlight(){
	backlightStatus = 1;
	backlightReg[0] = 0x0C;
	backlightReg[1] = 0x08;
	backlightReg[2] = 0x0D;
	backlightReg[3] = 0x09;
	backlightStatus = 1;
	send_command((cursor ? CURSOR_ON : CURSOR_OFF));
}
void noBacklight(){
	backlightStatus = 0;
	backlightReg[0] = 0x04;
	backlightReg[1] = 0x00;
	backlightReg[2] = 0x05;
	backlightReg[3] = 0x01;
	backlightStatus = 0;
	send_command((cursor ? CURSOR_ON : CURSOR_OFF));
}
void lcd_clear(){
	send_command(0x01);
	HAL_Delay(5);
}
void lcd_home(){
	set_Cursor(0, 0);
}
void lcd_init(){
	HAL_Delay(50);
	send_command(0x30);
	HAL_Delay(5);
	send_command(0x30);
	HAL_Delay(1);
	send_command(0x30);
	HAL_Delay(10);
	send_command(0x20);
	HAL_Delay(10);

	send_command(0x28);
	HAL_Delay(1);
	send_command(0x08);
	HAL_Delay(1);
	send_command(0x01);
	HAL_Delay(2);
	send_command(0x06);
	HAL_Delay(1);
	send_command(0x0C);
}
void lcd_print_String(char *str){
	while(*str){
		send_data(*str++);
	}
}
void lcd_print_Integer(int num){
	char str[5];
	sprintf(str, "%d", num);
	lcd_print_String(str);
}
void lcd_print_Long(long num){
	char str[10];
	sprintf(str, "%ld", num);
	lcd_print_String(str);
}
void lcd_print_Float(float num){
	char str[7];
	sprintf(str, "%.2f", num);
	lcd_print_String(str);
}
void lcd_print_Char(uint8_t address){
	send_data((char)address);
}
void set_Cursor(int x, int y){
	switch(x){
	case 0:
		y |= 0x80;
		break;
	case 1:
		y |= 0xC0;
		break;
	case 2:
		y |= 0x94;
		break;
	case 3:
		y |= 0xD4;
		break;

	}
	send_command(y);
}

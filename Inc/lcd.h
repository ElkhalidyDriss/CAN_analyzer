/*
 * lcd.h
 * Driver for  HD44780U lcd display controller.
 * Datasheet Link : https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 *  Created on: Apr 22, 2024
 *      Author: Idriss Elkhalidy
 *
 */

#ifndef LCD_H_
#define LCD_H_
#include "stm32f103xb.h"

/****Pinouts definition****/



/*************************/

/**************************Commands for HD44780U**************************/
#define DISPLAY_CLR 0x1 /*Clears entire display and sets DDRAM address 0 in address counter.*/
#define CURSOR_INC  0x6 /*Increment cursor (shift cursor to right)*/
#define CURSOR_DEC  0x4 /*Decrement cursor (shift cursor to left)*/
#define FUNCTION_SET 0x20 /*defaults to 4bits data length , 1 line 5 x 8 dots*/

/****************************************************************************/
/**********Function Set flags*********/
#define DL_4_BITS (0) /*4 bits data length*/
#define DL_8_BITS (1 << 4) /*8 bits data length*/
#define TWO_LINES (1 << 3) /*Number of lines in the display is two*/
#define ONE_LINE (0)      /*Number of lines in the display is one*/
#define DOTS_5x10 (1 << 2) /*Font is 5×10 dots*/
#define DOTS_5x8 (0) /*Font is 5x8 dots*/
/***********************************/


void lcd_init();
void lcd_send_char(uint8_t data);
void lcd_send_string(uint8_t *data);
void lcd_send_cmd(uint8_t cmd);
uint32_t lcd_read_busy_flag();

#endif /* LCD_H_ */

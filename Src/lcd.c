/*
 * lcd.c
 *
 *  Created on: Apr 22, 2024
 *      Author: Idriss Elkhalidy
 */
#include "lcd.h"
#include "stm32f103xb.h"
#define EN  GPIO_ODR_ODR6 /*Pin 6 of PortA*/
#define RW  GPIO_ODR_ODR5 /*Pin 5 of PortA*/
#define RS  GPIO_ODR_ODR4 /*Pin 4 of PortA*/
#define DATA_4_BITS (GPIO_ODR_ODR3 | GPIO_ODR_ODR2 | GPIO_ODR_ODR1 | GPIO_ODR_ODR0)




void lcd_init(){
	/*Enable Clock for PortA*/
    RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
	/*Set GPIOA (PIN 0 to PIN 6 as output push pull with 2Mhz max speed*/
    GPIOA -> CRL |= 0x02222222;
    /**/


}

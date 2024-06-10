/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Elkhalidy Driss
 * @brief          : Main program
 ******************************************************************************
**/


#include <stdint.h>
#include <stdio.h>
#include "stm32f103xb.h"

can_header_t *can_header ;
uint8_t can_rx_data[8];

void USB_LP_CAN_RX0_IRQHandler(void); /*CAN RX Interrupt request handler override */

int main(void)
{
	can_init();
	while(1);
}

void USB_LP_CAN_RX0_IRQHandler(void){
	can_readFIFO0(can_header, can_rx_data);

}

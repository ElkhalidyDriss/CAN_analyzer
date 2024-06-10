/*
 * can.h
 *
 *  Created on: Apr 19, 2024
 *      Author: Idriss
 */

#ifndef CAN_H_
#define CAN_H_
#define CAN_BTR_CONFIG_Msk (CAN_BTR_BRP_Msk | CAN_BTR_TS1_Msk | CAN_BTR_TS2_Msk | CAN_BTR_SJW_Msk | CAN_BTR_LBKM_Msk | CAN_BTR_SILM_Msk)
#define CAN_BTR_CONFIG_Val (0x812F0002) /*Prescaler=2 , TS1=15 , TS2=2 , silent mode enabled , loopback mode disabled*/
#define GPIO_CNF_AF_PL (0x2) /*Alternate function output Push-pull*/
#define GPIO_MODE_OUTPUT_2MHZ (0x2)  /*Output mode, max speed 2 MHz.*/

/*CAN message header */
typedef struct {
	uint32_t stdID : 11; /*Standard identifier or MSBs of the extended identifier depending on the IDE value*/
	uint32_t extID : 18; /*LSBs of the extended identifier*/
	uint32_t IDE   : 1; /*Identifier extension , this bit defines the identifier type of message in the mailbox*/
	uint32_t RTR   : 1; /*Remote Transmission request*/
	uint32_t  : 0; //telling the compiler to start packing in a new integer variable
	uint32_t time_stamp : 16; /*16-bit timer value captured at the SOF detection*/
	uint32_t filter_match_index  : 8; /*Index of the filter the message stored in the mailbox passed through*/
	uint32_t data_length : 4; /*number of data bytes  data frame contains(0 to 8)*/
}can_header_t;


void can_init(void);
void can_readFIFO0(can_header_t *can_header , uint8_t *rx_data);

#endif /* CAN_H_ */

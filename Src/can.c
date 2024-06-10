/*
 * can.c
 *
 *  Created on: Apr 19, 2024
 *      Author: Idriss Elkhalidy
 */
#include "can.h"
#include "stm32f103xb.h"
void can_init(void){
	/*Enabling the clock for CAN*/
	RCC -> APB1ENR &= ~(RCC_APB1ENR_CAN1EN_Msk);
	RCC -> APB1ENR |= (RCC_APB1ENR_CAN1EN_Msk);
	/*Enabling the clock for PortA*/
	RCC -> APB2ENR &= ~(RCC_APB2ENR_IOPAEN_Msk);
	RCC -> APB2ENR |= (RCC_APB2ENR_IOPAEN_Msk);
    /*Configuring PA12(CAN_RX) as alternate function push pull*/
	GPIOA -> CRH &= ~(GPIO_CRH_MODE12_Msk | GPIO_CRH_CNF12_Msk);
	GPIOA -> CRH |= ((GPIO_MODE_OUTPUT_2MHZ << GPIO_CRH_MODE12_Pos) | (GPIO_CNF_AF_PL << GPIO_CRH_CNF12_Pos));
	/*!PA11 should be configured as input floating , which is exactly its state after reset ; no need to reconfigure it*/
	/*Alternate function remapping for CAN */
	AFIO -> MAPR &= ~(AFIO_MAPR_CAN_REMAP_Msk); /*0x00  CAN_RX mapped to PA11, CAN_TX mapped to PA12 */
	/**********************Configuration of bxCAN**************************/
	/*Entering CAN initialization mode*/
	CAN1 -> MCR &= ~(CAN_MCR_INRQ_Msk);
	CAN1 -> MCR |= CAN_MCR_INRQ;
	while((CAN1->MSR & CAN_MSR_INAK_Msk) != CAN_MSR_INAK); /*Waiting for hardware to enter initialization mode*/
    CAN1 -> BTR &= (CAN_BTR_CONFIG_Msk);
    CAN1 -> BTR |= CAN_BTR_CONFIG_Val; /*Prescaler=2 , TS1=15 , TS2=2 , silent mode enabled , loopback mode disabled*/
    /*Filter Configuration*/
    CAN1 -> FMR &= ~(CAN_FMR_CAN2SB);
    CAN1 -> FMR |= (28 << CAN_FMR_CAN2SB_Pos); /*When CAN2SB[5:0] = 28d, all the filters to CAN1 can be used , no need to CAN2 for this project*/
    CAN1 -> FMR |= CAN_FMR_FINIT; /*filter initialization mode by setting FINIT(filter init) bit*/
    CAN1 -> FM1R &= ~(CAN_FM1R_FBM0); /*Two 32-bit registers of filter bank 0 are in Identifier Mask mode.*/
    CAN1 -> FS1R |= (CAN_FS1R_FSC0); /* Single 32-bit scale configuration*/
    CAN1 -> FFA1R &= ~(CAN_FFA1R_FFA0); /*Filter 0 assigned to FIFO 0*/
    CAN1 -> FA1R |= CAN_FA1R_FACT0; /*filter 0 is active*/
    /*Accept all messages*/
    CAN1 -> sFilterRegister[0].FR1 = 0;
    CAN1 -> sFilterRegister[0].FR2 = 0;
    CAN1 -> FMR &= ~(CAN_FMR_FINIT); /*Exiting filter initialization mode*/
    /*Enabling interrupts for CAN message reception*/
    __disable_irq();
    CAN1 -> IER &= ~(CAN_IER_FMPIE0_Msk);
    CAN1 -> IER |= CAN_IER_FMPIE0; /*enabling interrupts when a message arrives to FIFO0*/
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    __enable_irq();
    CAN1 -> MCR &=  ~(CAN_MCR_INRQ); /*Enabling normal mode*/
    while((CAN1 -> MSR & CAN_MSR_INAK_Msk) == CAN_MSR_INAK); /*Waiting for hardware to enter normal mode*/
}
void can_readFIFO0(can_header_t *can_header , uint8_t *rx_data){
	  /* Reading can header data */
      can_header -> stdID = ((CAN1 -> sFIFOMailBox[0].RIR) & CAN_RI0R_STID_Msk);
      can_header -> extID = ((CAN1 -> sFIFOMailBox[0].RIR) & CAN_RI0R_EXID_Msk);
      can_header -> IDE   = ((CAN1 -> sFIFOMailBox[0].RIR) & CAN_RI0R_IDE_Msk);
      can_header -> RTR   = ((CAN1 -> sFIFOMailBox[0].RIR) & CAN_RI0R_RTR_Msk);
      can_header -> time_stamp = ((CAN1 -> sFIFOMailBox[0].RDTR) & CAN_RDT0R_TIME_Msk);
      can_header -> filter_match_index = ((CAN1 -> sFIFOMailBox[0].RDTR) & CAN_RDT0R_FMI_Msk);
      can_header -> data_length = ((CAN1 -> sFIFOMailBox[0].RDTR) & CAN_RDT0R_DLC_Msk);
      /* Reading data field */
      rx_data[0] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDLR) & CAN_RDL0R_DATA0 ) >> CAN_RDL0R_DATA0_Pos );
      rx_data[1] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDLR) & CAN_RDL0R_DATA1 ) >> CAN_RDL0R_DATA1_Pos );
      rx_data[2] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDLR) & CAN_RDL0R_DATA2 ) >> CAN_RDL0R_DATA2_Pos );
      rx_data[3] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDLR) & CAN_RDL0R_DATA3 ) >> CAN_RDL0R_DATA3_Pos );
      rx_data[4] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDHR) & CAN_RDH0R_DATA4 ) >> CAN_RDH0R_DATA4_Pos );
      rx_data[5] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDHR) & CAN_RDH0R_DATA5 ) >> CAN_RDH0R_DATA5_Pos );
      rx_data[6] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDHR) & CAN_RDH0R_DATA6 ) >> CAN_RDH0R_DATA6_Pos );
      rx_data[7] = (uint8_t)(((CAN1 -> sFIFOMailBox[0].RDHR) & CAN_RDH0R_DATA7 ) >> CAN_RDH0R_DATA7_Pos );
      /*Releasing FIFO0 after reading*/
      CAN1 -> RF0R |= CAN_RF0R_RFOM0 ; /*Set RFOM0 bit to release FIFO0*/

}


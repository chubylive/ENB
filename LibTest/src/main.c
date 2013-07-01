/*
 ===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "Drivers/lpc17xx_uart.h"

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP;

// TODO: insert other include files here

// TODO: insert other definitions and declarations here
volatile uint32_t UART1Status;
volatile uint8_t UART1TxEmpty = 1;
volatile uint8_t UART1Buffer[UART_BUFSIZE];
volatile uint32_t UART1Count = 0;
static UART_CFG_Type UART_CFG_1152;
static UART_FIFO_CFG_Type FIFO_DEFAULT;
uint8_t test[6]= {104,101,108,108,111,10};



/*******************************************************************************
 ** Function name:			UART_IRQHandler
 **
 ** Description:			This interrupt handler implements an interrupt
 ** 						driver UART read and write. By check the appropriate
 ** 						bit in the IIR register it can tell if the read buffer
 ** 						is empty and read from it and like wise if the transmit
 ** 						buffer is empty and allow which will allow the transmit
 ** 						function to send bytes.
 **
 ** Parameter: 				None
 ** Returned Value: 		None
 ******************************************************************************/

void UART1_IRQHandler(void) {
	uint8_t IIRValue, LSRValue;
	uint8_t Dummy = Dummy;

	IIRValue = LPC_UART1 ->IIR;
	IIRValue >>= 1; /* skip pending bit in IIR */
	IIRValue <<= 1; /*preserve first bits place*/
	IIRValue &= 0x07; /* check bit 1~3, interrupt identification */

	if (IIRValue == UART_IIR_INTID_RLS ) /* Receive Line Status */
	{
		LSRValue = LPC_UART1 ->LSR; /* Receive Line Status */

		/*If the cause of the interrupt was an error exit interrupt routine*/
		if (LSRValue & (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_RXFE | UART_LSR_BI )) {
			/* There are errors or break interrupt */
			/* Read LSR will clear the interrupt */
			UART1Status = LSRValue;
			Dummy = LPC_UART1 ->RBR; /* Dummy read on RX to clear interrupt, then bail out */
			return;
		}
		if (LSRValue & UART_LSR_RDR ) /* Receive Data Ready */
		{
			/* If no error on RLS, normal ready, save into the data buffer. */
			/* Note: read RBR will clear the interrupt */

			UART1Buffer[UART1Count] = UART_ReceiveByte((LPC_UART_TypeDef *) LPC_UART1);
			UART1Count++;
			if (UART1Count == UART_BUFSIZE) {
				UART1Count = 0; /* buffer overflow */
			}
		}
	} else if (IIRValue == UART_IIR_INTID_RDA ) /* Receive Data Available */
	{
		/* Receive Data Available */
		UART1Buffer[UART1Count] = UART_ReceiveByte((LPC_UART_TypeDef *) LPC_UART1);
		UART1Count++;
		if (UART1Count == UART_BUFSIZE) {
			UART1Count = 0; /* buffer overflow */
		}
	} else if (IIRValue == UART_IIR_INTID_CTI ) /* Character timeout indicator */
	{
		/* Character Time-out indicator */
		UART1Status |= 0x100; /* Bit 9 as the CTI error */
	}
}

/*****************************************************************************
 ** Function name:		UARTSend
 **
 ** Descriptions:		Send a block of data to the UART 0 port based
 **						on the data length. This function is interrupt
 **						driven as opposed to the other UART send function.
 **
 ** parameters:			portNum, buffer pointer, and data length
 ** Returned value:		None
 **
 *****************************************************************************/

void UART1_Send(uint8_t *BufferPtr, uint32_t length) {
	while (length != 0) {
		/* THRE status, contain valid data */
		while (!(UART1TxEmpty & 0x01));
		LPC_UART1 ->THR = *BufferPtr;
		UART1TxEmpty = 0; /* not empty in the THR until it shifts out */
		BufferPtr++;
		length--;
	}
}


int main(void) {
	//setup UART configuration struct
	UART_CFG_1152.Baud_rate = 115200;
	UART_CFG_1152.Databits = UART_DATABIT_8;
	UART_CFG_1152.Parity = UART_PARITY_NONE;
	UART_CFG_1152.Stopbits = UART_STOPBIT_1;

	//initialize Uart1
	UART_Init((LPC_UART_TypeDef *)LPC_UART1, &UART_CFG_1152);

	//setup fifo

	UART_FIFOConfigStructInit(&FIFO_DEFAULT);
	UART_FIFOConfig((LPC_UART_TypeDef *) LPC_UART1, &FIFO_DEFAULT);
	//enable respective interrupts

	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_THRE, ENABLE);
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_INTCFG_RLS, ENABLE);
	NVIC_EnableIRQ(UART1_IRQn);
	UART_TxCmd((LPC_UART_TypeDef *) LPC_UART1, ENABLE);

	// Enter an infinite loop, just incrementing a counter
	while (1) {


		if ( UART1Count != 0 )
		{

		//	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_IER_RBRINT_EN, DISABLE);
			  UART_Send((LPC_UART_TypeDef*) LPC_UART1,(uint8_t *)UART1Buffer, UART1Count, BLOCKING);
			  UART1Count = 0;

			//	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART1, UART_IER_RBRINT_EN, ENABLE);


		}
	}
	return 0;
}

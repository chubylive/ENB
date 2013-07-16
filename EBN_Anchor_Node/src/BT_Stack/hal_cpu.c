/* this is give the added functionality of put the LPC in low power mode
 * this is probably not going to be used a lot as we might want to be
 * constantly sending advertisements
 */


#include "includes/hal_cpu.h"
#include "LPC17xx.h"
#include "Drivers/includes/lpc17xx_clkpwr.h"
/*
 *
 */
uint8_t UART1IRQ_ON = 0;

void hal_cpu_enable_irqs(){
	NVIC_EnableIRQ(UART1_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
	//NVIC_EnableIRQ(DMA_IRQn);
  //  NVIC_EnableIRQ(ADC_IRQn);

}

void hal_cpu_disable_irqs(){
	/*NVIC_DisableIRQ(UART1_IRQn);
	NVIC_DisableIRQ(TIMER0_IRQn);
	NVIC_DisableIRQ(DMA_IRQn);
*/
}

void hal_cpu_set_uart_needed_during_sleep(uint8_t enabled){
	/*if(enabled){
		UART1IRQ_ON = 1;
		NVIC_EnableIRQ(UART1_IRQn);
	}else{
		UART1IRQ_ON = 0;
		//NVIC_DisableIRQ(UART1_IRQn);
	}*/
}
/*the interrupt from uart is expected to bring MCU out of sleep mode*/
void hal_cpu_enable_irqs_and_sleep(){
	/*if (UART1IRQ_ON){
		NVIC_EnableIRQ(UART1_IRQn);

		CLKPWR_Sleep();
	}else{
		NVIC_EnableIRQ(UART1_IRQn);
		CLKPWR_Sleep();
	}*/
}

void clock_init(){

}

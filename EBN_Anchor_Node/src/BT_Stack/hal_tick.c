/*This is the hardware abstraction layer for the timers
 *for the sake of simplicity I will be using an assumed clock rate of 100mhz
 *it will be divided by 2 and to produce to countdown of a 1/4 sec the
 *the TIMER_COUNTDOWN would be set to 3125000
 *
 */

#include "Drivers/includes/lpc17xx_timer.h"
#include "includes/hal_tick.h"
#include <stdlib.h>

static void dummy_handler(void){};
static void (*tick_handler)(void) = &dummy_handler;
static TIM_TIMERCFG_Type TTC0;
static TIM_MATCHCFG_Type MATC0;


#define TIMER_COUNTDOWN 2937500
//assume clock rate of 100mhz
//after timer0 division(by 4) clock rate is 25mhz
/*
 * from what i understand the msp430 has a capture and compare scheme for dealing with time
 * the implementation on the HAL_TICK seems to be something like counting up to 8192 (timer_countdwn) and
 * generating an interrupt each time it hits the number. On the LPC we can achieve this by using the mat registers.
 * by setting the interrupt for every quarter second we should have something similar. It would also seem that they
 * send the msp430 to sleep at the end of each interrupt.
 */

 /* the tick period is close to 500ms. this is an approximation i am yet to test. I will get better clocking later
 */

void hal_tick_init(void){
	//initialize timer struct
	TTC0.PrescaleOption = TIM_PRESCALE_TICKVAL;
	TTC0.PrescaleValue = 1;

	//intialize match channel struct
	MATC0.MatchChannel = 0;
	MATC0.IntOnMatch = ENABLE;
	MATC0.StopOnMatch = DISABLE;
	MATC0.ResetOnMatch = ENABLE;
	MATC0.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	MATC0.MatchValue = TIMER_COUNTDOWN; // this should give approximately 1/4 of a sec

	//initialize TIMER0
	TIM_Init((LPC_TIM_TypeDef *) LPC_TIM0, TIM_TIMER_MODE, &TTC0);

	//initialize MAT0
	 TIM_ConfigMatch((LPC_TIM_TypeDef *) LPC_TIM0, &MATC0);

	//reset Timer0
	TIM_ResetCounter((LPC_TIM_TypeDef *) LPC_TIM0);

	//enable Timer0
	TIM_Cmd((LPC_TIM_TypeDef *) LPC_TIM0, ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);

}

void hal_tick_set_handler(void (*handler)(void)){
	if (handler == NULL){
        tick_handler = &dummy_handler;
        return;
    }
    tick_handler = handler;

}


int  hal_tick_get_tick_period_in_ms(void){
	//250ms
    return 125;
}

void TIMER0_IRQHandler(void){
	TIM_ClearIntPending((LPC_TIM_TypeDef *) LPC_TIM0, TIM_MR0_INT);

	 (*tick_handler)();
}

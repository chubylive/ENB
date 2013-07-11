/* ==================================================
 Name        : main.c
 Author      : Rohit Ramesh
 Version     : 1
 Description : Pull Entropy fom the ADC (Blocking)  
 ===================================================
 */

#include "../BT_Stack/Drivers/includes/lpc17xx_adc.h"
#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "stdint.h"
#include "includes/random.h"
#include "stdio.h"

static volatile uint8_t collector_byte;
static volatile uint8_t collector_status;

void ADC_IRQHandler(void) {
    // if collector status != ready
    /*volatile uint16_t i,d;
    
    for(i = 0;i < 8;i++){
        d = ADC_ChannelGetData(LPC_ADC,i);
    }*/

    volatile uint16_t data = ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
    
    if(collector_status < 8) {
        // get low order bit, and append it to the byte we're
        //  collecting
        uint8_t bit = data & 0x01;
        collector_byte = (collector_byte << 1) | bit;
        collector_status++;
        // Restart Collection
        int i;
        for(i = 0;i < 8;i++){
            data = ADC_ChannelGetData(LPC_ADC,i);
        }
        ADC_StartCmd(LPC_ADC,ADC_START_NOW);
     } else {
        //NVIC_ClearPendingIRQ(ADC_IRQn);
        //ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,DISABLE);
        ADC_StartCmd(LPC_ADC,0);
     }
    
}

int Random_Init() {

    collector_byte = 0;
    collector_status = 0;

    ADC_Init(LPC_ADC,1000);

    // P0.23 as AD0.0
    LPC_PINCON->PINSEL1 = (1 << 14);

    //Enable Interrupt
    NVIC_EnableIRQ(ADC_IRQn);

    ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,SET);
    // Enable CHannel 0
    ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);

    return 1;
}

int Get_Random_Byte(volatile uint8_t * byte){
    // Start in continuous mode    
    ADC_StartCmd(LPC_ADC,ADC_START_NOW);   
    // Spin on collector status until done
    while (collector_status < 8);
    // Get byte 
    *byte = collector_byte;
    // Set Collector Status ready
    collector_byte = 0x00;
    collector_status = 0;
    // return sucess
    return 1;
}

int Fill_Random_Bytes(volatile uint8_t * buffer,int bytes){
    // repeated Get Random Byte
    volatile uint8_t *arr = buffer;
    int i;
    for(i = 0; i < bytes;i++){
        Get_Random_Byte(&arr[i]);
    }
    return 1;
}

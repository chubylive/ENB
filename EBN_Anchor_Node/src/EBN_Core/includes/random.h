#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h> 

int Random_Init();

int Get_Random_Byte(volatile uint8_t * byte);
uint8_t Get_Random_Byte1();

int Fill_Random_Bytes(volatile uint8_t * buffer,int bytes);

#endif

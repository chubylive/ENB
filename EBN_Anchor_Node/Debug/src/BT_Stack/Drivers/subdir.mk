################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BT_Stack/Drivers/SimpleCache.c \
../src/BT_Stack/Drivers/lpc17xx_adc.c \
../src/BT_Stack/Drivers/lpc17xx_clkpwr.c \
../src/BT_Stack/Drivers/lpc17xx_libcfg_default.c \
../src/BT_Stack/Drivers/lpc17xx_pinsel.c \
../src/BT_Stack/Drivers/lpc17xx_ssp.c \
../src/BT_Stack/Drivers/lpc17xx_timer.c \
../src/BT_Stack/Drivers/lpc17xx_uart.c \
../src/BT_Stack/Drivers/sd.c 

OBJS += \
./src/BT_Stack/Drivers/SimpleCache.o \
./src/BT_Stack/Drivers/lpc17xx_adc.o \
./src/BT_Stack/Drivers/lpc17xx_clkpwr.o \
./src/BT_Stack/Drivers/lpc17xx_libcfg_default.o \
./src/BT_Stack/Drivers/lpc17xx_pinsel.o \
./src/BT_Stack/Drivers/lpc17xx_ssp.o \
./src/BT_Stack/Drivers/lpc17xx_timer.o \
./src/BT_Stack/Drivers/lpc17xx_uart.o \
./src/BT_Stack/Drivers/sd.o 

C_DEPS += \
./src/BT_Stack/Drivers/SimpleCache.d \
./src/BT_Stack/Drivers/lpc17xx_adc.d \
./src/BT_Stack/Drivers/lpc17xx_clkpwr.d \
./src/BT_Stack/Drivers/lpc17xx_libcfg_default.d \
./src/BT_Stack/Drivers/lpc17xx_pinsel.d \
./src/BT_Stack/Drivers/lpc17xx_ssp.d \
./src/BT_Stack/Drivers/lpc17xx_timer.d \
./src/BT_Stack/Drivers/lpc17xx_uart.d \
./src/BT_Stack/Drivers/sd.d 


# Each subdirectory must supply rules for building sources it contributes
src/BT_Stack/Drivers/%.o: ../src/BT_Stack/Drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\chuby\Dropbox\WorkSpace\CMSISv2p00_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



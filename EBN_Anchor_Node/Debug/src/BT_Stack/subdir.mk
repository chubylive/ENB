################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BT_Stack/att.c \
../src/BT_Stack/btstack_memory.c \
../src/BT_Stack/hal_cpu.c \
../src/BT_Stack/hal_tick.c \
../src/BT_Stack/hal_uart_dma.c \
../src/BT_Stack/hci.c \
../src/BT_Stack/hci_cmds.c \
../src/BT_Stack/hci_dump.c \
../src/BT_Stack/hci_transport_h4_dma.c \
../src/BT_Stack/l2cap.c \
../src/BT_Stack/l2cap_signaling.c \
../src/BT_Stack/linked_list.c \
../src/BT_Stack/memory_pool.c \
../src/BT_Stack/remote_device_db_memory.c \
../src/BT_Stack/rfcomm.c \
../src/BT_Stack/run_loop.c \
../src/BT_Stack/run_loop_embedded.c \
../src/BT_Stack/sdp_util.c \
../src/BT_Stack/utils.c 

OBJS += \
./src/BT_Stack/att.o \
./src/BT_Stack/btstack_memory.o \
./src/BT_Stack/hal_cpu.o \
./src/BT_Stack/hal_tick.o \
./src/BT_Stack/hal_uart_dma.o \
./src/BT_Stack/hci.o \
./src/BT_Stack/hci_cmds.o \
./src/BT_Stack/hci_dump.o \
./src/BT_Stack/hci_transport_h4_dma.o \
./src/BT_Stack/l2cap.o \
./src/BT_Stack/l2cap_signaling.o \
./src/BT_Stack/linked_list.o \
./src/BT_Stack/memory_pool.o \
./src/BT_Stack/remote_device_db_memory.o \
./src/BT_Stack/rfcomm.o \
./src/BT_Stack/run_loop.o \
./src/BT_Stack/run_loop_embedded.o \
./src/BT_Stack/sdp_util.o \
./src/BT_Stack/utils.o 

C_DEPS += \
./src/BT_Stack/att.d \
./src/BT_Stack/btstack_memory.d \
./src/BT_Stack/hal_cpu.d \
./src/BT_Stack/hal_tick.d \
./src/BT_Stack/hal_uart_dma.d \
./src/BT_Stack/hci.d \
./src/BT_Stack/hci_cmds.d \
./src/BT_Stack/hci_dump.d \
./src/BT_Stack/hci_transport_h4_dma.d \
./src/BT_Stack/l2cap.d \
./src/BT_Stack/l2cap_signaling.d \
./src/BT_Stack/linked_list.d \
./src/BT_Stack/memory_pool.d \
./src/BT_Stack/remote_device_db_memory.d \
./src/BT_Stack/rfcomm.d \
./src/BT_Stack/run_loop.d \
./src/BT_Stack/run_loop_embedded.d \
./src/BT_Stack/sdp_util.d \
./src/BT_Stack/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/BT_Stack/%.o: ../src/BT_Stack/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\chuby\Dropbox\WorkSpace\CMSISv2p00_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/EBN_Core/subdir.mk
-include src/BT_Stack/Drivers/subdir.mk
-include src/BT_Stack/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: EBN_Anchor_Node.axf

# Tool invocations
EBN_Anchor_Node.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\chuby\Dropbox\WorkSpace\CMSISv2p00_LPC17xx\Debug" -Xlinker -Map="EBN_Anchor_Node.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "EBN_Anchor_Node_Debug.ld" -o "EBN_Anchor_Node.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) EBN_Anchor_Node.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "EBN_Anchor_Node.axf"; # arm-none-eabi-objcopy -O binary "EBN_Anchor_Node.axf" "EBN_Anchor_Node.bin" ; checksum -p LPC1769 -d "EBN_Anchor_Node.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

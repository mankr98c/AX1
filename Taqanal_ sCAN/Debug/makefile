################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include TCP_STR/Src/subdir.mk
-include SMS/Src/subdir.mk
-include NANDFLASH/Src/subdir.mk
-include MQTT/Src/subdir.mk
-include LC86_GPS/Src/subdir.mk
-include GPIO/Src/subdir.mk
-include FOTA/Src/subdir.mk
-include Drivers/STM32H5xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include COMMON/Src/subdir.mk
-include CAN/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Taqanal_sCAN
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Taqanal_sCAN.elf \

MAP_FILES += \
Taqanal_sCAN.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Taqanal_sCAN.list \

OBJCOPY_HEX += \
Taqanal_sCAN.hex \

OBJCOPY_BIN += \
Taqanal_sCAN.bin \


# All Target
all: main-build

# Main-build Target
main-build: Taqanal_sCAN.elf secondary-outputs

# Tool invocations
Taqanal_sCAN.elf Taqanal_sCAN.map: $(OBJS) $(USER_OBJS) D:\IDE\sCAN_5.0.15\Taqanal_\ sCAN\STM32H573RITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Taqanal_sCAN.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"D:\IDE\sCAN_5.0.15\Taqanal_ sCAN\STM32H573RITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Taqanal_sCAN.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Taqanal_sCAN.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Taqanal_sCAN.list"
	@echo 'Finished building: $@'
	@echo ' '

Taqanal_sCAN.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "Taqanal_sCAN.hex"
	@echo 'Finished building: $@'
	@echo ' '

Taqanal_sCAN.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Taqanal_sCAN.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Taqanal_sCAN.bin Taqanal_sCAN.elf Taqanal_sCAN.hex Taqanal_sCAN.list Taqanal_sCAN.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets

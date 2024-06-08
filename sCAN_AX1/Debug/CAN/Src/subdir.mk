################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CAN/Src/CAN.c 

OBJS += \
./CAN/Src/CAN.o 

C_DEPS += \
./CAN/Src/CAN.d 


# Each subdirectory must supply rules for building sources it contributes
CAN/Src/%.o CAN/Src/%.su CAN/Src/%.cyclo: ../CAN/Src/%.c CAN/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H573xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/LC86_GPS/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/COMMON/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/CAN/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/FOTA/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/NANDFLASH/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/TCP_STR/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/SMS/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/GPIO/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CAN-2f-Src

clean-CAN-2f-Src:
	-$(RM) ./CAN/Src/CAN.cyclo ./CAN/Src/CAN.d ./CAN/Src/CAN.o ./CAN/Src/CAN.su

.PHONY: clean-CAN-2f-Src


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MQTT/Src/EC200MQTT.c 

OBJS += \
./MQTT/Src/EC200MQTT.o 

C_DEPS += \
./MQTT/Src/EC200MQTT.d 


# Each subdirectory must supply rules for building sources it contributes
MQTT/Src/%.o MQTT/Src/%.su MQTT/Src/%.cyclo: ../MQTT/Src/%.c MQTT/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H573xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/LC86_GPS/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/COMMON/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/MQTT/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-MQTT-2f-Src

clean-MQTT-2f-Src:
	-$(RM) ./MQTT/Src/EC200MQTT.cyclo ./MQTT/Src/EC200MQTT.d ./MQTT/Src/EC200MQTT.o ./MQTT/Src/EC200MQTT.su

.PHONY: clean-MQTT-2f-Src


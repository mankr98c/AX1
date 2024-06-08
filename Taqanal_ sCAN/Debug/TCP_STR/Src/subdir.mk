################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TCP_STR/Src/tcp.c 

OBJS += \
./TCP_STR/Src/tcp.o 

C_DEPS += \
./TCP_STR/Src/tcp.d 


# Each subdirectory must supply rules for building sources it contributes
TCP_STR/Src/%.o TCP_STR/Src/%.su TCP_STR/Src/%.cyclo: ../TCP_STR/Src/%.c TCP_STR/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H573xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/LC86_GPS/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/COMMON/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/CAN/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/FOTA/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/NANDFLASH/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/TCP_STR/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/SMS/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/GPIO/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TCP_STR-2f-Src

clean-TCP_STR-2f-Src:
	-$(RM) ./TCP_STR/Src/tcp.cyclo ./TCP_STR/Src/tcp.d ./TCP_STR/Src/tcp.o ./TCP_STR/Src/tcp.su

.PHONY: clean-TCP_STR-2f-Src


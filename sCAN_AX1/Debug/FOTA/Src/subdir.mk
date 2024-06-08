################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FOTA/Src/EC.c \
../FOTA/Src/Flash.c \
../FOTA/Src/Fota_func.c \
../FOTA/Src/SMS_COTA.c \
../FOTA/Src/TML_FOTA.c 

OBJS += \
./FOTA/Src/EC.o \
./FOTA/Src/Flash.o \
./FOTA/Src/Fota_func.o \
./FOTA/Src/SMS_COTA.o \
./FOTA/Src/TML_FOTA.o 

C_DEPS += \
./FOTA/Src/EC.d \
./FOTA/Src/Flash.d \
./FOTA/Src/Fota_func.d \
./FOTA/Src/SMS_COTA.d \
./FOTA/Src/TML_FOTA.d 


# Each subdirectory must supply rules for building sources it contributes
FOTA/Src/%.o FOTA/Src/%.su FOTA/Src/%.cyclo: ../FOTA/Src/%.c FOTA/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H573xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/LC86_GPS/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/COMMON/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/CAN/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/FOTA/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/NANDFLASH/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/TCP_STR/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/SMS/Inc" -I"D:/IDE/sCAN_5.0.15/sCAN_AX1/GPIO/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FOTA-2f-Src

clean-FOTA-2f-Src:
	-$(RM) ./FOTA/Src/EC.cyclo ./FOTA/Src/EC.d ./FOTA/Src/EC.o ./FOTA/Src/EC.su ./FOTA/Src/Flash.cyclo ./FOTA/Src/Flash.d ./FOTA/Src/Flash.o ./FOTA/Src/Flash.su ./FOTA/Src/Fota_func.cyclo ./FOTA/Src/Fota_func.d ./FOTA/Src/Fota_func.o ./FOTA/Src/Fota_func.su ./FOTA/Src/SMS_COTA.cyclo ./FOTA/Src/SMS_COTA.d ./FOTA/Src/SMS_COTA.o ./FOTA/Src/SMS_COTA.su ./FOTA/Src/TML_FOTA.cyclo ./FOTA/Src/TML_FOTA.d ./FOTA/Src/TML_FOTA.o ./FOTA/Src/TML_FOTA.su

.PHONY: clean-FOTA-2f-Src


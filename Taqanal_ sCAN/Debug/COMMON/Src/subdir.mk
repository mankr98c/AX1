################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../COMMON/Src/EC200Utils.c \
../COMMON/Src/common.c \
../COMMON/Src/fifo.c \
../COMMON/Src/map.c \
../COMMON/Src/stack.c 

OBJS += \
./COMMON/Src/EC200Utils.o \
./COMMON/Src/common.o \
./COMMON/Src/fifo.o \
./COMMON/Src/map.o \
./COMMON/Src/stack.o 

C_DEPS += \
./COMMON/Src/EC200Utils.d \
./COMMON/Src/common.d \
./COMMON/Src/fifo.d \
./COMMON/Src/map.d \
./COMMON/Src/stack.d 


# Each subdirectory must supply rules for building sources it contributes
COMMON/Src/%.o COMMON/Src/%.su COMMON/Src/%.cyclo: ../COMMON/Src/%.c COMMON/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H573xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc -I../Drivers/STM32H5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H5xx/Include -I../Drivers/CMSIS/Include -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/LC86_GPS/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/COMMON/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/CAN/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/FOTA/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/NANDFLASH/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/TCP_STR/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/SMS/Inc" -I"D:/IDE/sCAN_5.0.15/Taqanal_ sCAN/GPIO/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-COMMON-2f-Src

clean-COMMON-2f-Src:
	-$(RM) ./COMMON/Src/EC200Utils.cyclo ./COMMON/Src/EC200Utils.d ./COMMON/Src/EC200Utils.o ./COMMON/Src/EC200Utils.su ./COMMON/Src/common.cyclo ./COMMON/Src/common.d ./COMMON/Src/common.o ./COMMON/Src/common.su ./COMMON/Src/fifo.cyclo ./COMMON/Src/fifo.d ./COMMON/Src/fifo.o ./COMMON/Src/fifo.su ./COMMON/Src/map.cyclo ./COMMON/Src/map.d ./COMMON/Src/map.o ./COMMON/Src/map.su ./COMMON/Src/stack.cyclo ./COMMON/Src/stack.d ./COMMON/Src/stack.o ./COMMON/Src/stack.su

.PHONY: clean-COMMON-2f-Src


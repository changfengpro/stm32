################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../step\ motor/bujin.c 

OBJS += \
./step\ motor/bujin.o 

C_DEPS += \
./step\ motor/bujin.d 


# Each subdirectory must supply rules for building sources it contributes
step\ motor/bujin.o: ../step\ motor/bujin.c step\ motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/Stepper motors/step motor" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"step motor/bujin.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-step-20-motor

clean-step-20-motor:
	-$(RM) ./step\ motor/bujin.cyclo ./step\ motor/bujin.d ./step\ motor/bujin.o ./step\ motor/bujin.su

.PHONY: clean-step-20-motor


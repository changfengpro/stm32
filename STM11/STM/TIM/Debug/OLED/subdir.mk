################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../OLED/OLED.c 

OBJS += \
./OLED/OLED.o 

C_DEPS += \
./OLED/OLED.d 


# Each subdirectory must supply rules for building sources it contributes
OLED/%.o OLED/%.su OLED/%.cyclo: ../OLED/%.c OLED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/TIM/OLED" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-OLED

clean-OLED:
	-$(RM) ./OLED/OLED.cyclo ./OLED/OLED.d ./OLED/OLED.o ./OLED/OLED.su

.PHONY: clean-OLED


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PID/pid_2.c 

C_DEPS += \
./PID/pid_2.d 

OBJS += \
./PID/pid_2.o 


# Each subdirectory must supply rules for building sources it contributes
PID/%.o PID/%.su PID/%.cyclo: ../PID/%.c PID/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/A/Bsp" -I"D:/STM/A/PID" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-PID

clean-PID:
	-$(RM) ./PID/pid_2.cyclo ./PID/pid_2.d ./PID/pid_2.o ./PID/pid_2.su

.PHONY: clean-PID


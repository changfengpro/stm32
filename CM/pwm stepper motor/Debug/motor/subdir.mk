################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../motor/stepper_motor.c 

OBJS += \
./motor/stepper_motor.o 

C_DEPS += \
./motor/stepper_motor.d 


# Each subdirectory must supply rules for building sources it contributes
motor/%.o motor/%.su motor/%.cyclo: ../motor/%.c motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/pwm stepper motor/motor" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-motor

clean-motor:
	-$(RM) ./motor/stepper_motor.cyclo ./motor/stepper_motor.d ./motor/stepper_motor.o ./motor/stepper_motor.su

.PHONY: clean-motor


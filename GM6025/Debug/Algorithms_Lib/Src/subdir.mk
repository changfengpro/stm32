################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Algorithms_Lib/Src/Algorithms_Lib.c 

OBJS += \
./Algorithms_Lib/Src/Algorithms_Lib.o 

C_DEPS += \
./Algorithms_Lib/Src/Algorithms_Lib.d 


# Each subdirectory must supply rules for building sources it contributes
Algorithms_Lib/Src/%.o Algorithms_Lib/Src/%.su Algorithms_Lib/Src/%.cyclo: ../Algorithms_Lib/Src/%.c Algorithms_Lib/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/GM6025/Bsp/Inc" -I"D:/STM/GM6025/Chassis_Control/Inc" -I"D:/STM/GM6025/PID/Inc" -I"D:/STM/GM6025/Algorithms_Lib/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Algorithms_Lib-2f-Src

clean-Algorithms_Lib-2f-Src:
	-$(RM) ./Algorithms_Lib/Src/Algorithms_Lib.cyclo ./Algorithms_Lib/Src/Algorithms_Lib.d ./Algorithms_Lib/Src/Algorithms_Lib.o ./Algorithms_Lib/Src/Algorithms_Lib.su

.PHONY: clean-Algorithms_Lib-2f-Src


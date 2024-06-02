################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bsp/Src/Bsp_can.c \
../Bsp/Src/pid.c 

OBJS += \
./Bsp/Src/Bsp_can.o \
./Bsp/Src/pid.o 

C_DEPS += \
./Bsp/Src/Bsp_can.d \
./Bsp/Src/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Bsp/Src/%.o Bsp/Src/%.su Bsp/Src/%.cyclo: ../Bsp/Src/%.c Bsp/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/M20062/Bsp/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Bsp-2f-Src

clean-Bsp-2f-Src:
	-$(RM) ./Bsp/Src/Bsp_can.cyclo ./Bsp/Src/Bsp_can.d ./Bsp/Src/Bsp_can.o ./Bsp/Src/Bsp_can.su ./Bsp/Src/pid.cyclo ./Bsp/Src/pid.d ./Bsp/Src/pid.o ./Bsp/Src/pid.su

.PHONY: clean-Bsp-2f-Src


################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/drv_bsp.c \
../User/drv_can.c 

C_DEPS += \
./User/drv_bsp.d \
./User/drv_can.d 

OBJS += \
./User/drv_bsp.o \
./User/drv_can.o 


# Each subdirectory must supply rules for building sources it contributes
User/%.o User/%.su User/%.cyclo: ../User/%.c User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F427xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"D:/STM/A/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User

clean-User:
	-$(RM) ./User/drv_bsp.cyclo ./User/drv_bsp.d ./User/drv_bsp.o ./User/drv_bsp.su ./User/drv_can.cyclo ./User/drv_can.d ./User/drv_can.o ./User/drv_can.su

.PHONY: clean-User


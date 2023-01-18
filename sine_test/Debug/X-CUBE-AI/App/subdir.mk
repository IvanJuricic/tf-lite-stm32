################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../X-CUBE-AI/App/sine_approx.c \
../X-CUBE-AI/App/sine_approx_data.c 

OBJS += \
./X-CUBE-AI/App/sine_approx.o \
./X-CUBE-AI/App/sine_approx_data.o 

C_DEPS += \
./X-CUBE-AI/App/sine_approx.d \
./X-CUBE-AI/App/sine_approx_data.d 


# Each subdirectory must supply rules for building sources it contributes
X-CUBE-AI/App/%.o X-CUBE-AI/App/%.su: ../X-CUBE-AI/App/%.c X-CUBE-AI/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../X-CUBE-AI/App -I../Middlewares/ST/AI/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-X-2d-CUBE-2d-AI-2f-App

clean-X-2d-CUBE-2d-AI-2f-App:
	-$(RM) ./X-CUBE-AI/App/sine_approx.d ./X-CUBE-AI/App/sine_approx.o ./X-CUBE-AI/App/sine_approx.su ./X-CUBE-AI/App/sine_approx_data.d ./X-CUBE-AI/App/sine_approx_data.o ./X-CUBE-AI/App/sine_approx_data.su

.PHONY: clean-X-2d-CUBE-2d-AI-2f-App


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BBB_MPU6050.cpp \
../src/I2CDevice.cpp \
../src/Test_MPU6050.cpp 

OBJS += \
./src/BBB_MPU6050.o \
./src/I2CDevice.o \
./src/Test_MPU6050.o 

CPP_DEPS += \
./src/BBB_MPU6050.d \
./src/I2CDevice.d \
./src/Test_MPU6050.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



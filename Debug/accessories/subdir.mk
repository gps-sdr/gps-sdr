################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../accessories/fft.o \
../accessories/misc.o 

CPP_SRCS += \
../accessories/fft-test.cpp \
../accessories/fft.cpp \
../accessories/misc.cpp 

OBJS += \
./accessories/fft-test.o \
./accessories/fft.o \
./accessories/misc.o 

CPP_DEPS += \
./accessories/fft-test.d \
./accessories/fft.d \
./accessories/misc.d 


# Each subdirectory must supply rules for building sources it contributes
accessories/%.o: ../accessories/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



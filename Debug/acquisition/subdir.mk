################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../acquisition/acquisition.o 

CPP_SRCS += \
../acquisition/acq-test.cpp \
../acquisition/acquisition.cpp 

OBJS += \
./acquisition/acq-test.o \
./acquisition/acquisition.o 

CPP_DEPS += \
./acquisition/acq-test.d \
./acquisition/acquisition.d 


# Each subdirectory must supply rules for building sources it contributes
acquisition/%.o: ../acquisition/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../main/init.o \
../main/shutdown.o 

CPP_SRCS += \
../main/init.cpp \
../main/main.cpp \
../main/shutdown.cpp 

OBJS += \
./main/init.o \
./main/main.o \
./main/shutdown.o 

CPP_DEPS += \
./main/init.d \
./main/main.d \
./main/shutdown.d 


# Each subdirectory must supply rules for building sources it contributes
main/%.o: ../main/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



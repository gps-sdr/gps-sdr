################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../usrp/db_dbs_rx.cpp \
../usrp/gps-usrp.cpp 

OBJS += \
./usrp/db_dbs_rx.o \
./usrp/gps-usrp.o 

CPP_DEPS += \
./usrp/db_dbs_rx.d \
./usrp/gps-usrp.d 


# Each subdirectory must supply rules for building sources it contributes
usrp/%.o: ../usrp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



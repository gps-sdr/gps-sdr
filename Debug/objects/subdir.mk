################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../objects/channel.o \
../objects/commando.o \
../objects/correlator.o \
../objects/ephemeris.o \
../objects/fifo.o \
../objects/keyboard.o \
../objects/post_process.o \
../objects/pvt.o \
../objects/serial_telemetry.o \
../objects/sv_select.o \
../objects/telemetry.o \
../objects/threaded_object.o 

CPP_SRCS += \
../objects/channel.cpp \
../objects/commando.cpp \
../objects/correlator.cpp \
../objects/ephemeris.cpp \
../objects/fifo.cpp \
../objects/keyboard.cpp \
../objects/post_process.cpp \
../objects/pvt.cpp \
../objects/serial_telemetry.cpp \
../objects/sv_select.cpp \
../objects/telemetry.cpp \
../objects/threaded_object.cpp 

OBJS += \
./objects/channel.o \
./objects/commando.o \
./objects/correlator.o \
./objects/ephemeris.o \
./objects/fifo.o \
./objects/keyboard.o \
./objects/post_process.o \
./objects/pvt.o \
./objects/serial_telemetry.o \
./objects/sv_select.o \
./objects/telemetry.o \
./objects/threaded_object.o 

CPP_DEPS += \
./objects/channel.d \
./objects/commando.d \
./objects/correlator.d \
./objects/ephemeris.d \
./objects/fifo.d \
./objects/keyboard.d \
./objects/post_process.d \
./objects/pvt.d \
./objects/serial_telemetry.d \
./objects/sv_select.d \
./objects/telemetry.d \
./objects/threaded_object.d 


# Each subdirectory must supply rules for building sources it contributes
objects/%.o: ../objects/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../newgui/gui.cpp \
../newgui/gui_acquisition.cpp \
../newgui/gui_almanac.cpp \
../newgui/gui_channel.cpp \
../newgui/gui_classes.cpp \
../newgui/gui_commands.cpp \
../newgui/gui_default.cpp \
../newgui/gui_ephemeris.cpp \
../newgui/gui_select.cpp \
../newgui/gui_serial.cpp \
../newgui/gui_toplevel.cpp \
../newgui/threaded_object.cpp 

OBJS += \
./newgui/gui.o \
./newgui/gui_acquisition.o \
./newgui/gui_almanac.o \
./newgui/gui_channel.o \
./newgui/gui_classes.o \
./newgui/gui_commands.o \
./newgui/gui_default.o \
./newgui/gui_ephemeris.o \
./newgui/gui_select.o \
./newgui/gui_serial.o \
./newgui/gui_toplevel.o \
./newgui/threaded_object.o 

CPP_DEPS += \
./newgui/gui.d \
./newgui/gui_acquisition.d \
./newgui/gui_almanac.d \
./newgui/gui_channel.d \
./newgui/gui_classes.d \
./newgui/gui_commands.d \
./newgui/gui_default.d \
./newgui/gui_ephemeris.d \
./newgui/gui_select.d \
./newgui/gui_serial.d \
./newgui/gui_toplevel.d \
./newgui/threaded_object.d 


# Each subdirectory must supply rules for building sources it contributes
newgui/%.o: ../newgui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



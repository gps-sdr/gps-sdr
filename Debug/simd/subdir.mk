################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../simd/cpuid.o \
../simd/sse.o \
../simd/x86.o 

CPP_SRCS += \
../simd/cpuid.cpp \
../simd/simd-test.cpp \
../simd/sse.cpp \
../simd/sse_new.cpp \
../simd/x86.cpp 

OBJS += \
./simd/cpuid.o \
./simd/simd-test.o \
./simd/sse.o \
./simd/sse_new.o \
./simd/x86.o 

CPP_DEPS += \
./simd/cpuid.d \
./simd/simd-test.d \
./simd/sse.d \
./simd/sse_new.d \
./simd/x86.d 


# Each subdirectory must supply rules for building sources it contributes
simd/%.o: ../simd/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



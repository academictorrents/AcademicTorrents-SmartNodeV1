################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/lib_tests.cpp 

OBJS += \
./src/lib_tests.o 

CPP_DEPS += \
./src/lib_tests.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib_tests.o: ../src/lib_tests.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/nogueira/workspace/AcademicTorrents-SmartNodeV1/include" -I"/home/nogueira/workspace/AcademicTorrents-SmartNodeV1/include/boost" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/lib_tests.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



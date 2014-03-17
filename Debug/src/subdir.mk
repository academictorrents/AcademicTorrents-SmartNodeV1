################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/async_at_connection.cpp \
../src/lib_tests.cpp 

OBJS += \
./src/async_at_connection.o \
./src/lib_tests.o 

CPP_DEPS += \
./src/async_at_connection.d \
./src/lib_tests.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/nogueira/workspace/AcademicTorrents-SmartNodeV1/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/lib_tests.o: ../src/lib_tests.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/nogueira/workspace/AcademicTorrents-SmartNodeV1/include" -I"/home/nogueira/workspace/AcademicTorrents-SmartNodeV1/include/boost" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/lib_tests.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



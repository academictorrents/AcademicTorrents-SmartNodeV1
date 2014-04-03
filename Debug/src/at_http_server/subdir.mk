################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/at_http_server/server_connection.cpp \
../src/at_http_server/server_main.cpp \
../src/at_http_server/server_mime_types.cpp \
../src/at_http_server/server_reply.cpp \
../src/at_http_server/server_request_handler.cpp \
../src/at_http_server/server_request_parser.cpp 

OBJS += \
./src/at_http_server/server_connection.o \
./src/at_http_server/server_main.o \
./src/at_http_server/server_mime_types.o \
./src/at_http_server/server_reply.o \
./src/at_http_server/server_request_handler.o \
./src/at_http_server/server_request_parser.o 

CPP_DEPS += \
./src/at_http_server/server_connection.d \
./src/at_http_server/server_main.d \
./src/at_http_server/server_mime_types.d \
./src/at_http_server/server_reply.d \
./src/at_http_server/server_request_handler.d \
./src/at_http_server/server_request_parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/at_http_server/%.o: ../src/at_http_server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



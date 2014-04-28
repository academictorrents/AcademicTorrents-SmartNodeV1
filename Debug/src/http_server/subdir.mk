################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/http_server/at_server.cpp \
../src/http_server/at_server_connection.cpp \
../src/http_server/at_server_main.cpp \
../src/http_server/at_server_mime_types.cpp \
../src/http_server/at_server_reply.cpp \
../src/http_server/at_server_request_handler.cpp \
../src/http_server/at_server_request_parser.cpp 

OBJS += \
./src/http_server/at_server.o \
./src/http_server/at_server_connection.o \
./src/http_server/at_server_main.o \
./src/http_server/at_server_mime_types.o \
./src/http_server/at_server_reply.o \
./src/http_server/at_server_request_handler.o \
./src/http_server/at_server_request_parser.o 

CPP_DEPS += \
./src/http_server/at_server.d \
./src/http_server/at_server_connection.d \
./src/http_server/at_server_main.d \
./src/http_server/at_server_mime_types.d \
./src/http_server/at_server_reply.d \
./src/http_server/at_server_request_handler.d \
./src/http_server/at_server_request_parser.d 


# Each subdirectory must supply rules for building sources it contributes
src/http_server/%.o: ../src/http_server/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/usr/local/include/libtorrent -I"/home/at/AcademicTorrents-SmartNodeV1/include" -O0 -g3 -Wall -c -fmessage-length=0 -DTORRENT_USE_OPENSSL -DWITH_SHIPPED_GEOIP_H -DBOOST_ASIO_HASH_MAP_BUCKETS=1021 -DBOOST_EXCEPTION_DISABLE -DBOOST_ASIO_ENABLE_CANCELIO -DBOOST_ASIO_DYN_LINK -DTORRENT_LINKING_SHARED -DTORRENT_DEBUG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



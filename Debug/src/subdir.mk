################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CSVReader.cpp \
../src/TorrentInfo.cpp \
../src/async_at_connection.cpp \
../src/at_fetcher.cpp \
../src/database.cpp 

OBJS += \
./src/CSVReader.o \
./src/TorrentInfo.o \
./src/async_at_connection.o \
./src/at_fetcher.o \
./src/database.o 

CPP_DEPS += \
./src/CSVReader.d \
./src/TorrentInfo.d \
./src/async_at_connection.d \
./src/at_fetcher.d \
./src/database.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lucnguyen/workspace/AT/AcademicTorrents-SmartNodeV1/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



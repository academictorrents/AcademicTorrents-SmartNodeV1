################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CSVReader.cpp \
../src/TorrentInfo.cpp \
<<<<<<< Updated upstream
../src/async_at_connection.cpp \
../src/at_fetcher.cpp \
../src/database.cpp 
=======
../src/lib_tests.cpp \
../src/testCV.cpp 
>>>>>>> Stashed changes

OBJS += \
./src/CSVReader.o \
./src/TorrentInfo.o \
<<<<<<< Updated upstream
./src/async_at_connection.o \
./src/at_fetcher.o \
./src/database.o 
=======
./src/lib_tests.o \
./src/testCV.o 
>>>>>>> Stashed changes

CPP_DEPS += \
./src/CSVReader.d \
./src/TorrentInfo.d \
<<<<<<< Updated upstream
./src/async_at_connection.d \
./src/at_fetcher.d \
./src/database.d 
=======
./src/lib_tests.d \
./src/testCV.d 
>>>>>>> Stashed changes


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
<<<<<<< Updated upstream
=======
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/grigorii/workspace/cs410/AcademicTorrents-SmartNodeV1/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/lib_tests.o: ../src/lib_tests.cpp
>>>>>>> Stashed changes
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/lucnguyen/workspace/AT/AcademicTorrents-SmartNodeV1/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



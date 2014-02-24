################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libtorrent/kademlia/closest_nodes.cpp \
../libtorrent/kademlia/dht_tracker.cpp \
../libtorrent/kademlia/find_data.cpp \
../libtorrent/kademlia/node.cpp \
../libtorrent/kademlia/node_id.cpp \
../libtorrent/kademlia/refresh.cpp \
../libtorrent/kademlia/routing_table.cpp \
../libtorrent/kademlia/rpc_manager.cpp \
../libtorrent/kademlia/traversal_algorithm.cpp 

OBJS += \
./libtorrent/kademlia/closest_nodes.o \
./libtorrent/kademlia/dht_tracker.o \
./libtorrent/kademlia/find_data.o \
./libtorrent/kademlia/node.o \
./libtorrent/kademlia/node_id.o \
./libtorrent/kademlia/refresh.o \
./libtorrent/kademlia/routing_table.o \
./libtorrent/kademlia/rpc_manager.o \
./libtorrent/kademlia/traversal_algorithm.o 

CPP_DEPS += \
./libtorrent/kademlia/closest_nodes.d \
./libtorrent/kademlia/dht_tracker.d \
./libtorrent/kademlia/find_data.d \
./libtorrent/kademlia/node.d \
./libtorrent/kademlia/node_id.d \
./libtorrent/kademlia/refresh.d \
./libtorrent/kademlia/routing_table.d \
./libtorrent/kademlia/rpc_manager.d \
./libtorrent/kademlia/traversal_algorithm.d 


# Each subdirectory must supply rules for building sources it contributes
libtorrent/kademlia/%.o: ../libtorrent/kademlia/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/nogueira/workspace/AcademicTorrents-SmartNodeV1/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



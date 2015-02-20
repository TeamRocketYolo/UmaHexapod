################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../little_helpers/Object.cpp \
../little_helpers/Parser.cpp \
../little_helpers/SocketServer.cpp \
../little_helpers/gnublin.cpp 

OBJS += \
./little_helpers/Object.o \
./little_helpers/Parser.o \
./little_helpers/SocketServer.o \
./little_helpers/gnublin.o 

CPP_DEPS += \
./little_helpers/Object.d \
./little_helpers/Parser.d \
./little_helpers/SocketServer.d \
./little_helpers/gnublin.d 


# Each subdirectory must supply rules for building sources it contributes
little_helpers/%.o: ../little_helpers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/home/lukas/Schreibtisch/subvrsion_hex/RoboMotion2/CrossCompiler/CMakeToolChain/x-tools6h/arm-unknown-linux-gnueabihf/bin/arm-unknown-linux-gnueabihf-g++ -std=c++1y -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I/usr/include/X11/extensions -I"/home/lukas/Schreibtisch/workspace/Hexapod" -I"/home/lukas/Schreibtisch/workspace/Hexapod/little_helpers" -I"/home/lukas/Schreibtisch/workspace/Hexapod/gaits" -O0 -g3 -Wall -lpthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



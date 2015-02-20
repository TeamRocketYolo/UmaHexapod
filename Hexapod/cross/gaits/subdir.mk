################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gaits/Movement.cpp \
../gaits/RippleGait.cpp \
../gaits/RippleGait_Response.cpp \
../gaits/TripodGait.cpp \
../gaits/TripodGait_Response.cpp \
../gaits/WaveGait.cpp 

OBJS += \
./gaits/Movement.o \
./gaits/RippleGait.o \
./gaits/RippleGait_Response.o \
./gaits/TripodGait.o \
./gaits/TripodGait_Response.o \
./gaits/WaveGait.o 

CPP_DEPS += \
./gaits/Movement.d \
./gaits/RippleGait.d \
./gaits/RippleGait_Response.d \
./gaits/TripodGait.d \
./gaits/TripodGait_Response.d \
./gaits/WaveGait.d 


# Each subdirectory must supply rules for building sources it contributes
gaits/%.o: ../gaits/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/home/lukas/Schreibtisch/subvrsion_hex/RoboMotion2/CrossCompiler/CMakeToolChain/x-tools6h/arm-unknown-linux-gnueabihf/bin/arm-unknown-linux-gnueabihf-g++ -std=c++1y -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I/usr/include/X11/extensions -I"/home/lukas/Schreibtisch/workspace/Hexapod" -I"/home/lukas/Schreibtisch/workspace/Hexapod/little_helpers" -I"/home/lukas/Schreibtisch/workspace/Hexapod/gaits" -O0 -g3 -Wall -lpthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Hexapod.cpp \
../InverseKinematic.cpp \
../Leg.cpp \
../LegCtrl.cpp \
../Servo.cpp \
../main.cpp 

OBJS += \
./Hexapod.o \
./InverseKinematic.o \
./Leg.o \
./LegCtrl.o \
./Servo.o \
./main.o 

CPP_DEPS += \
./Hexapod.d \
./InverseKinematic.d \
./Leg.d \
./LegCtrl.d \
./Servo.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	/home/lukas/Schreibtisch/subvrsion_hex/RoboMotion2/CrossCompiler/CMakeToolChain/x-tools6h/arm-unknown-linux-gnueabihf/bin/arm-unknown-linux-gnueabihf-g++ -std=c++1y -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I/usr/include/X11/extensions -I"/home/lukas/Schreibtisch/workspace/Hexapod" -I"/home/lukas/Schreibtisch/workspace/Hexapod/little_helpers" -I"/home/lukas/Schreibtisch/workspace/Hexapod/gaits" -O0 -g3 -Wall -lpthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



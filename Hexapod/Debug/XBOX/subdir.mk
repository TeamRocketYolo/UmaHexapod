################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../XBOX/Testdriver.cpp \
../XBOX/XBOXController.cpp 

OBJS += \
./XBOX/Testdriver.o \
./XBOX/XBOXController.o 

CPP_DEPS += \
./XBOX/Testdriver.d \
./XBOX/XBOXController.d 


# Each subdirectory must supply rules for building sources it contributes
XBOX/%.o: ../XBOX/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -I/usr/include/X11/extensions -I"/home/lukas/Schreibtisch/workspace/Hexapod" -I"/home/lukas/Schreibtisch/workspace/Hexapod/little_helpers" -I"/home/lukas/Schreibtisch/workspace/Hexapod/gaits" -O0 -g3 -Wall -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



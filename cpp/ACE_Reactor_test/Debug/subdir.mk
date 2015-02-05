################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ICP_Client.cpp \
../IPC_Server.cpp \
../main.cpp 

OBJS += \
./ICP_Client.o \
./IPC_Server.o \
./main.o 

CPP_DEPS += \
./ICP_Client.d \
./IPC_Server.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	c++ -I/home/sveigri/5414KTT/libraries/ACE_wrappers/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



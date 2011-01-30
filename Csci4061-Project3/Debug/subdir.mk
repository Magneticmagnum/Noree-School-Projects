################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../mm_public.o 

C_SRCS += \
../main_malloc.c \
../main_mm.c \
../mm_public.c \
../packet_public.c \
../tests.c 

OBJS += \
./main_malloc.o \
./main_mm.o \
./mm_public.o \
./packet_public.o \
./tests.o 

C_DEPS += \
./main_malloc.d \
./main_mm.d \
./mm_public.d \
./packet_public.d \
./tests.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



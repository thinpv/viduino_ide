#
# Machine makefile
#

.PHONY:clean
.PHONY:write
.PHONY:viduino
.PHONY:dump
.PHONY:test

ROOT = .
BUILD ?= build
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python3
OS ?= nonos

COMPILE			= /home/thinpv/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
CC				= $(COMPILE)gcc
CXX				= $(COMPILE)g++
AS				= $(COMPILE)gcc -x assembler-with-cpp
AR				= $(COMPILE)ar rcs
LD				= $(COMPILE)ld
OBJCOPY			= $(COMPILE)objcopy
OBJDUMP			= $(COMPILE)objdump
SIZE			= $(COMPILE)size

DEFINES			+= -D__ARM32_ARCH__=5 -D__ARM926EJS__ -D__ARM32__ -Wno-unused-function

ASFLAGS			:= -g -ggdb -Wall -O3
CFLAGS			:= -g -ggdb -Wall -O3
CPFLAGS			:= -g -ggdb -Wall -O3
LDFLAGS			:= -T $(ROOT)/variants/viduino_uno/f1c100s.ld -nostdlib
MCFLAGS			:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -specs=nosys.specs -mno-thumb-interwork

ASFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CPFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -fno-exceptions
LDFLAGS			+=	-Wl,-gc-sections -ffunction-sections -fdata-sections

# ************************** SDK **************************
SDK				= $(ROOT)/tools/sdk
DRIVER			= $(SDK)/driver
MACHINE			= $(SDK)/$(OS)
SYSTEM			= $(SDK)/system
UTIL			= $(SDK)/util

INCDIRS			+= $(DRIVER)
INCDIRS			+= $(MACHINE) $(MACHINE)/include
INCDIRS			+= $(SYSTEM) $(SYSTEM)/f1c100s
INCDIRS			+= $(UTIL)

SRCDIRS			+= $(MACHINE)
SRCDIRS			+= $(SYSTEM)
SRCDIRS			+= $(UTIL)

# ************************** USER **************************

ifeq ($(MAKECMDGOALS), )
else ifeq ($(MAKECMDGOALS), all)
else ifeq ($(MAKECMDGOALS), clean)
else ifeq ($(MAKECMDGOALS), write)
else ifeq ($(MAKECMDGOALS), writelzma)
else
include examples/$(MAKECMDGOALS)/component.mk
TARGET = $(MAKECMDGOALS)
endif

# ************************** LIBRARIES **************************

# ************************** BUILD **************************

DRIVERFILES		:=	$(foreach dir, $(DRIVER), $(wildcard $(dir)/*.c))
DRIVEROBJS		:=	$(patsubst %, $(BUILD)/%, $(DRIVERFILES:.c=.o))

SFILES			:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPFILES			:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

INCLUDE			:=	$(patsubst %, -I %, $(INCDIRS))
SOBJS			:=	$(patsubst %, $(BUILD)/%, $(SFILES:.S=.o))
COBJS			:=	$(patsubst %, $(BUILD)/%, $(CFILES:.c=.o))
CPOBJS			:=	$(patsubst %, $(BUILD)/%, $(CPFILES:.cpp=.o))
SRCOBJS 		= $(SOBJS) $(COBJS) $(CPOBJS)

ALLOBJ 			= $(SRCOBJS) $(DRIVEROBJS)

ALLOBJ_DIRS 	= $(sort $(dir $(ALLOBJ)))

all:
	$(ECHO) "user make <target>"

$(TARGET): $(BUILD)/firmware.bin.temp

$(ALLOBJ): | $(ALLOBJ_DIRS)
$(ALLOBJ_DIRS):
	@$(MKDIR) -p $@

$(BUILD)/%.o: %.S
	$(ECHO) "AS $<"
	@$(AS) $(MCFLAGS) $(ASFLAGS) -c $< -o $@
	
$(BUILD)/%.o: %.c
	$(ECHO) "CC $<"
	@$(CC) $(INCLUDE) $(MCFLAGS) $(CFLAGS) -MD -c $< -o $@

$(BUILD)/%.o: %.cpp
	$(ECHO) "CXX $<"
	@$(CXX) $(INCLUDE) $(MCFLAGS) $(CPFLAGS) -MD -c $< -o $@

$(BUILD)/firmware.elf: $(ALLOBJ)
	@$(MKDIR) -p $(BUILD)/lib
	@$(AR) $(BUILD)/lib/libsdk.a $(DRIVEROBJS)
	$(ECHO) "LINK $@"
	@$(CXX) -Wl,--cref,-Map=$@.map $(LDFLAGS) -Wl,--start-group $(SRCOBJS) -Wl,--end-group -Wl,-EL -o $@ $(BUILD)/lib/libsdk.a -lgcc -lm -lc -lnosys
	@$(SIZE) $@

$(BUILD)/firmware.bin.temp: $(BUILD)/firmware.elf
	@$(OBJCOPY) -v -O binary $^ $@
	$(PYTHON) $(ROOT)/tools/pack.py $(BUILD)/firmware.bin.temp $(BUILD)/firmware.bin
	$(PYTHON) $(ROOT)/tools/pack_lzma.py $(BUILD)/firmware.bin.temp $(BUILD)/firmware.bin.lzma

write: $(BUILD)/firmware.bin.temp
	$(PYTHON) $(ROOT)/tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin

writelzma: $(BUILD)/firmware.bin.temp
	$(PYTHON) $(ROOT)/tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin.lzma

clean:
	$(RM) -rf $(BUILD)/

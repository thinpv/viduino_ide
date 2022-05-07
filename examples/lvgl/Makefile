#
# Machine makefile
#

.PHONY:clean
.PHONY:write
.PHONY:viduino
.PHONY:dump
.PHONY:test

ROOT = ../..
BUILD ?= build
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python3
NONOS	= NON_OS
# FREERTOS = FREERTOS_OS
OS		= $(NONOS)

COMPILE			= /home/thinpv/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
CC					= $(COMPILE)gcc
CXX					= $(COMPILE)g++
AS					= $(COMPILE)gcc -x assembler-with-cpp
AR					= $(COMPILE)ar rcs
LD					= $(COMPILE)ld
OBJCOPY			= $(COMPILE)objcopy
OBJDUMP			= $(COMPILE)objdump
SIZE				= $(COMPILE)size

DEFINES			+= -D__ARM32_ARCH__=5 -D__ARM926EJS__ -D__ARM32__ -Wno-unused-function

# ifeq ($(OS), $(FREERTOS))
# DEFINES			+= -DFREERTOS_OS
# endif

ASFLAGS			:= -g -ggdb -Wall -O3
CFLAGS			:= -g -ggdb -Wall -O3
CPFLAGS			:= -g -ggdb -Wall -O3
LDFLAGS			:= -T $(ROOT)/variants/viduino_uno/f1c100s.ld -nostdlib
MCFLAGS			:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -specs=nosys.specs -mno-thumb-interwork

ASFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CPFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -fno-exceptions
LDFLAGS			+=	-Wl,-gc-sections -ffunction-sections -fdata-sections

# ************************** API **************************
# API					= cores/allwinner/api
# CORES				= cores/allwinner

# INCDIRS			+= $(API)
# SRCDIRS			+= $(API)

# INCDIRS			+= $(CORES) variants/viduino_uno
# SRCDIRS			+= $(CORES)

# SRCDIRS			+= .

# ************************** SDK **************************
SDK					= $(ROOT)/tools/sdk
DRV					= $(SDK)/driver
UTIL				= $(SDK)/util
MACHINE			= $(SDK)/$(NONOS)/boot
# FREERTOS_BOOT		= $(SDK)/$(FREERTOS)/boot $(SDK)/$(FREERTOS)/freertos

INCDIRS			+= $(DRV)
INCDIRS			+= $(UTIL)
INCDIRS			+= $(MACHINE) $(MACHINE)/f1c100s
# ifeq ($(OS), $(NONOS))
# INCDIRS			+= $(SDK)/$(NONOS)/boot $(SDK)/$(NONOS)/boot/f1c100s
# else
# INCDIRS			+= $(SDK)/$(FREERTOS)/boot $(SDK)/$(FREERTOS)/boot/f1c100s $(SDK)/$(FREERTOS)/freertos/include
# endif

# ************************** USER **************************
USER				= .
INCDIRS			+= $(USER) src
SRCDIRS			+= $(USER) src

INCDIRS			+= lvgl/src \
	lvgl/src/lv_core \
	lvgl/src/lv_draw \
	lvgl/src/lv_font \
	lvgl/src/lv_gpu \
	lvgl/src/lv_hal \
	lvgl/src/lv_misc \
	lvgl/src/lv_themes \
	lvgl/src/lv_widgets

SRCDIRS			+= lvgl/src/lv_core \
	lvgl/src/lv_draw \
	lvgl/src/lv_font \
	lvgl/src/lv_gpu \
	lvgl/src/lv_hal \
	lvgl/src/lv_misc \
	lvgl/src/lv_themes \
	lvgl/src/lv_widgets

# ************************** LIBRARIES **************************

# ************************** BUILD **************************

DRVCFILES		:=	$(foreach dir, $(DRV), $(wildcard $(dir)/*.c))
DRVCOBJS		:=	$(patsubst %, $(BUILD)/%, $(DRVCFILES:.c=.o))
DRVOBJS = $(DRVCOBJS) 

UTILCFILES		:=	$(foreach dir, $(UTIL), $(wildcard $(dir)/*.c))
UTILCOBJS		:=	$(patsubst %, $(BUILD)/%, $(UTILCFILES:.c=.o))
UTILOBJS = $(UTILCOBJS) 

MACHINESFILES	:=	$(foreach dir, $(MACHINE), $(wildcard $(dir)/*.S))
MACHINECFILES	:=	$(foreach dir, $(MACHINE), $(wildcard $(dir)/*.c))
MACHINESOBJS		:=	$(patsubst %, $(BUILD)/%, $(MACHINESFILES:.S=.o))
MACHINECOBJS		:=	$(patsubst %, $(BUILD)/%, $(MACHINECFILES:.c=.o))
MACHINEOBJS = $(MACHINESOBJS) $(MACHINECOBJS)

# FREERTOS_BOOTSFILES	:=	$(foreach dir, $(FREERTOS_BOOT), $(wildcard $(dir)/*.S))
# FREERTOS_BOOTCFILES	:=	$(foreach dir, $(FREERTOS_BOOT), $(wildcard $(dir)/*.c))
# FREERTOS_BOOTSOBJS		:=	$(patsubst %, $(BUILD)/%, $(FREERTOS_BOOTSFILES:.S=.o))
# FREERTOS_BOOTCOBJS		:=	$(patsubst %, $(BUILD)/%, $(FREERTOS_BOOTCFILES:.c=.o))
# FREERTOS_BOOTOBJS = $(FREERTOS_BOOTSOBJS) $(FREERTOS_BOOTCOBJS)

# ifeq ($(OS), $(NONOS))
# BOOTOBJS = $(MACHINEOBJS)
# else
# BOOTOBJS = $(FREERTOS_BOOTOBJS)
# endif

SDKOBJS = $(DRVOBJS) $(UTILOBJS) $(MACHINEOBJS)

SFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

INCLUDE	:=	$(patsubst %, -I %, $(INCDIRS))
SOBJS		:=	$(patsubst %, $(BUILD)/%, $(SFILES:.S=.o))
COBJS		:=	$(patsubst %, $(BUILD)/%, $(CFILES:.c=.o))
CPOBJS		:=	$(patsubst %, $(BUILD)/%, $(CPFILES:.cpp=.o))
OBJS = $(SOBJS) $(COBJS) $(CPOBJS)

ALLOBJ = $(OBJS) $(DRVOBJS) $(UTILOBJS) $(MACHINEOBJS)

ALLOBJ_DIRS = $(sort $(dir $(ALLOBJ)))

all: $(BUILD)/firmware.bin

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
	@$(AR) $(BUILD)/lib/libsdk.a $(MACHINEOBJS)
	@$(AR) $(BUILD)/lib/libdriver.a $(DRVOBJS) $(UTILOBJS)
	$(ECHO) "LINK $@"
	@$(CXX) -Wl,--cref,-Map=$@.map $(LDFLAGS) -Wl,--start-group $(OBJS) -Wl,--end-group -Wl,-EL -o $@ $(BUILD)/lib/libsdk.a $(BUILD)/lib/libdriver.a $(BUILD)/lib/libsdk.a -lgcc -lm -lc -lnosys
	@$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	@$(OBJCOPY) -v -O binary $^ $@
	$(PYTHON) $(ROOT)/tools/lzmatool.py $(BUILD)/firmware.bin $(BUILD)/firmware.bin.lzma

write:
	$(PYTHON) $(ROOT)/tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin.lzma

write2:
	$(PYTHON) $(ROOT)/tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin

# viduino: $(SDKOBJS)
# 	mkdir -p viduino
# 	cp -r cores libraries tools variants boards.txt platform.txt programmers.txt viduino
# 	mkdir -p viduino/$(SDK)/lib
# 	mkdir -p viduino/$(SDK)/$(NONOS)/lib
# 	mkdir -p viduino/$(SDK)/$(FREERTOS)/lib
# 	$(AR) viduino/$(SDK)/lib/libdriver.a $(DRVOBJS) $(UTILOBJS)
# 	$(AR) viduino/$(SDK)/$(NONOS)/lib/libsdk.a $(MACHINEOBJS)
# 	$(AR) viduino/$(SDK)/$(FREERTOS)/lib/libsdk.a $(FREERTOS_BOOTOBJS)
# 	rm viduino/$(MACHINE)/*.c viduino/$(MACHINE)/*.S
# 	rm viduino/$(SDK)/$(FREERTOS)/boot/*.c viduino/$(SDK)/$(FREERTOS)/boot/*.S viduino/$(SDK)/$(FREERTOS)/freertos/*.c
# 	rm viduino/$(DRV)/*.c
# 	rm viduino/$(UTIL)/*.c
# 	tar -cf $(VIDUINO) viduino 
# 	rm -r viduino
# 	sha256sum $(VIDUINO) | tr '[a-z]' '[A-Z]'
# 	ls -ls $(VIDUINO)

clean:
	rm -rf $(BUILD)/

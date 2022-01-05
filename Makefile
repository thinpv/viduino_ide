#
# Machine makefile
#

.PHONY:clean
.PHONY:write
.PHONY:viduino
.PHONY:dump
.PHONY:test

BUILD ?= build
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python3
VIDUINO = viduino-0.0.14.tar
NONOS	= NON_OS
FREERTOS = FREERTOS_OS
OS		= $(FREERTOS)

COMPILE			= /home/thinpv/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
CC					= $(COMPILE)gcc
CXX					= $(COMPILE)g++
AS					= $(COMPILE)gcc -x assembler-with-cpp
AR					= $(COMPILE)ar rcs
LD					= $(COMPILE)ld
OBJCOPY			= $(COMPILE)objcopy
OBJDUMP			= $(COMPILE)objdump
SIZE				= $(COMPILE)size

API					= cores/allwinner/api
CORES				= cores/allwinner

DEFINES			+= -D__ARM32_ARCH__=5 -D__ARM926EJS__ -D__ARM32__ -Wno-unused-function

ASFLAGS			:= -g -ggdb -Wall -O3
CFLAGS			:= -g -ggdb -Wall -O3
CPFLAGS			:= -g -ggdb -Wall -O3
LDFLAGS			:= -T variants/viduino_uno/f1c100s.ld -nostdlib
MCFLAGS			:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -specs=nosys.specs -mno-thumb-interwork

ASFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CPFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -fno-exceptions
LDFLAGS			+=	-Wl,-gc-sections -ffunction-sections -fdata-sections

# ************************** SDK **************************
SDK					= tools/sdk
DRV					= $(SDK)/driver
UTIL				= $(SDK)/util
NONOS_BOOT				= $(SDK)/$(NONOS)/boot
FREERTOS_BOOT		= $(SDK)/$(FREERTOS)/boot

INCDIRS			+= $(DRV)
INCDIRS			+= $(UTIL)
ifeq ($(OS), $(NONOS))
INCDIRS			+= $(NONOS_BOOT) $(NONOS_BOOT)/f1c100s
else
INCDIRS			+= $(FREERTOS_BOOT) $(FREERTOS_BOOT)/f1c100s
endif

# ************************** API **************************

INCDIRS			+= $(API)
SRCDIRS			+= $(API)

INCDIRS			+= $(CORES) variants/viduino_uno
SRCDIRS			+= $(CORES)

SRCDIRS			+= .

# ************************** LIBRARIES **************************

INCDIRS			+= libraries/Wire/src
SRCDIRS			+= libraries/Wire/src

INCDIRS			+= libraries/fatfs/src
SRCDIRS			+= libraries/fatfs/src

INCDIRS			+= libraries/FrameBuffer/src
SRCDIRS			+= libraries/FrameBuffer/src

INCDIRS			+= libraries/NS2009/src
SRCDIRS			+= libraries/NS2009/src

INCDIRS			+= libraries/SoftwareTimer
SRCDIRS			+= libraries/SoftwareTimer

# INCDIRS			+= libraries/lvgl/src \
# 	libraries/lvgl/src/lvgl/src \
# 	libraries/lvgl/src/lvgl/src/lv_core \
# 	libraries/lvgl/src/lvgl/src/lv_draw \
# 	libraries/lvgl/src/lvgl/src/lv_font \
# 	libraries/lvgl/src/lvgl/src/lv_gpu \
# 	libraries/lvgl/src/lvgl/src/lv_hal \
# 	libraries/lvgl/src/lvgl/src/lv_misc \
# 	libraries/lvgl/src/lvgl/src/lv_themes \
# 	libraries/lvgl/src/lvgl/src/lv_widgets \
# 	libraries/lvgl/examples/lv_demo_music
# SRCDIRS			+= libraries/lvgl/src/lvgl/src/lv_core \
# 	libraries/lvgl/src/lvgl/src/lv_draw \
# 	libraries/lvgl/src/lvgl/src/lv_font \
# 	libraries/lvgl/src/lvgl/src/lv_gpu \
# 	libraries/lvgl/src/lvgl/src/lv_hal \
# 	libraries/lvgl/src/lvgl/src/lv_misc \
# 	libraries/lvgl/src/lvgl/src/lv_themes \
# 	libraries/lvgl/src/lvgl/src/lv_widgets \
# 	libraries/lvgl/examples/lv_demo_music

# INCDIRS			+= libraries/tinyusb/src \
# 	libraries/tinyusb/src/common \
# 	libraries/tinyusb/src/device \
# 	libraries/tinyusb/src/class \
# 	libraries/tinyusb/src/portable/sunxi \
# 	libraries/tinyusb/examples/device/cdc_dual_ports/src

# SRCDIRS			+= libraries/tinyusb/src \
# 	libraries/tinyusb/src/common \
# 	libraries/tinyusb/src/device \
# 	libraries/tinyusb/src/class/cdc \
# 	libraries/tinyusb/src/portable/sunxi \
# 	libraries/tinyusb/examples/device/cdc_dual_ports/src

# INCDIRS			+= libraries/png/libz
# SRCDIRS			+= libraries/png/libz

# INCDIRS			+= libraries/png/libpng
# SRCDIRS			+= libraries/png/libpng

# INCDIRS			+= libraries/nes/src
# SRCDIRS			+= libraries/nes/src

# INCDIRS			+= libraries/png/png
# SRCDIRS			+= libraries/png/png

# LIBFILES		+= libraries/mjpegDecorder/jpeg_dec.a
# INCDIRS			+= libraries/mjpegDecorder \
# 	libraries/mjpegDecorder/videoplay \
# 	libraries/mjpegDecorder/test_jpeg
# SRCDIRS			+= libraries/mjpegDecorder \
# 	libraries/mjpegDecorder/videoplay

# ************************** BUILD **************************

DRVCFILES		:=	$(foreach dir, $(DRV), $(wildcard $(dir)/*.c))
DRVCOBJS		:=	$(patsubst %, $(BUILD)/%, $(DRVCFILES:.c=.o))
DRVOBJS = $(DRVCOBJS) 

UTILCFILES		:=	$(foreach dir, $(UTIL), $(wildcard $(dir)/*.c))
UTILCOBJS		:=	$(patsubst %, $(BUILD)/%, $(UTILCFILES:.c=.o))
UTILOBJS = $(UTILCOBJS) 

NONOS_BOOTSFILES	:=	$(foreach dir, $(NONOS_BOOT), $(wildcard $(dir)/*.S))
NONOS_BOOTCFILES	:=	$(foreach dir, $(NONOS_BOOT), $(wildcard $(dir)/*.c))
NONOS_BOOTSOBJS		:=	$(patsubst %, $(BUILD)/%, $(NONOS_BOOTSFILES:.S=.o))
NONOS_BOOTCOBJS		:=	$(patsubst %, $(BUILD)/%, $(NONOS_BOOTCFILES:.c=.o))
NONOS_BOOTOBJS = $(NONOS_BOOTSOBJS) $(NONOS_BOOTCOBJS)

FREERTOS_BOOTSFILES	:=	$(foreach dir, $(FREERTOS_BOOT), $(wildcard $(dir)/*.S))
FREERTOS_BOOTCFILES	:=	$(foreach dir, $(FREERTOS_BOOT), $(wildcard $(dir)/*.c))
FREERTOS_BOOTSOBJS		:=	$(patsubst %, $(BUILD)/%, $(FREERTOS_BOOTSFILES:.S=.o))
FREERTOS_BOOTCOBJS		:=	$(patsubst %, $(BUILD)/%, $(FREERTOS_BOOTCFILES:.c=.o))
FREERTOS_BOOTOBJS = $(FREERTOS_BOOTSOBJS) $(FREERTOS_BOOTCOBJS)

ifeq ($(OS), $(NONOS))
BOOTOBJS = $(NONOS_BOOTOBJS)
else
BOOTOBJS = $(FREERTOS_BOOTOBJS)
endif

SDKOBJS = $(DRVOBJS) $(UTILOBJS) $(BOOTOBJS)

SFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

INCLUDE	:=	$(patsubst %, -I %, $(INCDIRS))
SOBJS		:=	$(patsubst %, $(BUILD)/%, $(SFILES:.S=.o))
COBJS		:=	$(patsubst %, $(BUILD)/%, $(CFILES:.c=.o))
CPOBJS		:=	$(patsubst %, $(BUILD)/%, $(CPFILES:.cpp=.o))
OBJS = $(SOBJS) $(COBJS) $(CPOBJS)

ALLOBJ = $(OBJS) $(SDKOBJS)

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
	@$(AR) $(BUILD)/lib/libsdk.a $(BOOTOBJS)
	@$(AR) $(BUILD)/lib/libdriver.a $(DRVOBJS) $(UTILOBJS)
	$(ECHO) "LINK $@"
	@$(CXX) -Wl,--cref,-Map=$@.map $(LDFLAGS) -Wl,--start-group $(OBJS) -Wl,--end-group -Wl,-EL -o $@ $(BUILD)/lib/libsdk.a $(BUILD)/lib/libdriver.a $(BUILD)/lib/libsdk.a -lgcc -lm -lc -lnosys
	@$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	@$(OBJCOPY) -v -O binary $^ $@
	$(PYTHON) tools/lzmatool.py $(BUILD)/firmware.bin $(BUILD)/firmware.bin.lzma

write:
	$(PYTHON) tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin.lzma

write2:
	$(PYTHON) tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin

viduino: $(SDKOBJS)
	mkdir -p viduino
	cp -r cores libraries tools variants boards.txt platform.txt programmers.txt viduino
	mkdir -p viduino/$(SDK)/$(NONOS)/lib
	mkdir -p viduino/$(SDK)/$(FREERTOS)/lib
	$(AR) viduino/$(SDK)/$(NONOS)/lib/libsdk.a $(NONOS_BOOTOBJS)
	$(AR) viduino/$(SDK)/$(FREERTOS)/lib/libsdk.a $(FREERTOS_BOOTOBJS)
	$(AR) viduino/$(SDK)/lib/libdriver.a $(DRVOBJS) $(UTILOBJS)
	rm viduino/$(NONOS_BOOT)/*.c viduino/$(NONOS_BOOT)/*.S
	rm viduino/$(FREERTOS_BOOT)/*.c viduino/$(FREERTOS_BOOT)/*.S
	rm viduino/$(DRV)/*.c
	rm viduino/$(UTIL)/*.c
	tar -cf $(VIDUINO) viduino 
	rm -r viduino
	sha256sum $(VIDUINO) | tr '[a-z]' '[A-Z]'
	ls -ls $(VIDUINO)

clean:
	rm -rf $(BUILD)

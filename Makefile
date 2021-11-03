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
VIDUINO = viduino-0.0.12.tar

COMPILE			= ~/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-
CC					= $(COMPILE)gcc
CXX					= $(COMPILE)g++
AS					= $(COMPILE)gcc -x assembler-with-cpp
AR					= $(COMPILE)ar rcs
LD					= $(COMPILE)ld
OBJCOPY			= $(COMPILE)objcopy
OBJDUMP			= $(COMPILE)objdump
SIZE				= $(COMPILE)size

BOOT				= cores/allwinner/sdk/boot
DRIVER			= cores/allwinner/sdk/driver
LIB					= cores/allwinner/sdk/lib
API					= cores/allwinner/api
CORES				= cores/allwinner

DEFINES			+= -D__ARM32_ARCH__=5 -D__ARM926EJS__ -D__ARM32__ -Wno-unused-function

ASFLAGS			:= -g -ggdb -Wall -O3
CFLAGS			:= -g -ggdb -Wall -O3
CPFLAGS			:= -g -ggdb -Wall -O3
LDFLAGS			:= -T variants/viduino_uno/f1c100s_32.ld -nostdlib
MCFLAGS			:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -specs=nosys.specs -mno-thumb-interwork

ASFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CPFLAGS			+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -fno-exceptions
LDFLAGS			+=	-Wl,-gc-sections -ffunction-sections -fdata-sections

LIBS 				:= -lgcc -lm -lc -lnosys

SRCDIRS			+= .
SDKSRCDIRS	= 

INCDIRS			+= $(BOOT) $(BOOT)/f1c100s
# SRCDIRS			+= $(BOOT)
SDKSRCDIRS	+= $(BOOT)

INCDIRS			+= $(LIB)
# SRCDIRS			+= $(LIB)
SDKSRCDIRS	+= $(LIB)

INCDIRS			+= $(DRIVER)
# SRCDIRS			+= $(DRIVER)
SDKSRCDIRS	+= $(DRIVER)

INCDIRS			+= $(API)
SRCDIRS			+= $(API)

INCDIRS			+= $(CORES) variants/viduino_uno
SRCDIRS			+= $(CORES)

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

INCDIRS			+= libraries/lvgl/src \
	libraries/lvgl/src/lvgl/src \
	libraries/lvgl/src/lvgl/src/lv_core \
	libraries/lvgl/src/lvgl/src/lv_draw \
	libraries/lvgl/src/lvgl/src/lv_font \
	libraries/lvgl/src/lvgl/src/lv_gpu \
	libraries/lvgl/src/lvgl/src/lv_hal \
	libraries/lvgl/src/lvgl/src/lv_misc \
	libraries/lvgl/src/lvgl/src/lv_themes \
	libraries/lvgl/src/lvgl/src/lv_widgets \
	libraries/lvgl/examples/lv_demo_music
SRCDIRS			+= libraries/lvgl/src/lvgl/src/lv_core \
	libraries/lvgl/src/lvgl/src/lv_draw \
	libraries/lvgl/src/lvgl/src/lv_font \
	libraries/lvgl/src/lvgl/src/lv_gpu \
	libraries/lvgl/src/lvgl/src/lv_hal \
	libraries/lvgl/src/lvgl/src/lv_misc \
	libraries/lvgl/src/lvgl/src/lv_themes \
	libraries/lvgl/src/lvgl/src/lv_widgets \
	libraries/lvgl/examples/lv_demo_music

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

SFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

SDKSFILES	:=	$(foreach dir, $(SDKSRCDIRS), $(wildcard $(dir)/*.S))
SDKCFILES	:=	$(foreach dir, $(SDKSRCDIRS), $(wildcard $(dir)/*.c))
SDKCPFILES	:=	$(foreach dir, $(SDKSRCDIRS), $(wildcard $(dir)/*.cpp))

INCLUDE	:=	$(patsubst %, -I %, $(INCDIRS))
LIBFILE	:=	$(patsubst %, %, $(LIBFILES))
SOBJS		:=	$(patsubst %, $(BUILD)/%, $(SFILES:.S=.o))
COBJS		:=	$(patsubst %, $(BUILD)/%, $(CFILES:.c=.o))
CPOBJS		:=	$(patsubst %, $(BUILD)/%, $(CPFILES:.cpp=.o))
OBJS = $(SOBJS) $(COBJS) $(CPOBJS)

SDKSOBJS		:=	$(patsubst %, $(BUILD)/%, $(SDKSFILES:.S=.o))
SDKCOBJS		:=	$(patsubst %, $(BUILD)/%, $(SDKCFILES:.c=.o))
SDKCPOBJS		:=	$(patsubst %, $(BUILD)/%, $(SDKCPFILES:.cpp=.o))
SDKOBJS = $(SDKSOBJS) $(SDKCOBJS) $(SDKCPOBJS)

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
	@$(AR) $(BUILD)/libsdk.a $(SDKOBJS)
	$(ECHO) "LINK $@"
	@$(CXX) $(LDFLAGS) -Wl,--cref,-Map=$@.map -o $@ $(BUILD)/libsdk.a $(OBJS) $(BUILD)/libsdk.a $(LIBFILE) $(LIBS)
	@$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	@$(OBJCOPY) -v -O binary $^ $@
	$(PYTHON) tools/lzmatool.py $(BUILD)/firmware.bin $(BUILD)/firmware.bin.lzma

write:
	$(PYTHON) tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin.lzma

write2:
	$(PYTHON) tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 $(BUILD)/firmware.bin

viduino: $(SDKOBJS)
	mkdir viduino
	cp -r cores libraries tools variants boards.txt platform.txt programmers.txt viduino
	$(AR) viduino/cores/allwinner/sdk/libsdk.a $(SDKOBJS)
	rm viduino/$(BOOT)/*.c viduino/$(BOOT)/*.S
	rm viduino/$(DRIVER)/*.c
	rm viduino/$(LIB)/*.c
	tar -cf $(VIDUINO) viduino 
	rm -r viduino
	sha256sum $(VIDUINO) | tr '[a-z]' '[A-Z]'
	ls -ls $(VIDUINO)

clean:
	rm -rf $(BUILD)

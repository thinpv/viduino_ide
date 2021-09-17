#
# Machine makefile
#

.PHONY:clean
.PHONY:write
.PHONY:mktool
.PHONY:dump
.PHONY:test

BUILD ?= build
RM = rm
ECHO = @echo
CP = cp
MKDIR = mkdir
SED = sed
PYTHON = python

CROSS_COMPILE = /home/thinpv/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/arm-none-eabi-
CC	          = $(CROSS_COMPILE)gcc
CXX	          = $(CROSS_COMPILE)g++
AS	          = $(CROSS_COMPILE)gcc -x assembler-with-cpp
AR	          = $(CROSS_COMPILE)ar rcs
LD	          = $(CROSS_COMPILE)ld
OBJCOPY	      = $(CROSS_COMPILE)objcopy
OBJDUMP       = $(CROSS_COMPILE)objdump
SIZE 		  = $(CROSS_COMPILE)size

MACHINE			= cores/allwinner/sdk/machine
DRIVER			= cores/allwinner/sdk/driver
LIB					= cores/allwinner/sdk/lib
FATFS				= cores/allwinner/sdk/fatfs
API					= cores/allwinner/api
CORES				= cores/allwinner

# DEFINES		+= -DLVGL
# DEFINES				+= -DLCD_RGB888

DEFINES		+= -D__ARM32_ARCH__=5 -D__ARM926EJS__ -D__ARM32__ -Wno-unused-function

ASFLAGS			:= -g -ggdb -Wall -O3
CFLAGS			:= -g -ggdb -Wall -O3
CPFLAGS			:= -g -ggdb -Wall -O3
LDFLAGS			:= -T variants/viduino_uno/f1c100s.ld -nostdlib
MCFLAGS		:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork

ASFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu11 $(DEFINES)
CPFLAGS		+=	-ffunction-sections -fdata-sections -ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti
LDFLAGS		+=	-Wl,-gc-sections 

LIBS 			:= -lgcc -lm -lc -lnosys

SRCDIRS		+= .

INCDIRS		+= $(MACHINE)/include $(MACHINE)/include/f1c100s
SRCDIRS		+= $(MACHINE)

INCDIRS		+= $(LIB) $(LIB)/include
SRCDIRS		+= $(LIB)

INCDIRS		+= $(DRIVER)/include
SRCDIRS		+= $(DRIVER)

INCDIRS		+= $(FATFS)
SRCDIRS		+= $(FATFS)

INCDIRS		+= $(API)
SRCDIRS		+= $(API)

INCDIRS		+= $(CORES)
SRCDIRS		+= $(CORES)

INCDIRS		+= libraries/FrameBuffer/src
SRCDIRS		+= libraries/FrameBuffer/src

INCDIRS		+= libraries/NS2009/src
SRCDIRS		+= libraries/NS2009/src

# INCDIRS		+= libraries/fatfs/src
# SRCDIRS		+= libraries/fatfs/src

# INCDIRS		+= libraries/png/libz
# SRCDIRS		+= libraries/png/libz

# INCDIRS		+= libraries/png/libpng
# SRCDIRS		+= libraries/png/libpng

INCDIRS		+= libraries/lv_arduino/src \
	libraries/lv_arduino/src/src \
	libraries/lv_arduino/src/src/lv_core \
	libraries/lv_arduino/src/src/lv_draw \
	libraries/lv_arduino/src/src/lv_font \
	libraries/lv_arduino/src/src/lv_gpu \
	libraries/lv_arduino/src/src/lv_hal \
	libraries/lv_arduino/src/src/lv_misc \
	libraries/lv_arduino/src/src/lv_themes \
	libraries/lv_arduino/src/src/lv_widgets \
	libraries/lv_arduino/examples/lv_demo_music
SRCDIRS		+= libraries/lv_arduino/src/src/lv_core \
	libraries/lv_arduino/src/src/lv_draw \
	libraries/lv_arduino/src/src/lv_font \
	libraries/lv_arduino/src/src/lv_gpu \
	libraries/lv_arduino/src/src/lv_hal \
	libraries/lv_arduino/src/src/lv_misc \
	libraries/lv_arduino/src/src/lv_themes \
	libraries/lv_arduino/src/src/lv_widgets \
	libraries/lv_arduino/examples/lv_demo_music

# INCDIRS		+= libraries/nes/src
# SRCDIRS		+= libraries/nes/src

# INCDIRS		+= libraries/png/png
# SRCDIRS		+= libraries/png/png

# LIBFILES	+= libraries/mjpegDecorder/jpeg_dec.a
# INCDIRS		+= libraries/mjpegDecorder \
# 	libraries/mjpegDecorder/videoplay \
# 	libraries/mjpegDecorder/test_jpeg
# SRCDIRS		+= libraries/mjpegDecorder \
# 	libraries/mjpegDecorder/videoplay

SFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CPFILES	:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.cpp))

INCLUDE	:=	$(patsubst %, -I %, $(INCDIRS))
LIBFILE	:=	$(patsubst %, %, $(LIBFILES))
SOBJS		:=	$(patsubst %, $(BUILD)/%, $(SFILES:.S=.o))
COBJS		:=	$(patsubst %, $(BUILD)/%, $(CFILES:.c=.o))
CPOBJS		:=	$(patsubst %, $(BUILD)/%, $(CPFILES:.cpp=.o))

OBJ = $(SOBJS) $(COBJS) $(CPOBJS)

OBJ_DIRS = $(sort $(dir $(OBJ)))

all: $(BUILD)/firmware.bin

$(OBJ): | $(OBJ_DIRS)
$(OBJ_DIRS):
	$(MKDIR) -p $@

$(BUILD)/%.o: %.S
	$(ECHO) "AS $<"
	$(AS) $(MCFLAGS) $(ASFLAGS) -c $< -o $@
	
$(BUILD)/%.o: %.c
	$(ECHO) "CC $<"
	$(CC) $(INCLUDE) $(MCFLAGS) $(CFLAGS) -MD -c $< -o $@

$(BUILD)/%.o: %.cpp
	$(ECHO) "CXX $<"
	$(CXX) $(INCLUDE) $(MCFLAGS) $(CPFLAGS) -MD -c $< -o $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(OBJCOPY) -v -O binary $^ $@
	#@echo Make header information for brom booting
	#@$(MKSUNXI) $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(CXX) $(LDFLAGS) -Wl,--cref,-Map=$@.map -o $@ $^ $(LIBFILE) $(LIBS)
	$(SIZE) $@

write:
	sudo sunxi-fel -p spiflash-write 0x80000 $(BUILD)/firmware.bin

write2:
	python3 tools/upload.py --port /dev/ttyUSB0 --baud 921600 write_flash 0x80000 build/firmware.bin

clean:
	rm -rf $(BUILD)

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
VIDUINO = viduino-0.0.8.tar

# CROSS_COMPILE = /opt/gcc-linaro-7.5.0-2019.12-x86_64_arm-eabi/bin/arm-eabi-
# CROSS_COMPILE = /home/thinpv/Downloads/gcc-arm-none-eabi-9-2020-q2-update/bin/arm-none-eabi-

CROSS_COMPILE = arm-none-eabi-
CC	          = $(CROSS_COMPILE)gcc
CXX	          = $(CROSS_COMPILE)g++
AS	          = $(CROSS_COMPILE)gcc -x assembler-with-cpp
AR	          = $(CROSS_COMPILE)ar rcs
LD	          = $(CROSS_COMPILE)ld
OBJCOPY	      = $(CROSS_COMPILE)objcopy
OBJDUMP       = $(CROSS_COMPILE)objdump
SIZE 		  = $(CROSS_COMPILE)size

DEFINES		+= -D__ARM32_ARCH__=5 -D__ARM926EJS__

# LVGL_DIR ?= ${shell pwd}/libraries
# LVGL_DIR_NAME ?= lv_arduino/src
# include libraries/lv_arduino/src/lvgl.mk

NS2009_DIR = ${shell pwd}/libraries/NS2009
include libraries/NS2009/src/ns2009.mk

# NOFRENDO_DIR:= ${shell pwd}/libraries/nofrendo
# include libraries/nofrendo/src/nofrendo.mk
# NOFRENDO_F1C100S_DIR:= ${shell pwd}/libraries/nofrendo/example/nofrendo-f1c100s
# include libraries/nofrendo/example/nofrendo-f1c100s/nofrendo-f1c100s.mk

CXXFLAGS	:= $(CFLAGS)
ASFLAGS		:= -g -Wall -Os -ffreestanding -std=gnu11 $(DEFINES)
CFLAGS		+= -g -Wall -Os -ffreestanding -std=gnu11 $(DEFINES)
CXXFLAGS	+= -g -Wall -Os -ffreestanding -std=gnu++11 $(DEFINES) -fno-rtti
LDFLAGS		:= -T variants/viduino_uno/f1c100s.ld -nostdlib -Wno-unused-function
MCFLAGS		:= -march=armv5te -mtune=arm926ej-s -mfloat-abi=soft -marm -mno-thumb-interwork

LIBS 			:= -lgcc -lm -lc -lnosys
CORES			:= cores/allwinner
SDKSRC		:= cores/allwinner/sdk
SDKINC		:= cores/allwinner/sdk
INCDIRS		+= -I$(CORES) -I$(CORES)/api
INCDIRS		+= -I$(SDKINC)/driver/include -I$(SDKINC)/machine/include -I$(SDKINC)/machine/include/f1c100s -I$(SDKINC)/lib -I$(SDKINC)/lib/include
INCDIRS		+= -Ilibraries/keyboard/src

SRC_C = $(CSRCS)

X_CSRCDIRS	:=	$(patsubst %, %, $(CSRCDIRS))
CFILES		:=	$(foreach dir, $(X_CSRCDIRS), $(wildcard $(dir)/*.c))
X_CXXSRCDIRS	:=	$(patsubst %, %, $(CXXSRCDIRS))
CXXFILES	:=	$(foreach dir, $(X_CXXSRCDIRS), $(wildcard $(dir)/*.cpp))

SRC_S = \
	$(SDKSRC)/machine/start.S \
	$(SDKSRC)/lib/memcmp.S \
	$(SDKSRC)/lib/memcpy.S \
	$(SDKSRC)/lib/memmove.S \
	$(SDKSRC)/lib/memset.S \
	$(SDKSRC)/lib/setjmp.S \
	$(SDKSRC)/lib/strcmp.S \
	$(SDKSRC)/lib/strncmp.S \

SRC_C += \
	$(SDKSRC)/machine/boot_main.c \
	$(SDKSRC)/machine/sys-clock.c \
	$(SDKSRC)/machine/sys-dram.c \
	$(SDKSRC)/machine/sys-print.c \
	$(SDKSRC)/machine/sys-spi-flash.c \
	$(SDKSRC)/machine/sys-mmu.c \
	$(SDKSRC)/machine/sys-init.c \
	$(SDKSRC)/machine/sys-delay.c \
	
# drivers
SRC_C += \
	$(SDKSRC)/driver/gpio.c \
	$(SDKSRC)/driver/uart.c \
	$(SDKSRC)/driver/timer.c \
	$(SDKSRC)/driver/irq.c \
	$(SDKSRC)/driver/irq_gpio.c \
	$(SDKSRC)/driver/pwm.c \
	$(SDKSRC)/driver/ccu.c \
	$(SDKSRC)/driver/fb.c \
	$(SDKSRC)/driver/i2c.c \
	$(SDKSRC)/driver/spi.c \
	$(SDKSRC)/driver/usb_phy.c \
	$(SDKSRC)/driver/usb_dev.c \
	$(SDKSRC)/driver/usb_desc.c \
	$(SDKSRC)/driver/ccu.c \
	$(SDKSRC)/driver/dma.c \
	$(SDKSRC)/driver/audio.c \
	$(SDKSRC)/driver/defe.c \
	$(SDKSRC)/driver/sdc.c \
	
	# $(SDKSRC)/driver/audio.c \

SRC_C += \
	$(SDKSRC)/lib/malloc.c \
	$(SDKSRC)/lib/dma.c \
	$(SDKSRC)/lib/printf.c \
	$(SDKSRC)/lib/libc/exit/abort.c \
	$(SDKSRC)/lib/libc/exit/assert.c \

# allwinner api
SRC_C += \
	$(CORES)/wiring_analog.c \
	$(CORES)/wiring_digital.c \

# SRC_C += \
# 	libraries/infones/src/InfoNES.c \
# 	libraries/infones/src/InfoNES_Main.c \
# 	libraries/infones/src/InfoNES_Mapper.c \
# 	libraries/infones/src/InfoNES_pAPU.c \
# 	libraries/infones/src/K6502.c \
# 	libraries/infones/src/rom.c

SRC_C += \
	libraries/keyboard/src/usb_keyboard.c

# allwinner core
SRC_CPP += \
	$(CORES)/api/Print.cpp \
	$(CORES)/maincpp.cpp \
	$(CORES)/Uart.cpp \
	$(CORES)/Wire.cpp \
	$(CORES)/Pins_arduino.cpp \
	$(CORES)/new.cpp \
	$(CORES)/hooks.cpp \
	$(CORES)/abi.cpp \
	arduino.cpp

# INCDIRS		+= -Ilibraries/arduino-goodix/src
# SRC_CPP += \
# 	libraries/arduino-goodix/src/Goodix.cpp

# INCDIRS		+= -Ilibraries/FT5xx6/src
# SRC_CPP += \
# 	libraries/FT5xx6/src/FT5xx6.cpp

#freertos
INCDIRS		+= -Icores/allwinner/sdk/freertos/include -Icores/allwinner/sdk/freertos/portable/GCC/ARM926EJ-S
SRC_C += \
	cores/allwinner/sdk/freertos/croutine.c \
	cores/allwinner/sdk/freertos/event_groups.c \
	cores/allwinner/sdk/freertos/list.c \
	cores/allwinner/sdk/freertos/queue.c \
	cores/allwinner/sdk/freertos/tasks.c \
	cores/allwinner/sdk/freertos/timers.c \
	cores/allwinner/sdk/freertos/portable/MemMang/heap_2.c \
	cores/allwinner/sdk/freertos/portable/GCC/ARM926EJ-S/port.c \
	cores/allwinner/sdk/freertos/portable/GCC/ARM926EJ-S/portISR.c \

OBJ_S   = $(addprefix $(BUILD)/, $(SRC_S:.S=.o))
OBJ_C   = $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ_CPP = $(addprefix $(BUILD)/, $(SRC_CPP:.cpp=.o))


OBJ = $(OBJ_S) $(OBJ_C) $(OBJ_CPP)
#  cores/allwinner/sdk/libsdk.a

# $(BUILD)/firmware_boot.bin:$(BUILD)/firmware.bin
# 	./tools/mksunxiboot/mksunxiboot $^ $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(OBJCOPY) -v -O binary $^ $@
	#@echo Make header information for brom booting
	#@$(MKSUNXI) $@

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(CXX) $(LDFLAGS) -Wl,--cref,-Map=$@.map -o $@ $^ $(LIBS)
	$(SIZE) $@

$(BUILD)/%.o: %.S
	$(ECHO) "AS $<"
	$(AS) $(MCFLAGS) $(ASFLAGS) -c -o $@ $<
	
$(BUILD)/%.o: %.s
	$(ECHO) "AS $<"
	$(AS) $(MCFLAGS) $(ASFLAGS) -c -o $@ $<

define compile_c
	$(ECHO) "CC $<"
	$(CC) $(INCDIRS) $(MCFLAGS) $(CFLAGS) -c -MD -o $@ $<
endef

define compile_cpp
	$(ECHO) "CXX $<"
	$(CXX) $(INCDIRS) $(MCFLAGS) $(CXXFLAGS) -c -MD -o $@ $<
endef

$(BUILD)/%.o: %.c
	$(call compile_c)

$(BUILD)/%.o: %.cpp
	$(call compile_cpp)

OBJ_DIRS = $(sort $(dir $(OBJ)))
$(OBJ): | $(OBJ_DIRS)
$(OBJ_DIRS):
	$(MKDIR) -p $@

lib: $(SDK_OBJ)
	$(AR) cores/allwinner/sdk/libsdk.a $(SDK_OBJ)

viduino: $(SDK_OBJ)
	$(AR) cores/allwinner/sdk/libsdk.a $(SDK_OBJ)
	mkdir viduino
	cp -r cores libraries tools variants boards.txt platform.txt programmers.txt viduino
	tar -cf $(VIDUINO) viduino 
	rm -r viduino
	sha256sum $(VIDUINO) | tr '[a-z]' '[A-Z]'
	ls -ls $(VIDUINO)

test:
	$(ECHO) $(SRC_C)

clean:
	find . -name "*.o"  | xargs rm -f
	find . -name "*.bin"  | xargs rm -f
	find . -name "*.elf"  | xargs rm -f
	find . -name "*.P"  | xargs rm -f
	rm -rf *.o *.bin *.a *.elf.map
	rm -rf $(BUILD)

write:
	sudo sunxi-fel -p spiflash-write 0 $(BUILD)/firmware.bin

write-arduino:
	python3 tools/upload.py --no-stub --port /dev/ttyUSB0 --baud 921600 write_flash 0x200000 build/firmware.bin

mktool:
	cd tools/mksunxiboot && make
	cd tools/mksunxi && make

dump:
	$(OBJDUMP) -S myboot.o | less

MKSUNXI		:= tools/mksunxi/mksunxi

mkboot:
	@echo Make header information for brom booting
	@$(MKSUNXI) $(BUILD)/firmware.bin

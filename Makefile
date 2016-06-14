# Makefile for STM32

CC = arm-none-eabi-gcc
LL = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
DUMP = arm-none-eabi-objdump
MKDIR = mkdir -p
RM = rm -f

# Flags
CFLAGS = -mcpu=cortex-m3 -mthumb -Wall -Werror -Iplib -Icore_lib 
CFLAGS += -DSTM32F10X_MD
LFLAGS = -g -v -Tlinker_script.ld -nostartfiles -Wl,--cref -mthumb -mcpu=cortex-m3 $(LIBS)


CFLAGS += -ffunction-sections -fdata-sections
LFLAGS += -Wl,-gc-sections

# Output files
ELF = blinky
D_ELF = blinky_debug

# Constants
MCU_FLASH_SIZE = 32768


C_FILES = $(wildcard *.c plib/*.c core_lib/*.c)
ASM_FILES = core_lib/startup_stm32f10x_md_vl.S

# Debug build settings
D_OBJECT_DIR = ./obj/Debug
D_TARGET_DIR = ./bin/Debug
D_TARGET = $(D_TARGET_DIR)/$(D_ELF)
D_TARGET_HEX = $(D_TARGET_DIR)/$(D_ELF).hex
D_TARGET_BIN = $(D_TARGET_DIR)/$(D_ELF).bin
D_TARGET_LIST = $(D_TARGET_DIR)/$(D_ELF).list
D_OBJECTS = $(patsubst %.c, $(D_OBJECT_DIR)/%.o, $(C_FILES))
D_OBJECTS += $(patsubst %.S, $(D_OBJECT_DIR)/%.o, $(ASM_FILES))
D_CFLAGS = -g -DDEBUG

# Release build settings
R_OBJECT_DIR = ./obj/Release
R_TARGET_DIR = ./bin/Release
R_TARGET = $(R_TARGET_DIR)/$(ELF)
R_TARGET_HEX = $(R_TARGET_DIR)/$(ELF).hex
R_TARGET_BIN = $(R_TARGET_DIR)/$(ELF).bin
R_TARGET_LIST = $(R_TARGET_DIR)/$(ELF).list
R_OBJECTS = $(patsubst %.c, $(R_OBJECT_DIR)/%.o, $(C_FILES))
R_OBJECTS += $(patsubst %.S, $(R_OBJECT_DIR)/%.o, $(ASM_FILES))
R_CFLAGS = -O2

# SHELL := /bin/bash

.PHONY: all clean prep debug release buildall rebuildall d_file_check

.PRECIOUS: $(D_TARGET) $(D_TARGET_HEX) $(D_OBJECTS) $(R_TARGET) $(R_TARGET_HEX) $(R_OBJECTS)

# Default build
all: prep rebuildall

debug: $(D_TARGET)

$(D_TARGET): $(D_OBJECTS)
	$(LL) $(LFLAGS) -Wl,-Map=$(D_TARGET).map -o $(D_TARGET) $^
	$(OBJCOPY) -O binary $(D_TARGET) $(D_TARGET_BIN)
	$(DUMP) -S $(D_TARGET) > $(D_TARGET_LIST)

$(D_OBJECT_DIR)/%.o: %.c
	echo Building $@
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(D_CFLAGS) -o $@ $<

$(D_OBJECT_DIR)/%.o: %.S
	echo Building $@
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(D_CFLAGS) -o $@ $<



#
# Release build rules
#
# NOTE:
# Read the note above in the "Debug build rules" comment block, it applies to
# the release build too.
#
release: $(R_TARGET)

$(R_TARGET): $(R_OBJECTS)
	$(LL) $(LFLAGS) -Wl,-Map=$(R_TARGET).map -o $(R_TARGET) $^
	$(OBJCOPY) -O binary $(R_TARGET) $(R_TARGET_BIN)
	$(DUMP) -S $(R_TARGET) > $(R_TARGET_LIST)

$(R_OBJECT_DIR)/%.o: %.c
	echo Building $@
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(R_CFLAGS) -o $@ $<

$(R_OBJECT_DIR)/%.o: %.S
	echo Building $@
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(R_CFLAGS) -o $@ $<

#
# Other rules
#
prep:
	$(MKDIR) $(D_OBJECT_DIR)
	$(MKDIR) $(D_TARGET_DIR)
	$(MKDIR) $(R_OBJECT_DIR)
	$(MKDIR) $(R_TARGET_DIR)

rebuildall: clean release debug

buildall: release debug

clean:
	$(RM) $(D_TARGET) $(D_TARGET_HEX) $(D_TARGET_BIN) $(D_TARGET_LIST) $(D_OBJECTS) $(R_TARGET) $(R_TARGET_HEX) $(R_TARGET_BIN) $(R_TARGET_LIST) $(R_OBJECTS)

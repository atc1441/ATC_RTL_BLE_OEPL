######################################
# target
######################################
TARGET = ATC_RTL_BLE_OEPL_8762_97

######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -O1

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
BIN_DIR = bin

######################################
# flash settings
######################################
COM_PORT = COM35

# This is a separate EL097R2CRN application build, but it intentionally reuses
# the existing RTL8762ESL SDK, loader, vendor system image and header tool.
BASE_8762    = ../../ATC_RTL_BLE_OEPL_8762ESL
SDK          = $(BASE_8762)/sdk
TOOL         = $(BASE_8762)/tool
LOADER       = $(BASE_8762)/gcc/rtl8762_loader.py
FW_B         = $(BASE_8762)/gcc/RTL8762E_FW_B.bin
SYSTEM_BLOB  = $(BASE_8762)/gcc/data_0x801000.bin
PREPEND_TOOL = $(BASE_8762)/gcc/prepend_header.exe

######################################
# source
######################################
# C sources
C_SOURCES =  \
$(SDK)/src/mcu/rtl876x/system_rtl876x.c \
$(SDK)/src/mcu/peripheral/rtl876x_io_dlps.c \
$(SDK)/src/mcu/peripheral/rtl876x_nvic.c \
$(SDK)/src/mcu/peripheral/rtl876x_pinmux.c \
$(SDK)/src/mcu/peripheral/rtl876x_gpio.c \
$(SDK)/src/mcu/peripheral/rtl876x_rcc.c \
$(SDK)/src/mcu/peripheral/rtl876x_uart.c \
$(SDK)/src/ble/profile/server/simple_ble_service.c \
$(SDK)/src/ble/profile/server/bas.c \
$(SDK)/src/ble/profile/client/ancs_client.c \
../src/custom_service.c \
../src/app_task.c \
../src/epd.c \
../src/main.c \
../src/peripheral_app.c \
../src/retarget.c \
../src/ble_cmd_handler.c \
../src/eeprom.c \
../src/drawing.c \
../src/battery.c \
../src/boot_screen.c \
../src/ota.c \
../src/mac_edit.c \
../src/zlib/uzlib.c \
$(SDK)/src/mcu/peripheral/rtl876x_adc.c \
$(SDK)/src/mcu/peripheral/rtl876x_aon_wdg.c \
$(SDK)/src/mcu/rtl876x/overlay_mgr.c \
# C sources END

# ASM sources
ASM_SOURCES = $(SDK)/src/mcu/rtl876x/arm/startup_rtl876x_gcc.s
# ASM sources END

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
OD = $(GCC_PATH)/$(PREFIX)objdump
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
OD = $(PREFIX)objdump
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m0plus

# fpu
FPU =

# float-abi
#FLOAT-ABI = -mfloat-abi=hard
FLOAT-ABI = -mfloat-abi=soft

# mcu
MCU = $(CPU) -mthumb  $(FLOAT-ABI)

# MACROS for gcc
# AS Defines
AS_DEFS =
# AS Defines END

# C Defines
C_DEFS =
# C Defines END

# AS Includes
AS_INCLUDES = \
# AS Includes END

# C Includes
C_INCLUDES =  \
-I$(SDK)/inc/bluetooth \
-I$(SDK)/inc/app \
-I$(SDK)/inc/bluetooth/gap \
-I$(SDK)/inc/bluetooth/gap/gap_lib \
-I$(SDK)/inc/bluetooth/profile \
-I$(SDK)/inc/bluetooth/profile/server \
-I$(SDK)/inc/bluetooth/profile/client \
-I$(SDK)/inc/os \
-I$(SDK)/inc/peripheral \
-I$(SDK)/inc/platform \
-I../src \
-I../config \
# C Includes END

# C Pre Includes
PRE_INCLUDE=  \
-include app_flags.h \
# C Pre Includes END

# C Pre Defines
PRE_DEFINE=  \
-D SUPPORT_ALONE_UPPERSTACK_IMG \
# C Pre Defines END

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) -specs=nano.specs $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -std=c99

# pre-include
ifneq ($(PRE_INCLUDE), )
CFLAGS +=$(PRE_INCLUDE)
endif
ifneq ($(PRE_DEFINE), )
CFLAGS +=$(PRE_DEFINE)
endif

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = app.ld.gen

LIBS = -lc -lm -lnosys

# Libraries
LIBDIR = \
$(SDK)/lib/rom_symbol_gcc.axf \
$(SDK)/lib/adc.a \
$(SDK)/lib/gap_utils.a \
$(SDK)/lib/bee3_sdk.a \
# Libraries END

LDFLAGS = $(MCU) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections  -specs=nano.specs

# Default action: Build all targets.
#
# Optional: Generate a disassembly file for debugging.
# To enable this feature, remove the leading '#' from the command: $(OD) -D -S build/$(TARGET).elf > bin/$(TARGET).dis
# Note:
#   The '-S' flag interleaves source code with disassembled output, which is useful
#   for correlating the source with the corresponding machine code. However, since the
#   output file will contain source code, ensure that you protect its confidentiality.
.PHONY : all rm_app_bin flash flash-pt flash-all flash-all-pt
all:rm_app_bin $(BUILD_DIR)/$(TARGET).elf $(BIN_DIR)/$(TARGET).bin
	$(PREPEND_TOOL) -b 12 -t app_code -p $(BIN_DIR)/$(TARGET).bin -m 0 -i "mp.ini" -c sha256


# Override port: make flash COM_PORT=COM3
# App-only flash: update app image at 0x832000 (normal workflow after first flash)
flash: all
	python $(LOADER) flash-write -p $(COM_PORT) -f $(FW_B) --addr 0x832000 -i $(BIN_DIR)/$(TARGET).bin --reboot

# Like flash, but keeps UART open after reboot (Ctrl+C to exit)
flash-pt: all
	python $(LOADER) flash-write -p $(COM_PORT) -f $(FW_B) --addr 0x832000 -i $(BIN_DIR)/$(TARGET).bin --reboot --passthrough

# Full flash: system blob at 0x801000 + app at 0x832000 (first time / after SDK update)
flash-all: all
	python $(LOADER) flash-write -p $(COM_PORT) -f $(FW_B) --addr 0x801000 -i $(SYSTEM_BLOB)
	python $(LOADER) flash-write -p $(COM_PORT) -f $(FW_B) --addr 0x832000 -i $(BIN_DIR)/$(TARGET).bin --reboot

flash-all-pt: all
	python $(LOADER) flash-write -p $(COM_PORT) -f $(FW_B) --addr 0x801000 -i $(SYSTEM_BLOB)
	python $(LOADER) flash-write -p $(COM_PORT) -f $(FW_B) --addr 0x832000 -i $(BIN_DIR)/$(TARGET).bin --reboot --passthrough

# Remove the old app.bin to allow MAKE to regenerate app.bin from the elf file,
# ensuring that the app.bin passed to the prepend_header script is plain-text.
rm_app_bin:
	rm -rf $(BIN_DIR)/$(TARGET).bin
#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c  $(dir $(C_SOURCES))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(ASFLAGS) $< -o $@

$(LDSCRIPT):
	$(CC) -E -P -x c $(C_INCLUDES) $(basename $@) -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile | $(LDSCRIPT)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BIN_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BIN_DIR)
	$(BIN) $< $@ --remove-section=App.trace

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
	-rm -fR $(BIN_DIR)
	-rm -f $(LDSCRIPT)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

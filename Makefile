# Build each AVR lab program separately; each source file defines its own main().
MCU ?= atmega644p
F_CPU ?= 12000000UL
BUILD_DIR ?= build

CC = avr-gcc
OBJCOPY = avr-objcopy

CPPFLAGS += -DF_CPU=$(F_CPU) -Icommon
CFLAGS += -std=gnu11 -Os -Wall -Wextra -mmcu=$(MCU)
LDFLAGS += -mmcu=$(MCU)

PROGRAMS := \
    Digital-IO/counter \
    PWM/tone \
    ADC/voltage \
    ADC/proximity \
    Interrupts/timer \
    Interrupts/adc \
    Interrupts/external

ELFS := $(addprefix $(BUILD_DIR)/,$(addsuffix .elf,$(PROGRAMS)))
HEXS := $(ELFS:.elf=.hex)

.PHONY: all clean list
.SECONDARY: $(ELFS)
all: $(HEXS)

# Floating-point printf is required by the two ADC serial-output examples.
$(BUILD_DIR)/ADC/voltage.elf $(BUILD_DIR)/ADC/proximity.elf: LDLIBS += -Wl,-u,vfprintf -lprintf_flt -lm

# Each program is a standalone source file, plus the optional header helper.
$(BUILD_DIR)/%.elf: %.c common/debug.h Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LDLIBS)

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

list:
	@printf '%s\n' $(HEXS)

clean:
	rm -rf $(BUILD_DIR)

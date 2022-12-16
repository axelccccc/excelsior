##### VARIABLES #####

include ../../make.config

# General build info (arch, triple, etc.)
ARCHDIR=arch/$(TARGET_ARCH)

BUILD_DIR=../../build/bootloader

BOOTLOADER_SRCS := $(wildcard $(ARCHDIR)/*.asm)
BOOTLOADER_OBJS := $(patsubst $(ARCHDIR)/%.asm, $(BUILD_DIR)/%.o, $(BOOTLOADER_SRCS))

#####################

###### TARGETS ######

all: $(BOOTLOADER_OBJS)

clean:
	@find $(BUILD_DIR) -type f -name '*.o' -delete

.PHONY: all build_dir clean

build_dir:
	@mkdir -p $(BUILD_DIR)

include $(ARCHDIR)/make.config # Architecture-specific build info (CFLAGS, etc.)

$(BUILD_DIR)/%.o: $(ARCHDIR)/%.asm build_dir
	@nasm -I $(ARCHDIR) -f elf $< -F dwarf -g -o $@
	@ld $(LD_ARCH_FLAGS) -T $(ARCHDIR)/$(BOOTLOADER_LDS) $@ -o $@.elf
	@objcopy -O binary $(BUILD_DIR)/bootloader.o.elf $@

#####################
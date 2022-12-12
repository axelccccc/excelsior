include ../../make.config

ARCHDIR=arch/$(TARGET_ARCH)

BUILD_DIR=../../build/bootloader

BOOTLOADER_SRCS := $(wildcard $(ARCHDIR)/*.asm)
BOOTLOADER_OBJS := $(patsubst $(ARCHDIR)/%.asm, $(BUILD_DIR)/%.o, $(BOOTLOADER_SRCS))

all: $(BOOTLOADER_OBJS)

clean:
	@find $(BUILD_DIR) -type f -name '*.o' -delete

.PHONY: build_dir clean

build_dir:
	@mkdir -p $(BUILD_DIR)

include $(ARCHDIR)/make.config

$(BUILD_DIR)/%.o: $(ARCHDIR)/%.asm build_dir
	@nasm -I $(ARCHDIR) -f elf $< -F dwarf -g -o $@
	@ld $(LD_ARCH_FLAGS) -T $(ARCHDIR)/$(BOOTLOADER_LDS) $@ -o $@.elf
	@objcopy -O binary $(BUILD_DIR)/bootloader.o.elf $@
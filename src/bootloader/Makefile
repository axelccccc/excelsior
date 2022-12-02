BUILD_DIR=../../build/bootloader

BOOTLOADER_SRCS := $(wildcard *.asm)
BOOTLOADER_OBJS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(BOOTLOADER_SRCS))

all: $(BOOTLOADER_OBJS)

clean:
	@rm $(BUILD_DIR)/*

.PHONY: build_dir clean

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.asm build_dir
	@nasm -f elf $< -F dwarf -g -o $@
	@ld -m elf_i386 -T bootloader.lds $@ -o $@.elf
	@objcopy -O binary $(BUILD_DIR)/bootloader.o.elf $@
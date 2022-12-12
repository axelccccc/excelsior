# VARIABLES

## FLAGS

# CFLAGS ?= -g -Wall -masm=intel

## PATHS

BUILD_DIR=build
SRC_DIR=src

# bear generates a compile_commands.json file
# for clangd to use
BEAR=$(shell which bear)
BEAR_OUTPUT=$(BUILD_DIR)/compile_commands.json

## OBJECTS

BOOTLOADER=$(BUILD_DIR)/bootloader/bootloader.o
KERNEL=$(BUILD_DIR)/kernel/kernel

DISK_IMG=disk.img
DISK_PATH=$(BUILD_DIR)/$(DISK_IMG)

# TARGETS

all: make_tracked

clean:
	make -C $(SRC_DIR)/bootloader clean
	make -C $(SRC_DIR)/kernel clean
	make -C $(SRC_DIR)/libc clean
	rm $(DISK_PATH)
	rm $(BEAR_OUTPUT)

.PHONY: disk bootloader kernel clean build_dir compile_commands

# this target does the whole compilation process
# tracked by bear, if installed, to generate compile_commands.json
# for clangd to use
make_tracked:
ifeq ($(strip $(BEAR)),)
	make disk
else
	@$(BEAR) --output $(BEAR_OUTPUT) -- make disk
endif

bootloader:
	make -C $(SRC_DIR)/bootloader

kernel: libc
	make -C $(SRC_DIR)/kernel

libc:
	make -C $(SRC_DIR)/libc

# Create a 1.4mb floppy disk image
# of the bootloader
# conv=notrunc: preserve original size of disk image

disk: bootloader kernel libc
	@dd if=/dev/zero 	 of=$(DISK_PATH) bs=512 count=2880
	@dd if=$(BOOTLOADER) of=$(DISK_PATH) bs=512 count=1 seek=0 conv=notrunc
	@dd if=$(KERNEL) 	 of=$(DISK_PATH) bs=512 count=$$(($(shell stat --printf="%s" $(KERNEL))/512)) seek=1 conv=notrunc
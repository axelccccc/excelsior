# VARIABLES

## FLAGS

CFLAGS ?= -g -Wall -masm=intel

## PATHS

BUILD_DIR=build
SRC_DIR=src

## OBJECTS

BOOTLOADER=$(BUILD_DIR)/bootloader/bootloader.o
KERNEL=$(BUILD_DIR)/kernel/kernel

DISK_IMG=disk.img
DISK_PATH=$(BUILD_DIR)/$(DISK_IMG)

# TARGETS

all: disk

clean:
	make -C $(SRC_DIR)/bootloader clean
	make -C $(SRC_DIR)/kernel clean
	rm $(DISK_PATH)

.PHONY: disk bootloader kernel clean build_dir

bootloader:
	make -C $(SRC_DIR)/bootloader

kernel:
	make -C $(SRC_DIR)/kernel

# Create a 1.4mb floppy disk image
# of the bootloader
# conv=notrunc: preserve original size of disk image

disk: bootloader kernel
	@dd if=/dev/zero 	 of=$(DISK_PATH) bs=512 count=2880
	@dd if=$(BOOTLOADER) of=$(DISK_PATH) bs=512 count=1 seek=0 conv=notrunc
	@dd if=$(KERNEL) 	 of=$(DISK_PATH) bs=512 count=$$(($(shell stat --printf="%s" $(KERNEL))/512)) seek=1 conv=notrunc
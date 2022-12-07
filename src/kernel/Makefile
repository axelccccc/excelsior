BUILD_DIR=../../build/kernel
KERNEL=$(BUILD_DIR)/kernel

CC=gcc
CFLAGS+=-ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -std=gnu99 -fno-pic
INCLUDE_DIRS=-Iinclude
SOURCE_DIRS=src

KERNEL_SRCS := $(wildcard $(SOURCE_DIRS)/*.c)
KERNEL_OBJS := $(patsubst $(SOURCE_DIRS)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_SRCS))

all: $(KERNEL)

clean:
	@rm $(KERNEL_OBJS)
	@rm $(KERNEL)

.PHONY: all clean build_dir

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIRS)/%.c
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(KERNEL): $(KERNEL_OBJS)
	@ld -m elf_i386 -T kernel.lds --nmagic $(KERNEL_OBJS) -o $@
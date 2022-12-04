BUILD_DIR=../../build/kernel
KERNEL=$(BUILD_DIR)/kernel

CC ?= gcc
# CFLAGS+=-ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3
# CFLAGS+=-ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -std=gnu99
CFLAGS+=-ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -std=gnu99 -fno-pic

KERNEL_SRCS := $(wildcard *.c)
KERNEL_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(KERNEL_SRCS))

all: $(KERNEL)

clean:
	@rm $(KERNEL_OBJS)
	@rm $(KERNEL)

.PHONY: all clean build_dir

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(KERNEL_OBJS)
	@ld -m elf_i386 -T kernel.lds --nmagic $(KERNEL_OBJS) -o $@
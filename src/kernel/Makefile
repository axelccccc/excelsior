BUILD_DIR=../../build/kernel
KERNEL=$(BUILD_DIR)/kernel

TRIPLE=i686-linux-gnu-
CC=$(TRIPLE)gcc
LD=$(TRIPLE)ld
# -masm=intel ? not compatible with asm.c (GAS syntax)
CFLAGS+=-g -Wall -ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -std=gnu11 -fno-pic
INCLUDE_DIRS=-Iinclude
SOURCE_DIRS=src

CRT_BEGIN_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRT_END_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

KERNEL_SRCS := $(wildcard $(SOURCE_DIRS)/*.c)
KERNEL_OBJS := $(patsubst $(SOURCE_DIRS)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_SRCS))

LINK_LIST=\
$(BUILD_DIR)/crti.o \
$(BUILD_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(BUILD_DIR)/crtend.o \
$(BUILD_DIR)/crtn.o

all: $(KERNEL)

clean:
	@rm $(LINK_LIST)
	@rm $(BUILD_DIR)/*.d
	@rm $(KERNEL)

.PHONY: all clean build_dir

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIRS)/%.c
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@



$(BUILD_DIR)/crti.o: $(SOURCE_DIRS)/crti.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(BUILD_DIR)/crtn.o: $(SOURCE_DIRS)/crtn.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@



$(BUILD_DIR)/crtbegin.o:
	@cp $(CRT_BEGIN_OBJ) $(BUILD_DIR)/crtbegin.o

$(BUILD_DIR)/crtend.o:
	@cp $(CRT_END_OBJ) $(BUILD_DIR)/crtend.o



$(KERNEL): $(LINK_LIST)
	@$(LD) -m elf_i386 -T kernel.lds --nmagic $(LINK_LIST) -o $@
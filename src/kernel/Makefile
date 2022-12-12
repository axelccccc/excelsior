BUILD_DIR=../../build/kernel
KERNEL=$(BUILD_DIR)/kernel
SYSROOT=../../build/sysroot

TRIPLE=i686-elf-
CC=$(TRIPLE)gcc
LD=$(TRIPLE)ld
# -masm=intel ? not compatible with asm.c (GAS syntax)
CFLAGS+=-g -Wall -ffreestanding -gdwarf-4 -m32 -ggdb3 -std=gnu11 -fno-pic -D__is_kernel --sysroot=$(SYSROOT)
INCLUDE_DIRS=-isystem=/usr/include -Iinclude
LIBS=-nostdlib -lk -lgcc
SOURCE_DIRS=src

CRT_BEGIN_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRT_END_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

KERNEL_SRCS := $(wildcard $(SOURCE_DIRS)/*.c)
KERNEL_OBJS := $(patsubst $(SOURCE_DIRS)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_SRCS))

OBJS=\
$(BUILD_DIR)/crti.o \
$(BUILD_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(BUILD_DIR)/crtend.o \
$(BUILD_DIR)/crtn.o

LINK_LIST=\
$(BUILD_DIR)/crti.o \
$(BUILD_DIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(BUILD_DIR)/crtend.o \
$(BUILD_DIR)/crtn.o

all: build_dir $(KERNEL)

clean:
	@rm $(LINK_LIST)
	@rm $(BUILD_DIR)/*.d
	@rm $(KERNEL)

.PHONY: all clean build_dir

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIRS)/%.c
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@



$(BUILD_DIR)/crti.o: $(SOURCE_DIRS)/crti.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@

$(BUILD_DIR)/crtn.o: $(SOURCE_DIRS)/crtn.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@



$(BUILD_DIR)/crtbegin.o:
	@cp $(CRT_BEGIN_OBJ) $(BUILD_DIR)/crtbegin.o

$(BUILD_DIR)/crtend.o:
	@cp $(CRT_END_OBJ) $(BUILD_DIR)/crtend.o

-include $(BUILD_DIR)/*.d

$(KERNEL): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -T kernel.lds $(LINK_LIST) -o $@
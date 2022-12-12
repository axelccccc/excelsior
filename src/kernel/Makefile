include ../../make.config

ARCHDIR=arch/$(TARGET_ARCH)

BUILD_DIR=../../build/kernel
KERNEL=$(BUILD_DIR)/kernel
SYSROOT=../../build/sysroot
LIBK_SRC_DIR=../libc

CC=$(TRIPLE)gcc
LD=$(TRIPLE)ld

include $(ARCHDIR)/make.config

# -masm=intel ? not compatible with asm.c (GAS syntax)
CFLAGS+=$(ARCH_CFLAGS) -g -Wall -ffreestanding -gdwarf-4 -ggdb3 -std=gnu11 -fno-pic -D__is_kernel --sysroot=$(SYSROOT)
INCLUDE_DIRS=-isystem=/usr/include -Iinclude -I$(ARCHDIR) -I$(LIBC_SRC_DIR)/include -I$(LIBC_SRC_DIR)/$(ARCHDIR)
LIBS=-nostdlib -lk -lgcc
SOURCE_DIRS=src

CRT_BEGIN_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRT_END_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

KERNEL_SRCS := $(wildcard $(SOURCE_DIRS)/*.c)
KERNEL_OBJS := $(patsubst $(SOURCE_DIRS)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_SRCS))

ARCH_ASM_SRCS := $(wildcard $(ARCHDIR)/*.S)
ARCH_ASM_OBJS := $(patsubst $(ARCHDIR)/%.S, $(BUILD_DIR)/arch/%.o, $(ARCH_ASM_SRCS))
ARCH_ASM_OBJS := $(filter-out $(BUILD_DIR)/arch/crti.o, $(ARCH_ASM_OBJS))
ARCH_ASM_OBJS := $(filter-out $(BUILD_DIR)/arch/crtn.o, $(ARCH_ASM_OBJS))
ARCH_C_SRCS := $(wildcard $(ARCHDIR)/*.c)
ARCH_C_OBJS := $(patsubst $(ARCHDIR)/%.c, $(BUILD_DIR)/arch/%.o, $(ARCH_C_SRCS))
ARCH_OBJS=$(ARCH_ASM_OBJS) $(ARCH_C_OBJS)

OBJS=\
$(BUILD_DIR)/arch/crti.o \
$(BUILD_DIR)/crtbegin.o \
$(ARCH_OBJS) \
$(KERNEL_OBJS) \
$(BUILD_DIR)/crtend.o \
$(BUILD_DIR)/arch/crtn.o

LINK_LIST=\
$(BUILD_DIR)/arch/crti.o \
$(BUILD_DIR)/crtbegin.o \
$(ARCH_OBJS) \
$(KERNEL_OBJS) \
$(LIBS) \
$(BUILD_DIR)/crtend.o \
$(BUILD_DIR)/arch/crtn.o

all: build_dir $(KERNEL)

clean:
	@rm $(OBJS)
	@rm $(BUILD_DIR)/*.d
	@rm $(BUILD_DIR)/arch/*.d
	@rm $(KERNEL)

.PHONY: all clean build_dir

build_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/arch

$(BUILD_DIR)/arch/%.o: $(ARCHDIR)/%.c
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIRS)/%.c
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@



$(BUILD_DIR)/arch/%.o: $(ARCHDIR)/%.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIRS)/%.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@



$(BUILD_DIR)/crtbegin.o:
	@cp $(CRT_BEGIN_OBJ) $(BUILD_DIR)/crtbegin.o

$(BUILD_DIR)/crtend.o:
	@cp $(CRT_END_OBJ) $(BUILD_DIR)/crtend.o



-include $(BUILD_DIR)/*.d
-include $(BUILD_DIR)/arch/*.d

$(KERNEL): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -T kernel.lds $(LINK_LIST) -o $@
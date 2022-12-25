##### VARIABLES #####

include ../../make.config # General build info (arch, triple, etc.)

ARCHDIR=arch/$(TARGET_ARCH)

BUILD_DIR=../../build/kernel
KERNEL=$(BUILD_DIR)/kernel
SYSROOT=../../build/sysroot
LIBC_SRC_DIR=../libc

CC=$(TRIPLE)gcc
LD=$(TRIPLE)ld

# temporary include of the gcc library headers path (for clangd)
GCC_LIB_INCLUDE_PATH=$(shell which $(CC) | xargs dirname)/../lib/gcc/i686-elf/12.2.0/include

include $(ARCHDIR)/make.config # Architecture-specific build info (CFLAGS, etc.)

# -masm=intel ? not compatible with asm.c (GAS syntax)

# w/ sysroot (temporarily unused)

# CFLAGS+=$(ARCH_CFLAGS) -g -Wall -ffreestanding -gdwarf-4 -ggdb3 -std=gnu11 -fno-pic -D__is_kernel --sysroot=$(SYSROOT)
# INCLUDE_DIRS=-isystem=/usr/include -Iinclude -I$(ARCHDIR) -I$(LIBC_SRC_DIR)/include -I$(LIBC_SRC_DIR)/$(ARCHDIR)
# LIBS=-nostdlib -lk -lgcc

# w/o sysroot
CFLAGS+=$(ARCH_CFLAGS) -g -Wall -Wextra -ffreestanding -gdwarf-4 -ggdb3 -std=gnu99 -fno-pic -fstack-protector -D__is_kernel --sysroot=$(SYSROOT)

# production flags (ignore for now)
# CFLAGS+=$(ARCH_CFLAGS) -O3 -Wall -ffreestanding -std=gnu99 -fno-pic -fstack-protector -D__is_kernel --sysroot=$(SYSROOT)

# INCLUDE_DIRS=-Iinclude -I$(ARCHDIR) -I$(LIBC_SRC_DIR)/include -I$(LIBC_SRC_DIR)/$(ARCHDIR)
# temporary include of the gcc library headers path (for clangd)
INCLUDE_DIRS=-isystem $(GCC_LIB_INCLUDE_PATH) -isystem $(GCC_LIB_INCLUDE_PATH)-fixed -Iinclude -I$(ARCHDIR) -I$(LIBC_SRC_DIR)/include -I$(LIBC_SRC_DIR)/$(ARCHDIR)
LIBS=-L$(BUILD_DIR)/../libc -nostdlib -lk -lgcc

SOURCE_DIRS=src

CRT_BEGIN_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRT_END_OBJ=$(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

KERNEL_SRCS := $(wildcard $(SOURCE_DIRS)/*.c)
KERNEL_OBJS := $(patsubst $(SOURCE_DIRS)/%.c, $(BUILD_DIR)/%.o, $(KERNEL_SRCS))

ARCH_ASM_SRCS := $(wildcard $(ARCHDIR)/*.S)
ARCH_NASM_SRCS := $(wildcard $(ARCHDIR)/*.asm)
ARCH_ASM_OBJS := $(patsubst $(ARCHDIR)/%.S, $(BUILD_DIR)/arch/%.o, $(ARCH_ASM_SRCS))
ARCH_ASM_OBJS := $(filter-out $(BUILD_DIR)/arch/crti.o, $(ARCH_ASM_OBJS))
ARCH_ASM_OBJS := $(filter-out $(BUILD_DIR)/arch/crtn.o, $(ARCH_ASM_OBJS))
ARCH_NASM_OBJS := $(patsubst $(ARCHDIR)/%.asm, $(BUILD_DIR)/arch/%.asm.o, $(ARCH_NASM_SRCS))
# ARCH_ASM_OBJS := $(ARCH_ASM_OBJS) $(ARCH_NASM_OBJS)
ARCH_C_SRCS := $(wildcard $(ARCHDIR)/*.c)
ARCH_C_OBJS := $(patsubst $(ARCHDIR)/%.c, $(BUILD_DIR)/arch/%.o, $(ARCH_C_SRCS))
ARCH_OBJS=$(ARCH_ASM_OBJS) $(ARCH_NASM_OBJS) $(ARCH_C_OBJS)

OBJS=\
$(BUILD_DIR)/arch/crti.o \
$(BUILD_DIR)/crtbegin.o \
$(ARCH_OBJS) \
$(KERNEL_OBJS) \
$(BUILD_DIR)/crtend.o \
$(BUILD_DIR)/arch/crtn.o

# Link list is made separately as it incorporates
# linking of libraries and can't be used as a 
# make dependency 
LINK_LIST=\
$(BUILD_DIR)/arch/crti.o \
$(BUILD_DIR)/crtbegin.o \
$(ARCH_OBJS) \
$(KERNEL_OBJS) \
$(LIBS) \
$(BUILD_DIR)/crtend.o \
$(BUILD_DIR)/arch/crtn.o

#####################

###### TARGETS ######

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

# Objects

$(BUILD_DIR)/arch/%.o: $(ARCHDIR)/%.c
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIRS)/%.c
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@

# .S will be used for `as` assembly
$(BUILD_DIR)/arch/%.o: $(ARCHDIR)/%.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@

# .asm will be used for NASM assembly
$(BUILD_DIR)/arch/%.asm.o: $(ARCHDIR)/%.asm
	@nasm -f elf -MD -g -F dwarf -o $@ $<

$(BUILD_DIR)/%.o: $(SOURCE_DIRS)/%.S
	@$(CC) -MD $(CFLAGS) $(INCLUDE_DIRS) -c $< $(LIBS) -o $@

$(BUILD_DIR)/crtbegin.o:
	@cp $(CRT_BEGIN_OBJ) $(BUILD_DIR)/crtbegin.o

$(BUILD_DIR)/crtend.o:
	@cp $(CRT_END_OBJ) $(BUILD_DIR)/crtend.o

# Dependency info for automatic change detection (-MD)

-include $(BUILD_DIR)/*.d
-include $(BUILD_DIR)/arch/*.d

# Kernel final linking
# & extract debug symbol from binary
# to minimize size of kernel binary

$(KERNEL): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -T kernel.lds $(LINK_LIST) -o $@
	@objcopy --only-keep-debug $(KERNEL) $(KERNEL).sym
	@objcopy --strip-debug $(KERNEL)

# Installation targets

install-headers:
	@mkdir -p $(SYSROOT)/usr/include
	@cp -r include/* $(SYSROOT)/usr/include

install-kernel: $(KERNEL)
	@mkdir -p $(SYSROOT)/boot
	@cp $(KERNEL) $(SYSROOT)/boot/kernel

install: install-headers install-kernel

#####################
BUILD_DIR=../../build/libc
SYSROOT=../../build/sysroot

TRIPLE=i686-elf-
CC=$(TRIPLE)gcc
LD=$(TRIPLE)ld
AR=$(TRIPLE)ar

CFLAGS+=-g -Wall -ffreestanding -nostdlib -gdwarf-4 -m32 -ggdb3 -std=gnu11
LIBC_CFLAGS=$(CFLAGS) -D__is_libc
LIBK_CFLAGS=$(CFLAGS) -D__is_libk
INCLUDE_DIRS=-Iinclude

LIBC_SRCS := $(shell find . -iname '*.c' | sed 's/^\.\///')
LIBC_ASM_SRCS := $(shell find . -iname '*.S' | sed 's/^\.\///')
LIBC_OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(LIBC_SRCS))
LIBC_ASM_OBJS := $(patsubst %.S, $(BUILD_DIR)/%.o, $(LIBC_ASM_SRCS))

# list of directories to create for object files paths
OBJ_DIRS := $(sort $(dir $(LIBC_OBJS) $(LIBC_ASM_OBJS)))

LIBK_OBJS := $(LIBC_OBJS:.o=.libk.o)
LIBK_ASM_OBJS := $(LIBC_ASM_OBJS:.o=.libk.o)

all: build_dir $(BUILD_DIR)/libk.a install # $(BUILD_DIR)/libc.a 

clean:
# remove libk object files if link has been built
ifneq ($(wildcard $(BUILD_DIR)/libk.a),)
	@rm $(LIBK_OBJS) $(LIBK_ASM_OBJS)
	@rm $(BUILD_DIR)/libk.a
endif
# remove libc object files if link has been built
ifneq ($(wildcard $(BUILD_DIR)/libc.a),)
	@rm $(LIBC_OBJS) $(LIBC_ASM_OBJS)
	@rm $(BUILD_DIR)/libc.a
endif
# remove all .d files in all subdirectories of build
	@find $(BUILD_DIR) -name '*.d' -type f -delete

.PHONY: all clean build_dir install-headers install-libs install

build_dir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIRS)

# currently empty, no hosted environment
$(BUILD_DIR)/libc.a: $(LIBC_OBJS) $(LIBC_ASM_OBJS)
	@touch $@
# @$(TRIPLE)ar rcs $@ $^

$(BUILD_DIR)/libk.a: $(LIBK_OBJS) $(LIBK_ASM_OBJS)
	@$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: %.c
	@$(CC) -MD -c $< -o $@ $(LIBC_CFLAGS) $(INCLUDE_DIRS)

$(BUILD_DIR)/%.o: %.S
	@$(CC) -MD -c $< -o $@ $(LIBC_CFLAGS) $(INCLUDE_DIRS)

$(BUILD_DIR)/%.libk.o: %.c
	@$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(INCLUDE_DIRS)

$(BUILD_DIR)/%.libk.o: %.S
	@$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(INCLUDE_DIRS)

# $(LIBC_OBJS): $(LIBC_SRCS)
# 	@$(CC) -MD -c $< -o $@ $(LIBC_CFLAGS) $(INCLUDE_DIRS)

# $(LIBC_ASM_OBJS): $(LIBC_ASM_SRCS)
# 	@$(CC) -MD -c $< -o $@ $(LIBC_CFLAGS) $(INCLUDE_DIRS)

# $(LIBK_OBJS): $(LIBC_SRCS)
# 	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(INCLUDE_DIRS)

# $(LIBK_ASM_OBJS): $(LIBC_ASM_SRCS)
# 	@$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(INCLUDE_DIRS)

-include $(LIBC_OBJS:.o=.d)
-include $(LIBC_ASM_OBJS:.o=.d)
-include $(LIBK_OBJS:.libk.o=.libk.d)
-include $(LIBK_ASM_OBJS:.libk.o=.libk.d)

install-headers:
	@mkdir -p $(SYSROOT)/usr/include
	@cp -r include/* $(SYSROOT)/usr/include

install-libs: $(BUILD_DIR)/libk.a
	@mkdir -p $(SYSROOT)/usr/lib
	@cp $(BUILD_DIR)/libk.a $(SYSROOT)/usr/lib

install: install-headers install-libs
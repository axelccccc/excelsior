BUILD_DIR=../../build/kernel

KERNEL_SRCS := $(wildcard *.asm)
KERNEL_OBJS := $(patsubst %.asm, $(BUILD_DIR)/%.o, $(KERNEL_SRCS))

all: $(KERNEL_OBJS)

.PHONY: build_dir

build_dir:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.asm build_dir
	@nasm -f bin $< -o $@
CFLAGS ?= -g -Wall -masm=intel

all: bin/hello bin/disk.img

clean:
	@rm hello

.PHONY: all clean


bin/hello: hello.c
	@gcc $(CFLAGS) -o $@ $^


# compile bootloader

bin/bootloader: bootloader.asm
	@nasm -f bin -o $@ $^


# Create a 1.4mb floppy disk image
# of the bootloader
# conv=notrunc: preserve original size of disk image

bin/disk.img: bin/bootloader
	@dd if=/dev/zero of=$@ bs=512 count=2880
	@dd if=$^ of=$@ conv=notrunc bs=512 count=1 seek=0
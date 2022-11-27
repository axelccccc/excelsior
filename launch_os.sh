#! /bin/bash

cd bin || { echo "/bin not found"; exit 1; }

[[ -f "disk.img" ]] || { echo "disk.img not found"; exit 1; }

qemu-system-i386 \
	-machine q35 \
	-drive format=raw,file=disk.img,index=0,if=floppy \
	-gdb tcp::26000 \
	-S \
	-curses

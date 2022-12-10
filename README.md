# excelsior
A first self-taught attempt at developing a basic x86 operating system

## Dependencies
Development dependencies include the following
```
crossbuild-essential-i386 nasm gcc gdb make qemu-system-i386
```

## Development & Debug
 - Ensure you have `set auto-load safe-path /` in `~/.gdbinit` if you want the local `.gdbinit` file to be invoked on `gdb` launch.
 - `make`
 - Launch `launch_os.sh` in a terminal window
 - Launch gdb in another terminal while at project root (for `.gdbinit` )

Quitting `qemu` (in `-curses` mode)
In another terminal:
```
pkill qemu
```
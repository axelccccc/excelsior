# My OS
A first self-taught attempt at developing a basic x86 operating system

---

## Dependencies
Development dependencies include the following
```
nasm gcc gdb make qemu-system-i386
```

---

## Development & Debug
 - Ensure you have `set auto-load safe-path /` in `~/.gdbinit`
 - Launch `launch_os.sh` in a terminal window
 - Launch gdb while in this directory (`.gdbinit`)

Quitting `qemu` (in `-curses` mode)
In another terminal:
```
pkill qemu
```

---
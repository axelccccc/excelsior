;**********
; bootloader.asm
; simple bootloader
;**********

; org 0x7c00          ; start at 0x7c00
; /!\ only used for flat binary files (nasm -f bin)
; /!\ not used for elf files (nasm -f elf)
;       as the linker script will set the entry point

bits 16             ; 16 bit code

start:
    jmp boot        ; jump to boot



;**********
; constants
;**********

; message to print on screen
; (0ah = newline, 0dh = carriage return, 0 = end of string)
msg db "Welcome to my OS", 0x0a, 0x0d, 0x0

; GLOBAL DESCRIPTOR TABLE
; the GDT is necessary for the 32-bit protected mode to be enabled

gdt_start:          ; label for calculating gdt size
                    ; and for having the start address of the GDT

gdt_null:           ; the null descriptor (always first)
    dd 0x0          ; dd -> define double word (32bits)
    dd 0x0

gdt_code:           ; code segment descriptor
    ; 1st flags  : (present)1 (privilege)00 (descriptor type)1           -> 1001 b
    ; type flags : (code)1 (conforming)0 (readable)1 (accessed)0         -> 1010 b
    ; 2nd flags  : (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 0b10011010   ; 1st flags, type flags
    db 0b11001111   ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_data:           ; data segment descriptor
    ; the data segment overlaps the code segment for the *basic flat model*
    ; only type flags differ from code segment
    ; type flags : (code)0 ( expanddown)0 (writable)1 (accessed)0        -> 0010 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 0b10010010   ; 1st flags, type flags
    db 0b11001111   ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_end:            ; label for calculating GDT size

; GDT DESCRIPTOR
; structure passed to the CPU to identify the GDT
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; GDT size
    dd gdt_start                ; Start address of GDT

;**********
; macros
;**********

; Segment descriptor offsets
NULL_SEG equ 0x0
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

;**********
; variables
;**********



;**********
; code
;**********

boot:
    cli                     ; disable interrupts
    cld                     ; clear direction flag (string operations)

    ; print welcome message
    push msg                ; string  (1st parameter) = msg
    call print              ; print welcome message & advance cursor
    
    ; read 2nd sector of floppy disk into memory
    ;; prepare buffer
    mov ax, 0x50            ; buffer segment 0x50
    mov es, ax              ; set ES to buffer segment
    xor bx, bx              ; buffer offset 0x00
                            ; (buffer is now at 0x50:00 = 0x500)
    ;; prepare registers
    mov ah, 0x02            ; function 2: read sectors
    ; mov al, 2               ; read 2 sectors
    mov al, 8               ; read 8 sectors
    mov ch, 0               ; track 0
    mov cl, 2               ; read sector 2
    mov dh, 0               ; head 0
    mov dl, 0               ; drive 0
    int 0x13                ; call BIOS — Disk Interrupt

    ; switch to 32-bit protected mode
    cli                     ; disable interrupts
    lgdt [gdt_descriptor]   ; load GDT
    ;; set first bit of control register cr0 for actual switch
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax            ; we are now in 32-bit protected mode
    ;; far jump forces the CPU to flush the pipeline
    ;; (pre-fetched instructions)
    jmp CODE_SEG:protected_mode
    ; jmp [500h + 18h]        ; jump to dereferenced kernel entry point address
    
; include I/O custom library
; (while still in 16-bit mode)
%include "io.asm"
    
protected_mode:

bits 32         ; 32-bit protected mode
    
    ; initialize registers and stack, since
    ; 16-bit mode values are now meaningless
    mov ax, DATA_SEG        ; put valid 32-bit data segment
    mov ds, ax              ; into all segment registers
    mov ss, ax
    mov es, ax
    mov fs, ax              ; new in 32-bit mode
    mov gs, ax              ; new in 32-bit mode

    mov ebp, 0x90000        ; update stack position to be at the top
    mov esp, ebp            ; of free space

start_kernel:               ; finally jump to kernel entry point

    jmp [500h + 18h]        ; jump to dereferenced kernel entry point address
    
    hlt                     ; halt the CPU

; pad the rest of the sector with 0s
; (510 - (current offset - start offset))
times 510 - ($-$$) db 0

; boot signature
dw 0xaa55



;**********
; end of bootloader.asm
;**********

;**********
; bootloader.asm
; simple bootloader
;**********
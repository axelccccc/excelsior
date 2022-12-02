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
msg db "Welcome to my OS", 0ah, 0dh, 0h



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
    mov ah, 02h             ; function 2: read sectors
    mov al, 2               ; read 2 sectors
    mov ch, 0               ; track 0
    mov cl, 2               ; read sector 2
    mov dh, 0               ; head 0
    mov dl, 0               ; drive 0
    int 13h                 ; call BIOS — Disk Interrupt
    ;; execute the sector
    jmp [500h + 18h]        ; jump to dereferenced kernel entry point address
    
    hlt                     ; halt the CPU

strlen:
    push bp                 ; Store the current stack frame
    mov bp, sp              ; Preserve ESP into EBP for argument references
    mov bx, [bp+4]          ; load segment address
.loop:
    inc bx                  ; increment segment address
    cmp byte [bx], 0        ; check if end of string
    je .end                 ; if so, jump to end
    jmp .loop               ; otherwise, loop
.end:
    sub bx, [bp+4]          ; calculate length
    mov ax, bx              ; store length in ax
    mov sp, bp              ; remove parameters from stack
    pop bp                  ; Restore the previous stack frame
    ret

print:
    ; prologue
    push bp                 ; Store the current stack frame
    mov bp, sp              ; Preserve ESP into EBP for argument references
    sub sp, 4               ; allocate space to backup si and di
    push di                 ; backup di
    push si                 ; backup si

    ; get cursor position in dx
    mov ah, 03h             ; function 3: query cursor position
    mov bh, 0               ; page 0
    int 10h                 ; call BIOS — Video Interrupt
                            ; DH = row, DL = column

    ; get string length in cx
    push word [bp+4]        ; push string address onto stack
    call strlen             ; call strlen
    mov cx, ax              ; store length in cx

    mov bp, [bp+4]          ; load string address into bp

    ; print string
    mov ax, 1301h           ; function 13: write string with attributes
                            ; and update cursor position
    mov bh, 0h              ; page 0
    mov bl, 07h             ; white text
    ; mov si, [bp+4]          ; load segment address
    int 10h                 ; call BIOS — Video Interrupt

    ; epilogue
    pop si                  ; restore si
    pop di                  ; restore di
    add sp, 4               ; remove parameters from stack
    pop bp                  ; Restore the previous stack frame
    ret

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
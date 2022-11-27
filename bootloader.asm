;**********
; bootloader.asm
; simple bootloader
;**********

org 0x7c00          ; start at 0x7c00
bits 16             ; 16 bit code

start:
    jmp boot        ; jump to boot



;**********
; constants
;**********

; message to print on screen
; (0ah = newline, 0dh = carriage return, 0 = end of string)
msg db "Welcome to HaCe OS", 0ah, 0dh, 0h



;**********
; variables
;**********



;**********
; code
;**********

boot:
    cli             ; disable interrupts
    cld             ; clear direction flag (string operations)

    ; print string "Welcome to HaCe OS"
    ; push offset address
    push msg        ; string  (1st parameter) = msg
    call print      ; print string
    
    hlt             ; halt the CPU

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
    ; inc cx                  ; increment length by 1 (for null terminator)

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
    mov sp, bp              ; remove parameters from stack
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
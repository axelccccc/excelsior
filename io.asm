;**********
; io.asm
; simple I/O routines
;**********

;**********
; code
;**********

; get the position of the cursor in register AX
;   AH: row
;   AL: column
getcursorpos:
    push bp                 ; Store the current stack frame base pointer
    mov bp, sp              ; insert new stack frame pointer into ebp for reference
    mov ah, 03h             ; function 3: query cursor position
    mov bh, 0               ; page 0
    int 10h                 ; call BIOS — Video Interrupt
                            ; DH = row, DL = column
    mov ax, dx              ; return value in ax
    mov sp, bp              ; remove parameters from stack
    pop bp                  ; Restore the previous stack frame
    ret                     ; return to caller

; move the cursor to the specified position on the screen
; parameters:
;   row
;   col
movcursor:
    push bp                 ; Store the current stack frame base pointer
    mov bp, sp              ; insert new stack frame pointer into ebp for reference
    mov dh, [bp+4]          ; load row
    mov dl, [bp+5]          ; load column
    mov ah, 2               ; function 2: set cursor position
    mov bh, 0               ; page 0
    int 10h                 ; call BIOS — Video Interrupt
    mov sp, bp              ; remove parameters from stack
    pop bp                  ; Restore the previous stack frame base pointer
    ret                     ; return to caller

; print a character at the current cursor position
; parameters:
;   character
;   text color
;   number of times to print
putchar:
    push bp                 ; Store the current stack frame
    mov bp, sp              ; Preserve ESP into EBP for argument references
    mov ah, 09h             ; function 9: write character with attributes
    mov al, [bp+4]          ; load character
    mov bh, 0               ; page 0
    mov bl, [bp+6]          ; load text color
    mov cx, [bp+8]          ; load number of characters to write
    int 10h                 ; call BIOS — Video Interrupt
    mov sp, bp              ; remove parameters from stack
    pop bp                  ; Restore the previous stack frame
    ret                     ; return to caller

; get the length of a string (without the null terminator)
; parameters:
;   string address
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

; print a string at the current cursor position
; and advance the cursor
; parameters:
;   string address
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

;**********
; data
;**********

;**********
; io.asm
; simple I/O routines
;**********
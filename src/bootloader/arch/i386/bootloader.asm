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
; msg db "Welcome to my OS", 0x0a, 0x0d, 0x0

; GLOBAL DESCRIPTOR TABLE
; the GDT is necessary for the 32-bit protected mode to be enabled

gdt_start:          ; label for calculating gdt size
                    ; and for having the start address of the GDT

gdt_null:           ; the null descriptor (always first)
    dd 0x0          ; dd -> define double word (32bits)
    dd 0x0

gdt_code:           ; kernel code segment descriptor
    ; 1st flags  : (present)1 (privilege)00 (descriptor type)1           -> 1001 b
    ; type flags : (code)1 (conforming)0 (readable)1 (accessed)0         -> 1010 b
    ; 2nd flags  : (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 0b10011010   ; 1st flags, type flags
    db 0b11001111   ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_data:           ; kernel data segment descriptor
    ; the data segment overlaps the code segment for the *basic flat model*
    ; only type flags differ from code segment
    ; type flags : (code)0 (expanddown)0 (writable)1 (accessed)0        -> 0010 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 0b10010010   ; 1st flags, type flags
    db 0b11001111   ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_user_code:      ; user mode code segment descriptor
    ; 1st flags  : (present)1 (privilege)11 (descriptor type)1           -> 1111 b
    ; type flags : (code)1 (conforming)0 (readable)1 (accessed)0         -> 1010 b
    ; 2nd flags  : (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 0b11111010   ; 1st flags, type flags
    db 0b11001111   ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

gdt_user_data:      ; user mode data segment descriptor
    ; the data segment overlaps the code segment for the *basic flat model*
    ; only type flags differ from code segment
    ; type flags : (code)0 (expanddown)0 (writable)1 (accessed)0        -> 0010 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 0b11110010   ; 1st flags, type flags
    db 0b11001111   ; 2nd flags, Limit (bits 16-19)
    db 0x0          ; Base (bits 24-31)

task_state_seg:     ; task state segment descriptor
    ; More info: https://wiki.osdev.org/Task_State_Segment
    ; 1st flags  : (present)1 (privilege)00 (descriptor type)0           -> 1000 b
    ; type flags : (code)1 (conforming)0 (readable)0 (accessed)1         -> 1001 b
    ; 2nd flags  : (granularity)0 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 0100 b
    dw 0x6b         ; Limit (bits 0-15) Protected mode: 0x67 + 4
    dw 0x0          ; Base (bits 0-15) Which addres for TSS ??
    db 0x0          ; Base (bits 16-23)
    db 0b10001001   ; 1st flags, type flags
    db 0b00000000   ; 2nd flags, Limit (bits 16-19)
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
    ; push msg                ; string  (1st parameter) = msg
    ; call print              ; print welcome message & advance cursor
    
    ; /!\ since kernel is loaded at 0x100000,
    ; and bootloader is from 0x7c00 to 0x7e00 (512 bytes),
    ; mem from 0x7e00 to 0x9fc00 is accessible (0x9fc00 to 0x100000 is other BIOS stuff)
    ; for other purposes
    
    ; retrieve extended memory size
    ; using BIOS interrupt 0x15, function 0xe820
    ; to load the array to 0x7e04
    ; (0x7e00 will store array size)
mem_map:
    mov ax, 0               ;
    mov es, ax              ; clear ES segment
    mov edi, 0x7e04         ; (ES:DI is now at 0x0:0x7e00 = 0x7e00)
    xor ebx, ebx            ; clear ebx
    xor ebp, ebp            ; bp will store array size
    mov edx, 0x534d4150     ; set signature in edx ("SMAP" in ASCII)
    mov eax, 0xe820         ; function 0xe820: get ext memory map
    mov [es:di+20], dword 1 ; make entry ACPI compatible by setting last uint32_t to 1
    mov ecx, 0x18           ; ask for 24-byte memory map entry (for alignment)
    int 0x15                ; call BIOS — System Management Interrupt
    jc short .mm_error      ; if carry flag is set, error
    cmp eax, 0x534d4150     ; check signature
    jne short .mm_error     ; if not equal, error
    cmp ebx, 0              ; check if end of list
    jne short .mm_check_entry ; if not equal, check entry then loop
    jmp short .mm_end       ; else, end
.mm_loop:
    mov eax, 0xe820         ; function 0xe820: get ext memory map
    mov [es:di+20], dword 1 ; make entry ACPI compatible by setting last uint32_t to 1
    mov ecx, 0x18           ; ask for 24-byte memory map entry
    int 0x15                ; call BIOS — System Management Interrupt
    jc short .mm_end        ; if carry flag is set, end of list already reached
    mov edx, 0x534d4150     ; reset signature as a precaution
.mm_check_entry:
    cmp ecx, 0              ; check if bytes read is 0
    je short .mm_skip       ; if so, skip
    mov eax, [es:di+8]      ; if entry length is 0, skip entry
    or eax, [es:di+8]       ; OR lower 32 bits with upper 32 bits
    jz short .mm_skip       ; if zero, skip entry
    cmp cl, 24              ; check if bytes read is 24
    jne short .mm_valid_entry ; if not, no more checks needed
    test byte [es:di+20], 1 ; else check ACPI ignore entry bit
    je short .mm_skip       ; if set, skip entry
.mm_valid_entry:
    add di, 24              ; else, increment buffer offset by 24
    inc ebp                 ; and increment array size by 1
.mm_skip:
    cmp ebx, 0              ; check if end of list
    jne short .mm_loop      ; if not, loop
.mm_end:
    mov [0x7e00], ebp       ; store array size at 0x7e00
    jmp short .mm_finished  ; and jump to end
.mm_error:
    stc
    jmp short .mm_finished
.mm_finished:

    ; we assume int15 e820's array will not take more than
    ; 33kb (0x10000 - 0x7e00 = 0x8200) [the estimate is extremely conservative]
    ; and place the first page directory entry at 0x10000
    ; followed by the first page table entry at 0x11000 (the 1024 following map 4mb)
    ; and the second table is the first kernel table, at 0x12000
    ; to map the kernel to the higher half (0xC0000000, 2GB mark)
    ; and enable paging.
    ; we map 1st mb to itself and 4mbs of kernel to 0xC0000000.
    ; if the kernel ends up bigger than 4mb, the method needs to be changed

k_paging:
    mov ax, 0x1000
    mov es, ax              ; segment to 0x1000
    mov edi, 0x0            ; clear di -> es:di at 0x1000:00 -> 0x10000

    ; identity map the first 1mb

    ; page directory 1: addr 0, U/S 0, R/W 1, P 1 -> 0x00000003
    mov eax, 0x11000        ; address of first page table entry
    or eax, 0x3             ; add permissions above
    mov [es:di], eax        ; write pde

    ; make other pdes as not present (0x2) to remove their space needs
    ; if not present, no further info in entry is necessary
    mov eax, 1              ; loop counter (1 bc we did one already)
    add edi, 4              ; increment es:di
.kp_pde:
    mov ecx, 2              ; U/S 0, R/W 1, P 0
    mov [es:di], ecx        ; write pde
    add edi, 4              ; increment es:di
    inc eax                 ; increment counter
    cmp eax, 1024           ; do all entries
    jne .kp_pde

    mov eax, 0              ; loop counter
    mov ebx, 0x00000000     ; physical address (here corresponds to virtual)
    mov edi, 0x1000         ; es:di at 0x1000:1000 -> 0x11000
.kp_identity:
    mov ecx, ebx
    or ecx, 3               ; page permissions: U/S 0, R/W 1, P 1 -> 0x3
    mov [es:di], ecx        ; write pte
    add ebx, 0x1000         ; next address 4096 bytes (0x1000) further
    add edi, 4              ; next entry
    inc eax                 ; increment counter
    cmp eax, 256            ; see if we reached 1mb (256 * 4096 bytes)
    jne .kp_identity

    ; map kernel from 0x100000 to 0xC0000000

    mov edi, 0xC00          ; edi to 768th pde -> table starting at 0xC0000000
                            ; es:di -> 0x1000:C00 -> 0x10C00
    mov eax, 0x12000        ; we put first kernel table at 0x12000 (right after
                            ; 1st mb identity mapping table)
    or eax, 3               ; set pde 768 to present
    mov [es:di], eax

    mov eax, 0              ; reset loop counter
    mov ebx, 0x100000       ; physical address
    mov edi, 0x2000         ; es:di at 0x1000:2000 -> 0x12000

.kp_kernel:
    mov ecx, ebx
    or ecx, 3               ; page permissions: U/S 0, R/W 1, P 1 -> 0x3
    mov [es:di], ecx        ; write pte
    add ebx, 0x1000         ; next address 4096 bytes (0x1000) further
    add edi, 4              ; next entry
    inc eax                 ; increment counter
    cmp eax, 1024           ; see if we reached 4mb (1024 * 4096 bytes)
    jne .kp_kernel
    
load_kernel:
    ; read 2nd sector of floppy disk (kernel) into memory
    ;; prepare buffer
    mov ax, 0xFFFF          ; buffer segment 0xFFFF
    mov es, ax              ; set ES to buffer segment
    ; xor bx, bx              ; buffer offset 0x00
    mov bx, 0x10            ; buffer offset 0x10
                            ; (buffer is now at 0xFFFF:10 = 0x100000)
                            ; the kernel is loaded at a 64k boundary, (0x10000)
                            ; the floppy disk driver otherwise raises
                            ; a boundary cross error when it is traversed
    ;; prepare registers
    mov ah, 0x02            ; function 2: read sectors
    mov al, 128             ; read 128 sectors (64kb)
    mov ch, 0               ; track 0
    mov cl, 2               ; read sector 2
    mov dh, 0               ; head 0
    mov dl, 0               ; drive 0
    int 0x13                ; call BIOS — Disk Interrupt
    
    ; ; read the following 64kb from the floppy disk into memory
    ; ;; prepare buffer
    ; mov ax, 0x2000          ; buffer segment 0x2000
    ; mov es, ax              ; set ES to buffer segment
    ; xor bx, bx              ; buffer offset 0x00
    ;                         ; (buffer is now at 0x2000:00 = 0x20000)
    ; ;; prepare registers
    ; mov ah, 0x02            ; function 2: read sectors
    ; mov al, 128             ; read 128 sectors (64kb)
    ; mov ch, 7               ; track 7
    ; mov cl, 4               ; read sector 4 
    ;                         ; (7 * 18 + 14 = 130th sector (2 + 128))
    ; mov dh, 0               ; head 0
    ; mov dl, 0               ; drive 0
    ; int 0x13                ; call BIOS — Disk Interrupt

switch_protected:
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
; %include "io.asm"
    
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

enable_paging:
    mov eax, 0x10000        ; load page directory address
    mov cr3, eax            ; into CR3

    mov eax, cr0            ; put CR0 in eax for modification
    or eax, 0x80000000      ; enable paging flag
    mov cr0, eax            ; put back in CR0, we enabled paging

jump_to_kmain:

    ; TEMPORARY, TEST
    ; mov ebp, 0xC0300000     ; put stack position at 3mb mark over kernel start ?
    ; mov esp, ebp            ; 

    jmp CODE_SEG:0xC000235C ; far jump to kernel entry point on code segment (CODE_SEG:_start)
    
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
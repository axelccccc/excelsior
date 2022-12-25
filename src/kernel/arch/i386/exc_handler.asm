extern exception_handler
extern irq_handler
extern kbd_handler

%macro isr_err_stub 1
isr_stub_%+%1:
    pushad
    cld
    call exception_handler
    popad
    iret
%endmacro

; Macro for standard exception handler format
; with `iret`

%macro isr_no_err_stub 1
isr_stub_%+%1:
    pushad
    cld
    call exception_handler
    popad
    iret
%endmacro

; Macro for standard IRQ handler format
; with `iret`

%macro irq_stub 1
isr_stub_%+%1:
    pushad
    cld
    call irq_handler
    popad
    iret
%endmacro

; ISRs with their own error codes
; and `iret`

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

; IRQs

irq_stub 32
isr_stub_33: ; Keyboard Input handler
    pushad
    cld
    call kbd_handler
    popad
    iret
irq_stub 34 ; Slave IRQ, not used
irq_stub 35
irq_stub 36
irq_stub 37
irq_stub 38
irq_stub 39
irq_stub 40
irq_stub 41
irq_stub 42
irq_stub 43
irq_stub 44
irq_stub 45
irq_stub 46
irq_stub 47

; Write a table of addresses to the ISRs
global isr_stub_table
isr_stub_table:
%assign i 0
%rep    48
    dd isr_stub_%+i
%assign i i+1
%endrep
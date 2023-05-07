; Define I/O pins
P1 equ 90h ; 7-segment display
P2 equ A0h ; keypad
P3 equ B0h ; buzzer

; Define variables
count equ 20h ; countdown value (in minutes)
state equ 21h ; machine state (0: set time, 1: counting, 2: alarm)

; Main program
start:
    mov state, #0 ; initial state is set time
    mov P1, #0 ; clear 7-segment display
    mov P3, #0 ; turn off buzzer

; Set time loop
set_time:
    ; Check for button press
    mov A, P2 ; read keypad
    anl A, #0Fh ; mask off upper 4 bits
    cjne A, #0Fh, update_time ; check if any button is pressed

    ; Button not pressed, go to next state
    mov state, #1 ; change state to counting
    sjmp count_down ; start countdown

update_time:
    ; Handle button press
    cjne A, #01h, check_minus ; check if "-" button is pressed
    djnz count, update_display ; decrement count if not zero
    jmp set_time ; go back to set time loop

check_minus:
    cjne A, #02h, check_ok ; check if "+" button is pressed
    inc count ; increment count
    jmp set_time ; go back to set time loop

check_ok:
    cjne A, #04h, check_cancel ; check if "OK" button is pressed
    mov state, #1 ; change state to counting
    sjmp count_down ; start countdown

check_cancel:
    cjne A, #08h, set_time ; check if "Cancel" button is pressed
    mov count, #0 ; reset countdown value
    mov state, #0 ; change state to set time
    jmp set_time ; go back to set time loop

; Countdown loop
count_down:
    ; Display countdown value
    mov A, count
    mov P1, A
    acall delay_ms

    ; Check if countdown is finished
    jz alarm

    ; Decrement count and loop back
    djnz count, count_down
    sjmp start

; Alarm loop
alarm:
    mov state, #2 ; change state to alarm
    mov P3, #1 ; turn on buzzer
    acall delay_ms
    mov P3, #0 ; turn off buzzer
    acall
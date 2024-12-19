.data
leds: .word     26, 21, 20, 16, 12, 25, 24, 23

.text
.arm
.global digitalWriteAll
.extern digitalWrite

digitalWriteAll:
push {lr}
mov r6, r0
mov r2, #0

loop:
lsr r1, r6, r2
and r1, r1, #0x01
ldr r4, =leds
ldr r0, [r4, r2, lsl #2]

bl digitalWrite

add r2, r2, #1
cmp r2, #8
bne loop

pop {lr}
BX LR


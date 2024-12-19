.text
.arm
.global digitalWriteAll
.extern digitalWrite

leds:           .word     26, 21, 20, 16, 12, 25, 24, 23
               
                //Función para escribir en todos los LEDs basados en un valor binario
digitalWriteAll:
                mov     r1, r0             
                mov     r2, #0             
               
loop:
                lsr     r3, r1, r2
                and     r3, r3, #0x01
                ldr     r4, =leds          
                ldr     r5, [r4, r2, lsl #2] 
                stmfd SP! , {R0, R1, R3, R5}
                mov r0, r5
                mov r1, r3
                bl      digitalWrite     
                ldmfd SP! , {R0, R1, R3, R5}
                add     r2, r2, #1
                cmp     r2, #8
                bne     loop
               
                BX      LR

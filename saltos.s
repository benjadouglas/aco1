.text

.global saltos
saltos:

PUSH {R4, R5, R6, LR}

reset:
LDR R4, =saltosd  // R4 = (uint32_t*)&saltosd
MOV r6, #4        @ Número de patrones en la secuencia

loop:
LDRB R5, [R4], #1 // R5 = R4; R4++;

MOV R0, R5        // En el assembly, el `MOV R0, R5` es la forma de pasar el parámetro a la función
BL disp_binary    // disp_binary(R5)

MOV R0, R5
BL ledShow        // ledShow(R5)

MOV R0, #2
BL delay

SUBS R6, R6, #1
BEQ reset

CMP R0, #0
BNE loop
BL turnOff

POP {R4, R5, R6, PC}

.data
saltosd:
.byte 0x3    @ Luces 1 y 2
.byte 0xC0   @ Luces 7 y 8
.byte 0xC    @ Luces 3 y 4
.byte 0x30   @ Luces 5 y 6

.text
.global carga_bateria
carga_bateria:

PUSH {R4, R5, R6, LR}

reset:
MOV R4, #1      // Inicializar con un solo LED encendido (0x01)

main_loop:
MOV R0, R4
BL disp_binary  // Mostrar en pantalla el valor de R4 (0000 0001)

MOV R0, R4
BL ledShow      // Mostrar en LEDs el valor de R4 (0000 0001)

SUB R6 , R6, #1 // R6 = R6 -1

MOV R0, #3
BL delay
CMP R0, #0
BEQ turn_off    // Si delay retorna 0, apagar y salir

LSL R5, R4, #1  // if R4 (0000 0001) then R5 (0000 0010)
ORR R4, R4, R5  // Combinar el patrón actual con el nuevo LED
CMP R4, #0x100   // Verificar si todos los LEDs están encendidos (8 LEDs = 0xFF)
BGE reset       // Si llegamos al máximo, reiniciar

B main_loop     // Continuar el ciclo principal

turn_off:
BL turnOff      // Apagar LEDs

POP {R4, R5, R6, PC}

.data
msg_carga:  .asciz "Carga de Bateria:\n"

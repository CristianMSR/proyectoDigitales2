#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "ads1115.h"


int getKeyPress() {
    struct termios oldt, newt;
    int ch;

    // Configurar terminal para lectura sin bloqueo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    // Restaurar la configuración original del terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int checkKeys(int *tiempo) {
    //printf("Tiempo inicial: %d\n", tiempo);
    //printf("Usa las flechas hacia arriba y hacia abajo para ajustar el tiempo.\n");
    //printf("Presiona cualquier letra para salir.\n");
    int key = getKeyPress();
    if (key == '\033') { // Secuencia de escape para teclas especiales
        getKeyPress();  // Ignorar el siguiente carácter '['
        key = getKeyPress();
        if (key == 'A') { // Flecha hacia arriba
            *tiempo += 10;
            printf("Tiempo incrementado: %d\n", *tiempo);
            } else if (key == 'B') { // Flecha hacia abajo
                if (*tiempo > 10) {
                    *tiempo -= 10;
                    printf("Tiempo decrementado: %d\n", *tiempo);
                } else {
                    printf("El tiempo ya está en el mínimo permitido: %d\n", *tiempo);
                }
            }
        } else if (isalpha(key)) { // Verificar si es una letra
            printf("Letra detectada ('%c'). Saliendo del programa.\n", key);
            return -1;
        }
    return 0;
}


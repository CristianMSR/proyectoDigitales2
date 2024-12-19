#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

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

int main() {
    int tiempo = 10; // Inicializar la variable en 10
    printf("Tiempo inicial: %d\n", tiempo);
    printf("Usa las flechas hacia arriba y hacia abajo para ajustar el tiempo.\n");
    printf("Presiona cualquier letra para salir.\n");

    while (1) {
        int key = getKeyPress();

        if (key == '\033') { // Secuencia de escape para teclas especiales
            getKeyPress();  // Ignorar el siguiente carácter '['
            key = getKeyPress();

            if (key == 'A') { // Flecha hacia arriba
                tiempo += 10;
                printf("Tiempo incrementado: %d\n", tiempo);
            } else if (key == 'B') { // Flecha hacia abajo
                if (tiempo > 10) {
                    tiempo -= 10;
                    printf("Tiempo decrementado: %d\n", tiempo);
                } else {
                    printf("El tiempo ya está en el mínimo permitido: %d\n", tiempo);
                }
            }
        } else if (isalpha(key)) { // Verificar si es una letra
            printf("Letra detectada ('%c'). Saliendo del programa.\n", key);
            break;
        }
    }

    return 0;
}


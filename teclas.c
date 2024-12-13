#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int get_keypress(void) {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt); // Obtener configuración de la terminal
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Desactivar el modo canonico y la visualización de la tecla
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Aplicar cambios
    ch = getchar(); // Leer tecla
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restaurar configuración original
    return ch;
}

int main() {
    int velocidad = 10;
    int tecla;

    printf("Programa iniciado. Presiona una tecla para interactuar.\n");
    printf("Utiliza las flechas arriba y abajo para modificar la velocidad.\n");
    printf("Presiona cualquier letra para finalizar el programa.\n");

    while (1) {
        tecla = get_keypress(); // Obtener tecla presionada

        // Si es la flecha hacia arriba
        if (tecla == 65) {
            if (velocidad + 10 <= 5000) {
                velocidad += 10;
                printf("Velocidad incrementada a: %d\n", velocidad);
            } else {
                printf("La velocidad no puede ser mayor a 5000\n");
            }
        }
        // Si es la flecha hacia abajo
        else if (tecla == 66) {
            if (velocidad - 10 >= 10) {
                velocidad -= 10;
                printf("Velocidad decrementada a: %d\n", velocidad);
            } else {
                printf("La velocidad no puede ser menor a 10\n");
            }
        }
        // Si se lee una letra (finalizar el programa)
        else if ((tecla >= 'a' && tecla <= 'z') || (tecla >= 'A' && tecla <= 'Z')) {
            printf("Letra detectada. Finalizando programa.\n");
            break;
        }
    }

    return 0;
}

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt); // Obtener configuración actual del terminal
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Desactivar modo canonico y eco
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0); // Obtener flags de archivo
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Activar modo no bloqueante

    ch = getchar(); // Leer caracter

    // Restaurar configuraciones originales del terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin); // Volver a poner el caracter en stdin
        return 1;
    }

    return 0;
}

void checkArrows(int *velocidad, int *salir) {
    char ch;

    if (kbhit()) {
        ch = getchar(); // Leer primer caracter

        if (ch == 27) { // Código de escape para flechas
            if (kbhit() && getchar() == 91) { // Verificar siguiente parte de la secuencia
                if (kbhit()) {
                    ch = getchar();
                    if (ch == 'A') { // Flecha hacia arriba
                        *velocidad += 10;
                        printf("Velocidad incrementada: %d\n", *velocidad);
                    } else if (ch == 'B') { // Flecha hacia abajo
                        *velocidad -= 10;
                        if (*velocidad < 10) *velocidad = 10; // Límite mínimo
                        printf("Velocidad decrementada: %d\n", *velocidad);
                    }
                }
            }
        } else if (isalpha(ch)) { // Si es una letra
            *salir = 1;
            printf("Se presionó una letra. Saliendo...\n");
        } else {
            printf("Tecla no reconocida.\n");
        }
    }
}

int main() {
    int velocidad = 10;
    int salir = 0;

    printf("Presione las flechas hacia arriba o abajo para cambiar la velocidad o una letra para salir (Ctrl+C para salir).\n");

    while (!salir) {
        checkArrows(&velocidad, &salir);
        usleep(100000); // Reducir carga de la CPU (100 ms)
    }

    return 0;
}

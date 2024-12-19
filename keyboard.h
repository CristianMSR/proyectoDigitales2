#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ctype.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "ads1115.h"

int password() {
	const int FD_STDIN = 0;
	const char *PASSWORD = "12345";
	const int MAX_TRIES = 3;
	const int MAX_LEN = 5;
	
    char lect[MAX_LEN + 1];  // Cadena para almacenar la contraseña ingresada (5 dígitos + terminador nulo)
    struct termios t_old, t_new;
    int tries = 0;

    // Configurar el terminal para desactivar eco y entrada canónica
    tcgetattr(FD_STDIN, &t_old);  // Lee atributos del teclado
    t_new = t_old;
    t_new.c_lflag &= ~(ECHO | ICANON);  // Desactiva eco y entrada canónica (entrada sin esperar "Enter")
    tcsetattr(FD_STDIN, TCSANOW, &t_new);

    while (tries < MAX_TRIES) {
        printf("Ingrese su password de 5 dígitos: ");
        fflush(stdout);  // Forzar la salida inmediata del mensaje
        int len = 0;  // Longitud de la entrada actual

        // Limpiar la contraseña ingresada previamente
        memset(lect, 0, sizeof(lect));

        while (1) {
            char c;
            read(FD_STDIN, &c, 1);  // Lee un carácter

            if (c == 127) {  // Código ASCII para "Backspace"
                if (len > 0) {
                    len--;  // Elimina el último carácter ingresado
                    printf("\b \b");  // Mueve el cursor hacia atrás, imprime espacio, mueve cursor de nuevo atrás
                    fflush(stdout);  // Forzar actualización de la pantalla
                }
            } else if (c == '\n') {  // Si se presiona "Enter"
                break;
            } else if (len < MAX_LEN) {  // Solo si la longitud actual es menor que 5
                lect[len++] = c;
                printf("*");  // Muestra un asterisco por cada carácter ingresado
                fflush(stdout);  // Forzar la salida inmediata del asterisco
            }
        }

        // Restablecer la configuración del terminal
        tcsetattr(FD_STDIN, TCSANOW, &t_old);
        printf("\n");

        // Comparar la contraseña ingresada con la contraseña almacenada
        if (strncmp(lect, PASSWORD, MAX_LEN) == 0) {
            printf("Bienvenido al Sistema\n");
            return 0;
        } else {
            printf("Password no válida\n");
            tries++;
        }

        // Vuelve a activar el modo sin eco para el siguiente intento
        tcsetattr(FD_STDIN, TCSANOW, &t_new);
    }

    printf("Número máximo de intentos alcanzado. Abortando...\n");
    tcsetattr(FD_STDIN, TCSANOW, &t_old);
    return -1;
}


#endif


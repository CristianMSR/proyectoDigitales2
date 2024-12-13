#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "termset.h"

#define BAUDRATE 9600

int main() {
    const char *raspi = "/dev/ttyUSB0"; //cambiar esto cuando lo implementemos en la raspi
    int fd;
    char initialSpeed;  //char para seleccionar entre 0 y 255
    char menuSelect;    // selecciona la velocidad y la secuencia

    fd = open(raspi, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Error estableciendo la comunicacion ");
        return EXIT_FAILURE;
    }

    struct termios ttyold, ttynew;
    if (termset(fd, BAUDRATE, &ttyold, &ttynew) < 0) {
        printf("Error configurando el puerto serie\n");
        close(fd);
        return EXIT_FAILURE;
    }

    char buffer[100];       //para recepcion
    char mensaje[100] = ""; //para transmision

    while(1){ //bucle de comunicación
       /* while(1){
            // Recibir menu de la raspi
            int bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);
            if (bytes_leidos > 0) {
                buffer[bytes_leidos] = '\0'; // Fin de cadena
                printf("%s\n", buffer);
                break;
            }
        }*/

        printf("Secuencia:");
        scanf("%c", &menuSelect);
        printf("%c\n",menuSelect);

        printf("Velocidad inicial:");
        scanf("%c", &initialSpeed);

        // Enviar datos
	    strcat(mensaje, initialSpeed);
	    strcat(mensaje, menuSelect);
        write(fd, mensaje, sizeof(mensaje));
    }

    // Restaurar configuración original del terminal
    tcsetattr(fd, TCSANOW, &ttyold);

    close(fd);
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "termset.h"

#define BAUDRATE 9600

int main() {
    const char *raspi = "/dev/ttyUSB0"; // cambiar esto cuando lo implementemos en la raspi
    int fd;
    char initialSpeed;  // char para seleccionar entre 0 y 255
    char menuSelect;    // selecciona la velocidad y la secuencia

    fd = open(raspi, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Error estableciendo la comunicacion ");
        return EXIT_FAILURE;
    }

    if (termset(fd, BAUDRATE, &ttyold, &ttynew) < 0) {
        printf("Error configurando el puerto serie\n");
        close(fd);
        return EXIT_FAILURE;
    }

    tcflush(fd, TCIOFLUSH);

    char buffer[3] = "";      // para recepcion
    char mensaje[2] = "";     // para transmision
    
    const char* title[] = {
        "Auto fantástico",
        "Choque",
        "Apilada",
        "Carrera",
        "Ruleta Rusa",
        "Saltito",
        "Alternado",
        "Contador Binario",
        "Setear velocidad"
    };

    while (1) { // bucle de comunicacion
          int bytes_leidos = 0;
             // Recibir menu de la raspi
             printf("Esperando selección de modo...\n");
             bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);
             if (bytes_leidos > 0) {
                 buffer[bytes_leidos] = '\0'; // Fin de cadena
             }
             
        printf("Seleccione la opción deseada:\n");
        for (int i = 0; i < 9; i++) {
            printf("    %d. %s\n", i + 1, title[i]);
        }

        printf("Secuencia: ");
        scanf(" %c", &menuSelect);

        printf("Velocidad inicial: ");
        scanf(" %c", &initialSpeed);

        // Enviar datos
        strcat(mensaje, &initialSpeed);
        strcat(mensaje, &menuSelect);
        write(fd, mensaje, sizeof(mensaje));
    }

    // Restaurar configuración original del terminal
    tcsetattr(fd, TCSANOW, &ttyold);

    close(fd);
    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "termset.h"

#define BAUDRATE B9600

extern int sequenceSelect ();

int main() {
    const char *raspi = "/dev/ttyUSB0"; // cambiar esto cuando lo implementemos en la raspi
    int fd;

    fd = open(raspi, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Error estableciendo la comunicacion ");
        return EXIT_FAILURE;
    }

    if (termset(fd, BAUDRATE, &ttyold, &ttynew) < 0) {
        tcsetattr(fd, TCSANOW, &ttyold);
        printf("Error configurando el puerto serie\n");
        close(fd);
        return EXIT_FAILURE;
    }

    tcflush(fd, TCIOFLUSH);

    int buffer = 0;      // para recepcion
    short int op = 0;

    while (1) { // bucle de comunicacion
           // Recibir menu de la raspi
           printf("Esperando selección de modo...\n");
           read(fd, &buffer, sizeof(buffer));

        op = sequenceSelect();

        // Enviar datos
        write(fd, &op, sizeof(op));
    }

    close(fd);
    return EXIT_SUCCESS;
}

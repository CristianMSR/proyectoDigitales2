#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "termset.h"

#define BAUDRATE B9600

int main() {
    const char *raspi = "/dev/ttyUSB0";
    int fd = open(raspi, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Error estableciendo la comunicacion");
        return EXIT_FAILURE;
    }

    if (termset(fd, BAUDRATE, &ttyold, &ttynew) < 0) {
        tcsetattr(fd, TCSANOW, &ttyold);
        printf("Error configurando el puerto serie\n");
        close(fd);
        return EXIT_FAILURE;
    }

    tcflush(fd, TCIOFLUSH);

    short int buffer = 0;
    short int op = 0;

    while (1) {
        printf("Esperando selección de modo...\n");
        read(fd, &buffer, sizeof(buffer)); // Recibe el valor

        op = htons(sequenceSelect()); // Convierte a big-endian
        write(fd, &op, sizeof(op));
    }

    close(fd);
    return EXIT_SUCCESS;
}

}

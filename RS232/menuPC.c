#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "termset.h"

#define BAUDRATE 9600

int main() {
    const char *raspi = "/dev/ttyUSB0"; //cambiar esto cuando lo implementemos en la raspi
    int fd;

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


    // Enviar datos
    const char *mensaje = "Hola Raspberry Pi!\n";
    write(fd, mensaje, sizeof(mensaje));
    

    // Recibir datos
    char buffer[100];
    int bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_leidos > 0) {
        buffer[bytes_leidos] = '\0'; // Asegura un fin de cadena
        printf("Recibido: %s\n", buffer);
    } //  else {
    //     printf("No se recibieron datos\n");
    // }

    // Restaurar configuración original del terminal
    tcsetattr(fd, TCSANOW, &ttyold);

    

    close(fd);
    return EXIT_SUCCESS;
}

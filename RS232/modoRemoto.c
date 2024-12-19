#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "termset.h"

#define BAUDRATE B9600
#define SERIAL_PORT "/dev/ttyS0"

struct termios ttyold, ttynew;

// Configuración del puerto serie
int termset(int fd, int baudrate, struct termios *old_tty, struct termios *new_tty) {
    if (tcgetattr(fd, old_tty) != 0) {
        perror("Error obteniendo configuracion del puerto serie");
        return -1;
    }

    memset(new_tty, 0, sizeof(*new_tty));
    new_tty->c_cflag = baudrate | CS8 | CLOCAL | CREAD;
    new_tty->c_iflag = IGNPAR;
    new_tty->c_oflag = 0;
    new_tty->c_lflag = 0;
    new_tty->c_cc[VTIME] = 0;
    new_tty->c_cc[VMIN] = 1;

    if (tcsetattr(fd, TCSANOW, new_tty) != 0) {
        perror("Error configurando el puerto serie");
        return -1;
    }

    return 0;
}

void modoRemoto() {
    int fd;
    char buffer = 0; // Para recibir menuSelect
    char start = 1;
    
    // Abrir puerto serie
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("Error abriendo el puerto serie");
        exit(EXIT_FAILURE);
    }

    // Configurar el puerto serie
    if (termset(fd, BAUDRATE, &ttyold, &ttynew) < 0) {
        tcsetattr(fd, TCSANOW, &ttyold);
        close(fd);
        exit(EXIT_FAILURE);
    }

    tcflush(fd, TCIOFLUSH);
    write(fd, &start, sizeof(start)); //Inicio comunicación con PC
    start = 0;

    do{
      read(fd, &buffer, sizeof(buffer)); //Recibe opción de PC
      printf("Opción seleccionada: %c\n", buffer);
    }while(buffer != 48);

    if(tcsetattr(fd, TCSANOW, &ttyold) != 0)
      printf("Pedantinc error\n");

    printf("Cerrando puerto\n");
    close(fd);
}

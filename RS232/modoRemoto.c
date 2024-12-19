#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int termset(int, int , struct termios *, struct termios *);

void modoRemoto() {
    int fd;
    short int buffer = 0; // Para recibir menuSelect
    
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

    write(fd, &start, sizeof(start)); //Inicio comunicación con PC
    
    do{
      read(fd, &buffer, sizeof(buffer)); //Recibe opción de PC
      printf("Opción seleccionada: %d\n", buffer);
    }while(buffer != 10);
    
    close(fd);
}

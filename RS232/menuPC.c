#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "termset.h"

#define BAUDRATE 9600

extern int sequenceSelect ();

int main() {
    const char *raspi = "/dev/ttyUSB0";
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

    short int buffer = 0;   // para recepcion
    short int op = 0;
    
    while(1){
      printf("Esperando selección de modo...\n");
      
      read(fd, &buffer, sizeof(buffer));
      if (buffer == 1){
        printf("Ingreso a modo remoto\n");
        do{
          op = sequenceSelect();
          write(fd, &op, sizeof(op));
        }while(op != 10);
      }
    }
    
    close(fd);
    return EXIT_SUCCESS;
}

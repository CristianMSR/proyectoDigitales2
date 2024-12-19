#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else 
#define CLEAR_COMMAND "clear"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "termset.h"
#include "PC.h"

#define BAUDRATE 9600

extern struct termios ttyold, ttynew;

extern char sequenceSelect ();

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

    char buffer = 0;   // para recepcion
    char op = 0;
    char key = 0;

    while(1){
      tcflush(fd, TCIOFLUSH);
      printf("Esperando selección de modo...\n");

      read(fd, &buffer, sizeof(buffer));

      if (buffer == 1){
        system(CLEAR_COMMAND);
        printf("Ingreso a modo remoto\n");
        do{
          op = sequenceSelect();
          write(fd, &op, sizeof(op));
          system(CLEAR_COMMAND);
          switch(op){
		case '0': 
		break;	
		case '9': read(fd,&buffer,sizeof(buffer));
		break;
		default: mensaje(op); 
			  do{
                              key = checkKeysRemoto();
                              write(fd, &key, sizeof(key));
                          }while(!isalpha(key));
		break;
	  }
        }while(op != 48);
      }
    }

    if (tcsetattr(fd, TCSANOW, &ttyold) != 0)
      printf("Pedantic error\n");

    printf("Cerrando el puerto\n");
    close(fd);
    return EXIT_SUCCESS;
}

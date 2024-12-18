// función para llamar en la raspi cuando se ejecute el modo remoto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BAUDRATE B9600
#define SERIAL_PORT "/dev/ttyS0" // Creo que era serial0

// Estructura para almacenar la configuración del terminal
static struct termios ttyold, ttynew;

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

  while(1){
    tcflush(fd, TCIOFLUSH);
    if(c==1){
      write(fd, &c, sizeof(c));
    }

    printf("Mando esto: %d\n",c);

   do{
    buffer=0;
    // Esperar y recibir menuSelect
    read(fd, &buffer, sizeof(buffer));
    if(buffer!=0)
      printf("Secuencia seleccionada: %d\n", buffer);
   
   }while(buffer!=10);
  }
    // Cerrar puerto serie
    close(fd);
}

#ifndef MENU_H
#define MENU_H

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
#include "termset.h"

#define BAUDRATE B9600
#define SERIAL_PORT "/dev/ttyS0"

struct termios ttyold, ttynew;

extern int verificarOP(int);

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

int menuModo() {
	
  int op, valid, ext;

    printf("Seleccione el modo de trabajo:\n");
    printf("    1. Modo remoto\n");
    printf("    2. Modo local\n");
    printf("    3. Finalizar programa\n");

  do{

    printf("\nOpción (1, 2 o 3): ");
    valid = scanf("%d",&op);

    while(getchar() != '\n');
    
    ext = verificarOP(op);

    if(valid != 1 || ext)
      printf("\n¡ERROR! Opción no válida.\nIngrese 1, 2 o 3.\n");

  }while(valid != 1 || op < 1 || op > 3);

  return op;
}

char sequenceSelect(){

  char op;
  int valid;

const char* title[] = {
        "Volver al inicio",
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

printf("Seleccione la opción deseada:\n");
    for (int i = 0; i < 10; i++) {
        printf("    %d. %s\n", i, title[i]);
    }

  do{
    printf("\nOpción (0-9): ");
    valid = scanf(" %c",&op);

    while (getchar() != '\n');

    if(valid != 1 || op < 48 || op > 57){
      printf("\n¡ERROR! Opción no válida.\nIngrese un número del 0 al 9.\n");
    }
  } while (valid != 1 || op < 48 || op > 57);

return op;
}

void mensaje(char op){
	printf("Ejecutando la secuencia ");
	switch(op){
		case '1': printf("Auto Fantástico");
        break;
        case '2': printf("El Choque");
        break;
        case '3': printf("La Apilada");
        break;
        case '4': printf("La Carrera");
        break;
        case '5': printf("La Ruleta Rusa");
        break;
        case '6': printf("Saltitos");
        break;
        case '7': printf("Alternado");
        break;
        case '8': printf("Contador Binario");
        break;
	}
	printf("...\n");
	printf("Utilice las flechas 'subir' y 'bajar' para modificar la velocidad\n");
	printf("Presione cualquier letra para salir\n");
}

void modoLocal(){
	char op;
	do{
		system(CLEAR_COMMAND);
		op = sequenceSelect();
		system(CLEAR_COMMAND);
		switch(op){
			case '0': return;
			break;	
			case '9': setTime();   
			break;
			default: mensaje(op); 
					 makeThreads(op, 2, 0);
			break;
		}
	}while(op);
}

void modoRemoto() {
    int fd;
    char buffer = 0; // Para recibir la opción
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

    do{
      system(CLEAR_COMMAND);
      printf("Esperando comando de la PC...\n");
      read(fd, &buffer, sizeof(buffer)); // Recibe opción de PC
      system(CLEAR_COMMAND);
      switch(buffer){
	case '0': if(tcsetattr(fd, TCSANOW, &ttyold) != 0)
                    printf("Pedantinc error\n");

                  printf("Cerrando puerto\n");
                  close(fd);
	          return;
	break;	
	case '9': setTime();
            write(fd,&start,sizeof(start));
	break;
	default: printf("Ejecutando secuencia...\n");
           makeThreads(buffer, 1, fd);
	break;
      }
    }while(buffer);
}

#endif

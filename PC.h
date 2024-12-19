#ifndef PC_H
#define PC_H

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
#include <termios.h>

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

int getKeyPress() {
    struct termios oldt, newt;
    int ch;

    // Configurar terminal para lectura sin bloqueo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    // Restaurar la configuración original del terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

char checkKeysRemoto() {
        key = getKeyPress();  
        if (key == '\033') { // Secuencia de escape
            getKeyPress(); // Ignorar '['
            key = getKeyPress();
            if (key == 'A'){
              key = '1';
            }else if(key == 'B'){ 
              key = '2';
            }
        } 
        if (isalpha(key)) { // Si es una letra
            printf("Letra detectada ('%c'). Finalizando secuencia.\n", key);
        }
        
        write(fd, &key, sizeof(key));
        return key;
}

#endif

#ifndef MENU_H
#define MENU_H

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else 
#define CLEAR_COMMAND "clear"
#endif

#include <stdio.h>

int menuModo() {
	
  int op, valid;

    printf("Seleccione el modo de trabajo:\n");
    printf("    1. Modo remoto\n");
    printf("    2. Modo local\n");
    printf("    3. Finalizar programa\n");

  do{

    printf("\nOpción (1, 2 o 3): ");
    valid = scanf("%d",&op);

    while(getchar() != '\n');

    if(valid != 1 || op < 1 || op > 3)
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
      printf("Sopa: %d",op);
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
					 makeThreads(op, 2);
			break;
		}
	}while(op);
}	

#endif

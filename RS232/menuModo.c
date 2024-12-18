#include <stdio.h>
#include "modoRemoto.h"

//extern void modoLocal();
extern void modoRemoto();

int main (int argc, char *argv[]) {

  int op, valid;

    printf("Seleccione el modo de trabajo:\n");
    printf("    1. Modo remoto\n");
    printf("    2. Modo local\n");

  do{

    printf("\nOpción (1 o 2): ");
    valid = scanf("%d",&op);

    while(getchar() != '\n');

    if(valid != 1 || op < 1 || op > 2)
      printf("\n¡ERROR! Opción no válida.\nIngrese 1 o 2.\n");

  }while(valid != 1 || op < 1 || op >2);

  if(op == 2){
    //modoLocal();
  }
else
    modoRemoto();

  return 0;
}

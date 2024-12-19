#include <stdio.h>

//extern void fantasticar(int*);
//extern void choque(int*);
//extern void apilada(int*);
//extern void theRace(int*);
//extern void rusky(int*);
//extern void jumpy(int*);
//extern void alternate(int*);
//extern void binary(int*);
//extern void setTime(int*);

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

//void sequenceExec(int op, int* time){
//  switch (op){
//    case 0: fantasticar(time);
//          break;
//    case 1: choque(time);
//          break;
//    case 2: apilada(time);
//          break;
//    case 3: theRace(time);
//          break;
//    case 4: rusky(time);
//          break;
//    case 5: jumpy(time);
//          break;
//    case 6: alternate(time);
//          break;
//    case 7: binary(time);
//          break;
//    case 8: setTime(time);
//          break;
//    case 9: break;
//  }
//}

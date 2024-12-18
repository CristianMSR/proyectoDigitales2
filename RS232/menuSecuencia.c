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

short int sequenceSelect(){

  short int op;
  int valid;

const char* title[] = {
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
    for (int i = 0; i < 9; i++) {
        printf("    %d. %s\n", i + 1, title[i]);
    }

  do{
    printf("\nOpción (1-9): ");
    valid = scanf("%hd",&op);

    while (getchar() != '\n');

    if(valid != 1 || op < 1 || op > 9)
      printf("\n¡ERROR! Opción no válida.\nIngrese un número del 1 al 8.\n");

  } while (valid != 1 || op < 1 || op > 9);

return op;
}

//void sequenceExec(int op, int* time){
//  switch (op){
//    case 1: fantasticar(time);
//          break;
//    case 2: choque(time);
//          break;
//    case 3: apilada(time);
//          break;
//    case 4: theRace(time);
//          break;
//    case 5: rusky(time);
//          break;
//    case 6: jumpy(time);
//          break;
//    case 7: alternate(time);
//          break;
//    case 8: binary(time);
//          break;
//    case 9: setTime(time);
//          break;
//  }
//}

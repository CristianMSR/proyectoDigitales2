#include <stdio.h>

int main (int argc, char *argv[]) {

  int op, valid;
  const char* title[] = {
        "Auto fantástico",
        "Choque",
        "Apilada",
        "Carrera",
        "Ruleta Rusa",
        "Saltito",
        "Alternado",
        "Contador Binario"
    };

    printf("Seleccione la secuencia a ejecutar:\n");
    for (int i = 0; i < 8; i++) {
        printf("    %d. %s\n", i + 1, title[i]);
    }

  do{
    printf("\nOpción (1-8): ");
    valid = scanf("%d",&op);

    while (getchar() != '\n');

    if(valid != 1 || op < 1 || op > 8)
      printf("\n¡ERROR! Opción no válida.\nIngrese un número del 1 al 8.\n");

  } while (valid != 1 || op < 1 || op > 8);

    printf("\nSecuencia seleccionada: %s\n", title[op-1]);

  //switch(op):
  //case 1:
  //case 2:
  //case 3:
  //case 4:
  //case 5:
  //case 6:
  //case 7:
  //default:

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "termset.h"

extern int menuModo();
//extern void modoLocal();
extern void modoRemoto();

int main (int argc, char *argv[]) {

 int mode;

  while(1){
    mode = menuModo();

    if(mode == 1)
      modoRemoto();
    //else
      //modoLocal();
  }

  return 0;
}


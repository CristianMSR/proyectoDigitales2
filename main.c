#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "libraries/easypio.h"
#include "libraries/sequence.h"
#include "libraries/keyboard.h"
#include "libraries/ads1115.h"
#include "libraries/hilos.h"
#include "libraries/menu.h"

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else 
#define CLEAR_COMMAND "clear"
#endif

extern int verificarOP(int);

int tiempo = 10;
// Función principal
int main() {
    pioInit();
    sequenceInit();
    system(CLEAR_COMMAND);
    //Solicitud de contraseña
    if(password() == -1){return -1;}  
    system(CLEAR_COMMAND);
    //Lectura inicial del potenciómetro 
	setInicialTime(&tiempo);
    int modo;
    do{
	//Menú local/remoto
	modo = menuModo();
	if(modo == 1){
	    modoRemoto();
	} 
	if(modo == 2){
	    modoLocal();
	}	    
    }while(modo != 3);
       
    printf("Programa finalizado.\n");
    return 0;
}

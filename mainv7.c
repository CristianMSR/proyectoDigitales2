#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h> // Para usleep
#include "easypio.h"
#include "sequence.h"
#include "keyboard.h"
#include "ads1115.h"
#include "hilos.h"


int tiempo = 10;
// Función principal
int main() {
    pioInit();
    sequenceInit();
    
    //Solicitud de contraseña
	if(password() == -1){return -1;}  
    //Lectura inicial del potenciómetro 
	setInicialTime(&tiempo);
    //Menú local/remoto
    
    //Menú secuencia
    
    //if(secuencia == '9'){setTime(&tiempo);}
    
    makeThreads('1', 2);
    makeThreads('2', 2);
    makeThreads('3', 2);
    makeThreads('4', 2);
    makeThreads('5', 2);
    makeThreads('6', 2);
    makeThreads('7', 2);
    makeThreads('8', 2);
    
    printf("Programa finalizado.\n");
    return 0;
}

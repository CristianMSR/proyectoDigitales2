#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "ads1115.h"

int getKeyPress();
int checkKeys(int *);
int password();
void openFile(int);
void setInicialTime(int*);
void setTime(int*);

int main(){
	int tiempo=10;
	//Solicitud de contraseña
	if(password() == -1){return -1;}
	//Lectura inicial del potenciómetro 
	setInicialTime(&tiempo);
	//Selección de modo local/remoto
	setTime(&tiempo);
	//Lectura de teclas
	while(checkKeys(&tiempo) == 0);
	return 0;
}

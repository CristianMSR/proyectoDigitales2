#include <stdio.h>
#include <wiringPiI2C.h>
#include "termios.h"
#include <unistd.h>
#define direccion 0x48 //Al estar conectado ADDR a gnd, la adress es 0x48
#define reg 0x00 //0x00 es la direccion del registro que contiene los valores de conversion del ADC

int main() {
	int velocidad;
	int fd = wiringPiI2CSetup(direccion); /*inicializa la comunicacion con 0x48(ADC)
	y guarda el descriptor de archivo*/
	
	if (fd == -1) {
		printf("Error al configurar el dispositivo I2C.\n");
		return;
	}
	printf("Configure el potenciometro a la velocidad deseada, presione enter cuando este listo\n");
	int tec=0;
	while(tec!=10){
		tec = getchar(); //El usuario ajusta el pote, cuando esta listo aprieta enter y recien ahi el ADC es leido
		int valor = wiringPiI2CReadReg16(fd, reg);  /*Lee 2 byte de datos desde el dispositivo I2C.
		Ya que la entrada analogica del adc se conecta de forma unidireccional(solo valores positivos de 
		tension), el ADC envia 15 bits de datos(0-32767). reg es el valor de la medicion, se guarda en valor
		y colocamos fd para indicar a que dispositivo nos estamos comunicando*/
	}
	close(fd);
	velocidad = valor / 1000;
	printf("Velocidad seteada en %d milisegundos", velocidad);
	
	return velocidad;
}


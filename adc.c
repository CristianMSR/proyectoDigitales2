#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "ads1115.h"
//Lo que hace es recibir la direccion de memoria de una variable, y se la carga con la "velocidad"
void funcion_velocidad(int *velocidad){
	int file;
	const char *filename = "/dev/i2c-1";
	
	if((file = open(filename, O_RDWR)) < 0){
		fprintf(stderr, "Error al abrir el bus I2C\n");
                exit(EXIT_FAILURE);
	}
	
	if(ioctl(file, I2C_SLAVE, ADS1115_ADDRESS) < 0){		
		fprintf(stderr, "Error al seleccionar el dispositivo ADS1115\n");
		close(file);
                exit(EXIT_FAILURE);
	}
	int tec=0;
	int valoradc;
	char opcion = 's';
	while(opcion == 's'){
		valoradc = ads1115_read_single_ended(file, 0);
		printf("La velocidad actual es %.2f, desea recalibrarla? (s/n):\n", valoradc/10000.0);
		opcion = getchar();
		char enter = getchar();
		if(opcion == 'n'){
			*velocidad = valoradc/100*10;
			printf("La velocidad fue seteada en %d milisegundos\n", *velocidad);
			break;
		}
	}
	close(file);
}

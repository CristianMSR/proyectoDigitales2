#include <stdio.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "ads1115.h"

int main(){
	int file;
	const char *filename = "/dev/i2c-1";
	
	if((file = open(filename, O_RDWR)) < 0){
		printf("Error al abrir el bus I2C\n");
		return -1;
	}
	
	if(ioctl(file, I2C_SLAVE, ADS1115_ADDRESS) < 0){		
		printf("Error al seleccionar el dispositivo ADS1115\n");
		close(file);
		return -1;
	}
	int tec=0;
	int valoradc;
	char opcion = 's';
	while(opcion == 's'){
		valoradc = ads1115_read_single_ended(file, 0);
		printf("La velocidad actual es %.2f, %.2fV , desea recalibrarla? (s/n):\n", valoradc/10000.0,valoradc * 4.096/ 32768.0);
		opcion = getchar();
		char enter = getchar();
		if(opcion == 'n'){
			int velocidad = valoradc/100*10;
			printf("La velocidad fue seteada en %d milisegundos\n", velocidad);
			break;
		}
	}
	close(file);
	return 0;
}

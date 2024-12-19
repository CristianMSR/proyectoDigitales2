#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "ads1115.h"

extern int tiempo;

int ads1115_read_single_ended(int file, int channel) {
    if (channel < 0 || channel > 3) {
        printf("Canal fuera de rango (0-3)\n");
        return -1;
    }

    int config = 0x8000;  // Bit 15: modo de disparo único
    config |= (0x4000 | (channel << 12));  // Bits 14-12: MUX para seleccionar el canal
    config |= 0x0200;  // Bits 11-9: Configurar el PGA (rango +/- 4.096V)
    config |= 0x0100;  // Bit 8: Modo de disparo único
    config |= 0x0080;  // Bits 7-5: Data rate (128 muestras por segundo)
    config |= 0x0003;  // Bits 1-0: Configuración de comparador (modo de umbral desactivado)

    char config_buffer[3];
    config_buffer[0] = ADS1115_REG_CONFIG;
    config_buffer[1] = (config >> 8) & 0xFF;
    config_buffer[2] = config & 0xFF;

    if (write(file, config_buffer, 3) != 3) {
        printf("Error al configurar el ADS1115\n");
        return -1;
    }

    usleep(8000);

    char reg_buffer[1] = { ADS1115_REG_CONVERSION };
    if (write(file, reg_buffer, 1) != 1) {
        printf("Error al solicitar el registro de conversión\n");
        return -1;
    }

    char data_buffer[2];
    if (read(file, data_buffer, 2) != 2) {
        printf("Error al leer la conversión del ADS1115\n");
        return -1;
    }

    int16_t adc_value = (data_buffer[0] << 8) | data_buffer[1];
    return adc_value;
}

void openFile(int *file){
	const char *filename = "/dev/i2c-1";
	
	if((*file = open(filename, O_RDWR)) < 0){
		fprintf(stderr, "Error al abrir el bus I2C\n");
                exit(EXIT_FAILURE);
	}
	
	if(ioctl(*file, I2C_SLAVE, ADS1115_ADDRESS) < 0){		
		fprintf(stderr, "Error al seleccionar el dispositivo ADS1115\n");
		close(*file);
                exit(EXIT_FAILURE);
	}
}

void setInicialTime(){
	int file;
	openFile(&file);
	int valoradc;
	valoradc = ads1115_read_single_ended(file, 0);
	tiempo = valoradc/100*10;
	if(tiempo < 10) tiempo = 10;
	printf("La velocidad inicial es de %d milisegundos\n", tiempo);
	close(file);
}

//Lo que hace es recibir la direccion de memoria de una variable, y se la carga con la "velocidad"
void setTime(){
	int file;
	openFile(&file);
	int tec=0;
	int valoradc;
	char opcion = 's';
	while(opcion == 's'){
		valoradc = ads1115_read_single_ended(file, 0);
		printf("La velocidad actual es de %d milisegundos, desea recalibrarla? (s/n):\n", valoradc/100*10);
		opcion = getchar();
		char enter = getchar();
		if(opcion == 'n'){
			tiempo = valoradc/100*10;
			if(tiempo < 10) tiempo = 10;
			printf("La velocidad fue seteada en %d milisegundos\n", tiempo);
			break;
		}
	}
	close(file);
}

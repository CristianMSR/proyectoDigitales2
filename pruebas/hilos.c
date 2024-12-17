#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "easypio.h"
#include "sequence.h"

void* hilo1_funcion(void* argumento){
	sleep(5);
	printf("Hilo 1 \n");
	sleep(1);
	return NULL;
}

void* hilo2_funcion(void* argumento){
	fantasticar(100);
	return NULL;
}


int main(){
	pthread_t hilo1, hilo2;

	pioInit();
	sequenceInit();
	printf("1\n");
	delayMillis(1000);
	if(pthread_create(&hilo1, NULL, hilo1_funcion, NULL) != 0){
		perror("Error creando el hilo1");
		exit(EXIT_FAILURE);
	}

	if(pthread_create(&hilo2, NULL, hilo2_funcion, NULL) != 0){
		perror("Error creando el hilo2");
		exit(EXIT_FAILURE);
	}

	printf("2\n");
	delayMillis(1000);
		
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
	
	printf("Programa terminado\n");
	return 0;
}

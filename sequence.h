#ifndef sequence
#define sequence

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "easypio.h"

#define D1 26
#define D2 21
#define D3 20
#define D4 16
#define D5 12
#define D6 25
#define D7 24
#define D8 23

const int leds[8] = {D1,D2,D3,D4,D5,D6,D7,D8};

const int race[16] = {128,128,64,64,32,32,16,16,136,72,36,20,10,6,3,1};
const int jump[14] = {128,64,128,32,128,16,128,8,128,4,128,2,128,1};
const int alt[16] = {0xAA, 0x55};

extern void digitalWriteAll(int);

void sequenceInit(){
	for(int i=0; i<8;i++){
		pinMode(leds[i], OUTPUT);
	}	
}
/*
void digitalWriteAll(int value){
	for(int i=0; i<8; i++){
		int bitValue = (value >> i) & 0x01;
		digitalWrite(leds[i],bitValue);
	}
}
*/
void digitalPrintAll(int value){
	for(int i=0; i<8; i++){	
		if ((value >> i) & 1) {
            printf("*");
        } else {
            printf("-");
        }
    }
    printf("\n");
}
/*

void fantasticar(int tiempo){	
	for(int i=0; i<7; i++){
		digitalWriteAll(0x80 >> i);
		delayMillis(tiempo);
	}	
	for(int i=0;i<7; i++){
		digitalWriteAll(0x01 << i);
		delayMillis(tiempo);
	}
}
 
void choque(int tiempo){
	for(int i=0; i<4; i++){
		digitalWriteAll((0x80 >> i) | (0x1 << i));
		delayMillis(tiempo);
	}
	for(int i=5; i<8; i++){
		digitalWriteAll((0x80 >> i) | (0x1 << i));
		delayMillis(tiempo);
	}
}

void apilada(int tiempo){
	for(int i=0; i<8; i++){
		for(int j=0; j<(8-i); j++){
			digitalWriteAll((0x80 >> j)|((1<<i)-1));
			delayMillis(tiempo);
		}
		digitalWrite(leds[0+i],0);
		delayMillis(tiempo);
		digitalWrite(leds[0+i],1);
		delayMillis(tiempo);
	}
}
 
void theRace(int tiempo){
	for(int i=0; i<16; i++){
		digitalWriteAll(race[i]);
		delayMillis(tiempo);
	}
}
	
void rusky(int tiempo){
	srand(time(NULL));
	digitalWriteAll(0xFF); //prendemos todos los leds
	delayMillis(tiempo);
	int control[8]={1,1,1,1,1,1,1,1};
	int contador = 0;
	
	while(1){
		int random = rand() % 8; //crea el numero random
		
		if(control[random] != 0){
			control[random] = 0;
			digitalWrite(leds[random], 0);
			contador ++;
		}
		
		delayMillis(tiempo);
		if(contador == 8)
		   break;
		
	}
}

void jumpy(int tiempo){
	for(int i=0; i<14; i++){
		digitalWriteAll(jump[i]);
		delayMillis(tiempo);
	}
}

void alternate(int tiempo){
	for(int i=0; i<2; i++){
		digitalWriteAll(alt[i]);
		delayMillis(tiempo);
	}
}

void binary(int tiempo){
	for(int i=0; i<255; i++){
		digitalWriteAll(i);
		delayMillis(tiempo);
	}
}*/


#endif

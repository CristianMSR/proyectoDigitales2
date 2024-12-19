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

void sequenceInit(){
	for(int i=0; i<8;i++){
		pinMode(leds[i], OUTPUT);
	}	
}

void digitalWriteAll(int value){
	for(int i=0; i<8; i++){
		int bitValue = (value >> i) & 0x01;
		digitalWrite(leds[i],bitValue);
	}
}

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

#endif

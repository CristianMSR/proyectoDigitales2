#ifndef EasyPio
#define EasyPio

#include <sys/mman.h> //Mapeo de memoria
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h>

/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////


// GPIO FSEL Types
#define INPUT  0
#define OUTPUT 1
#define ALT0   4 //Otros modos, modos especiales, por ejemplo Tx y Rx
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

//GPIO Registers
#define GPFSEL   ((volatile unsigned int *) (gpio + 0))  //Selección de pin
#define GPSET    ((volatile unsigned int *) (gpio + 7))  //Escribe 1
#define GPCLR    ((volatile unsigned int *) (gpio + 10)) //Escribe 0
#define GPLEV    ((volatile unsigned int *) (gpio + 13)) //Lee

// Physical addresses - Memory Map
#define BCM2836_PERI_BASE        0x3F000000 //Dirección base, depende del modelo de raspi
#define TIMER_BASE 	        (BCM2836_PERI_BASE + 0x3000)
#define GPIO_BASE               (BCM2836_PERI_BASE + 0x200000) //Dirección de los GPIO
#define SPI_BASE 		(BCM2836_PERI_BASE + 0x204000)
#define BLOCK_SIZE (4*1024)   //Tamaño del bloque a mapear

// Pointers that will be memory mapped when pioInit() is called
volatile unsigned int *gpio; //pointer to base of gpio
//volatiles para que se actualicen los valores de los registros

void pioInit() {
	int  mem_fd;
	void *reg_map;

	// /dev/mem is a psuedo-driver for accessing memory in the Linux filesystem
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
	      printf("can't open /dev/mem \n");
	      exit(-1);
	}

	reg_map = mmap(
      NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE, // Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      GPIO_BASE);       // Offset to GPIO peripheral

      gpio = (volatile unsigned *) reg_map;    // user accessible GPIO pins
      close(mem_fd);	
}


void pinMode(int pin, int function) { // sets mode of selected GPIO pin
	int reg = pin/10;
	int offset = (pin%10)*3;
	GPFSEL[reg] &= ~((0b111 & ~function) << offset);
	GPFSEL[reg] |= ((0b111 & function) << offset);
}

void digitalWrite(int pin, int val) { // writes to selected pin
	int reg = pin / 32;
	int offset = pin % 32;
	if (val) GPSET[reg] = 1 << offset;
	else GPCLR[reg] = 1 << offset;
}

int digitalRead(int pin){
	int reg = pin/32;
	int offset = pin%32;
	return (GPLEV[reg]>>offset) & 0x00000001;
}

#endif

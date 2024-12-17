#ifndef ADS1115_H
#define ADS1115_H


// Dirección del ADS1115
#define ADS1115_ADDRESS 0x48

// Definiciones de los registros del ADS1115
#define ADS1115_REG_CONVERSION 0x00
#define ADS1115_REG_CONFIG 0x01

// Prototipo de la función
int ads1115_read_single_ended(int file, int channel);

#endif

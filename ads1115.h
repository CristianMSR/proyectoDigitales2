#ifndef ads1115
#define ads1115

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

// Dirección del ADS1115 (ajustable si los pines ADDR están configurados diferente)
#define ADS1115_ADDRESS 0x48

// Definiciones de los registros del ADS1115
#define ADS1115_REG_CONVERSION 0x00
#define ADS1115_REG_CONFIG 0x01

// Función para configurar y leer el valor del ADC
int ads1115_read_single_ended(int file, int channel) {
    // Validar que el canal esté entre 0 y 3
    if (channel < 0 || channel > 3) {
        printf("Canal fuera de rango (0-3)\n");
        return -1;
    }

    // Config básica para lectura de un solo canal, modo de disparo único, rango +/- 6.144V
    int config = 0x8000;  // Bit 15: modo de disparo único
    config |= (0x4000 | (channel << 12));  // Bits 14-12: MUX para seleccionar el canal
    config |= 0x0200;  // Bits 11-9: Configurar el PGA (rango +/- 2.048V)
    config |= 0x0100;  // Bit 8: Modo de disparo único
    config |= 0x0080;  // Bits 7-5: Data rate (128 muestras por segundo)
    config |= 0x0003;  // Bits 1-0: Configuración de comparador (modo de umbral desactivado)

    // Convertir la configuración en dos bytes para enviarlos al registro de configuración
    char config_buffer[3];
    config_buffer[0] = ADS1115_REG_CONFIG;
    config_buffer[1] = (config >> 8) & 0xFF;  // Byte alto
    config_buffer[2] = config & 0xFF;         // Byte bajo

    // Enviar la configuración al ADS1115
    if (write(file, config_buffer, 3) != 3) {
        printf("Error al configurar el ADS1115\n");
        return -1;
    }

    // Esperar el tiempo necesario para que se complete la conversión (8 ms para data rate de 128 SPS)
    usleep(8000);

    // Leer el registro de conversión
    char reg_buffer[1] = { ADS1115_REG_CONVERSION };
    if (write(file, reg_buffer, 1) != 1) {
        printf("Error al solicitar el registro de conversión\n");
        return -1;
    }

    // Leer los dos bytes de datos del ADC
    char data_buffer[2];
    if (read(file, data_buffer, 2) != 2) {
        printf("Error al leer la conversión del ADS1115\n");
        return -1;
    }

    // Combinar los dos bytes leídos en un solo valor de 16 bits
    int16_t adc_value = (data_buffer[0] << 8) | data_buffer[1];

    // El ADS1115 devuelve un valor de 16 bits en complemento a dos, así que ya está listo para ser interpretado.
    return adc_value;
}

#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h> // Para usleep
#include "easypio.h"
#include "sequence.h"
#include "keyboard.h"
#include "ads1115.h"

// Prototipos de funciones
void* fantasticar(void* arg); 
void* thread_fantasticar(void* arg); 
void* choque(void* arg);
void* apilada(void* arg);
void* theRace(void* arg);
void* rusky(void* arg);
void* jumpy(void* arg);
void* alternate(void* arg);
void* binary(void* arg);
void* thread_checkKeys(void* arg);


// Variables compartidas
int tiempo = 100; // Tiempo inicial en milisegundos
int running = 0; // Bandera para terminar el programa
pthread_mutex_t lock; // Mutex para proteger la variable tiempo

// Función que simula la detección de teclas (placeholder)
int getKeyPress() {
    struct termios oldt, newt;
    int ch;

    // Configurar terminal para lectura sin bloqueo
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    // Restaurar la configuración original del terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// Función fantasticar
void* fantasticar(void* arg) {
    while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 7 && !running; i++) {
            digitalWriteAll(0x80 >> i);
            delayMillis(delay);
        }
        for (int i = 0; i < 7 && !running; i++) {
            digitalWriteAll(0x01 << i);
            delayMillis(delay);
        }
    }
    return NULL;
}

void *choque(void *arg) {
    while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 4 && !running; i++) {
            digitalWriteAll((0x80 >> i) | (0x1 << i));
            delayMillis(delay);
        }
        for (int i = 5; i < 8 && !running; i++) {
            digitalWriteAll((0x80 >> i) | (0x1 << i));
            delayMillis(delay);
        }
    }
    return NULL;
}

void *apilada(void *arg) {
    while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 8 && !running; i++) {
            for (int j = 0; j < (8 - i) && !running; j++) {
                digitalWriteAll((0x80 >> j) | ((1 << i) - 1));
                delayMillis(delay);
            }
            digitalWrite(leds[0 + i], 0);
            delayMillis(delay);
            digitalWrite(leds[0 + i], 1);
            delayMillis(delay);
        }
    }
    return NULL;
}

void *theRace(void *arg) {
    while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 16 && !running; i++) {
            digitalWriteAll(race[i]);
            delayMillis(delay);
        }
    }
    return NULL;
}

void *rusky(void *arg) {
    while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);


        srand(time(NULL));
        digitalWriteAll(0xFF); // Prender todos los LEDs
        delayMillis(delay);
        int control[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        int contador = 0;
        while (contador < 8 && !running) {
            int random = rand() % 8;
            if (control[random] != 0) {
                control[random] = 0;
                digitalWrite(leds[random], 0);
                contador++;
            }
            delayMillis(delay);
        }
    }
    return NULL;
}

void *jumpy(void *arg) {
     while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 14 && !running; i++) {
            digitalWriteAll(jump[i]);
            delayMillis(delay);
        }
    }
    return NULL;
}

void *alternate(void *arg) {
    while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 2 && !running; i++) {
            digitalWriteAll(alt[i]);
            delayMillis(delay);
        }
    }
    return NULL;
}

void *binary(void *arg) {
    while (!running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 255 && !running; i++) {
            digitalWriteAll(i);
            delayMillis(delay);
        }
    }
    return NULL;
}

// Función checkKeys
void* thread_checkKeys(void* arg) {
    while (!running) {
        int key = getKeyPress();
        if (key == '\033') { // Secuencia de escape
            getKeyPress(); // Ignorar '['
            key = getKeyPress();
            pthread_mutex_lock(&lock);
            if (key == 'A') { // Flecha hacia arriba
                tiempo += 10;
                printf("Tiempo incrementado: %d\n", tiempo);
            } else if (key == 'B') { // Flecha hacia abajo
                if (tiempo > 10) {
                    tiempo -= 10;
                    printf("Tiempo decrementado: %d\n", tiempo);
                } else {
                    printf("El tiempo ya está en el mínimo permitido: %d\n", tiempo);
                }
            }
            pthread_mutex_unlock(&lock);
        } else if (isalpha(key)) { // Si es una letra
            printf("Letra detectada ('%c'). Finalizando programa.\n", key);
            running = 1; // Señalar finalización
        }
    }
    return NULL;
}

// Función principal
int main() {
    pthread_t thread1, thread2;
    pioInit();
    sequenceInit();
    
    //Solicitud de contraseña
	if(password() == -1){return -1;}  
    //Lectura inicial del potenciómetro 
	setInicialTime(&tiempo);
    
    //
	setTime(&tiempo);
    
    // Inicializar el mutex
    pthread_mutex_init(&lock, NULL);

    // Crear hilos
    pthread_create(&thread1, NULL, choque, NULL);
    pthread_create(&thread2, NULL, thread_checkKeys, NULL);

    // Esperar a que los hilos terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destruir el mutex
    pthread_mutex_destroy(&lock);
    

    printf("Programa finalizado.\n");
    return 0;
}

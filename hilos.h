#ifndef HILOS_H
#define HILOS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h> 
#include "easypio.h"
#include "sequence.h"
#include "keyboard.h"
#include "ads1115.h"

pthread_t thread1, thread2;
// Variables compartidas
int running = 1; // Bandera para terminar el programa
pthread_mutex_t lock; // Mutex para proteger la variable tiempo
extern int tiempo;

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

void* fantasticar(void* arg) {
    while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 7 && running; i++) {
            digitalWriteAll(0x80 >> i);
            delayMillis(delay);
        }
        for (int i = 0; i < 7 && running; i++) {
            digitalWriteAll(0x01 << i);
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void *choque(void *arg) {
    while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 4 && running; i++) {
            digitalWriteAll((0x80 >> i) | (0x1 << i));
            delayMillis(delay);
        }
        for (int i = 5; i < 8 && running; i++) {
            digitalWriteAll((0x80 >> i) | (0x1 << i));
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void *apilada(void *arg) {
    while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 8 && running; i++) {
            for (int j = 0; j < (8 - i) && running; j++) {
                digitalWriteAll((0x80 >> j) | ((1 << i) - 1));
                delayMillis(delay);
            }
            digitalWrite(leds[0 + i], 0);
            delayMillis(delay);
            digitalWrite(leds[0 + i], 1);
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void *theRace(void *arg) {
    while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 16 && running; i++) {
            digitalWriteAll(race[i]);
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void *rusky(void *arg) {
    while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);


        srand(time(NULL));
        digitalWriteAll(0xFF); // Prender todos los LEDs
        delayMillis(delay);
        int control[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        int contador = 0;
        while (contador < 8 && running) {
            int random = rand() % 8;
            if (control[random] != 0) {
                control[random] = 0;
                digitalWrite(leds[random], 0);
                contador++;
            }
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void *jumpy(void *arg) {
     while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 14 && running; i++) {
            digitalWriteAll(jump[i]);
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void *alternate(void *arg) {
    while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 2 && running; i++) {
            digitalWriteAll(alt[i]);
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void *binary(void *arg) {
    while (running) {
        pthread_mutex_lock(&lock);
        int delay = tiempo; // Copia local del tiempo
        pthread_mutex_unlock(&lock);

        for (int i = 0; i < 255 && running; i++) {
            digitalWriteAll(i);
            delayMillis(delay);
        }
    }
    pthread_exit(NULL);
}

void* thread_checkKeys(void* arg) {
    while (running) {
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
            running = 0; // Señalar finalización
        }
    }
    pthread_exit(NULL);
}


void makeThreads(short secuencia){

    // Inicializar el mutex
    pthread_mutex_init(&lock, NULL);

    // Crear hilos
    switch(secuencia){
        case 1: pthread_create(&thread1, NULL, fantasticar, NULL);
        break;
        case 2: pthread_create(&thread1, NULL, choque, NULL);
        break;
        case 3: pthread_create(&thread1, NULL, apilada, NULL);
        break;
        case 4: pthread_create(&thread1, NULL, theRace, NULL);
        break;
        case 5: pthread_create(&thread1, NULL, rusky, NULL);
        break;
        case 6: pthread_create(&thread1, NULL, jumpy, NULL);
        break;
        case 7: pthread_create(&thread1, NULL, alternate, NULL);
        break;
        case 8: pthread_create(&thread1, NULL, binary, NULL);
        break;
    }
    
    pthread_create(&thread2, NULL, thread_checkKeys, NULL);

    // Esperar a que los hilos terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Destruir el mutex
    pthread_mutex_destroy(&lock);
    running = 1;
}

#endif

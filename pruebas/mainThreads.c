#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h> // Para usleep
#include "easypio.h"
#include "sequence.h"


extern void fantasticar(int tiempo);
extern void choque(int tiempo);
extern void apilada(int tiempo);
extern void theRace(int tiempo);
extern void rusky(int tiempo);
extern void jumpy(int tiempo);
extern void alternate(int tiempo);
extern void binary(int tiempo);

pthread_mutex_t mutex;
int tiempo = 100; // Tiempo inicial en ms.
int running = 1; // Bandera para controlar la ejecución de los hilos.

void *secuenciaThread(void *arg) {
    void (*secuencia)(int) = arg;

    while (running) {
        pthread_mutex_lock(&mutex);
        int localTiempo = tiempo;
        pthread_mutex_unlock(&mutex);

        secuencia(localTiempo);
    }

    pthread_exit(NULL);
}
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

int checkKeys(int *tiempo) {
    //printf("Tiempo inicial: %d\n", tiempo);
    //printf("Usa las flechas hacia arriba y hacia abajo para ajustar el tiempo.\n");
    //printf("Presiona cualquier letra para salir.\n");
    int key = getKeyPress();
    if (key == '\033') { // Secuencia de escape para teclas especiales
        getKeyPress();  // Ignorar el siguiente carácter '['
        key = getKeyPress();
        if (key == 'A') { // Flecha hacia arriba
            *tiempo += 10;
            printf("Tiempo incrementado: %d\n", *tiempo);
            } else if (key == 'B') { // Flecha hacia abajo
                if (*tiempo > 10) {
                    *tiempo -= 10;
                    printf("Tiempo decrementado: %d\n", *tiempo);
                } else {
                    printf("El tiempo ya está en el mínimo permitido:%d\n", *tiempo);
                }
            }
        } else if (isalpha(key)) { // Verificar si es una letra
            printf("Letra detectada ('%c'). Saliendo del programa.\n", key);
            return -1;
        }
    return 0;
}


void *checkKeysThread(void *arg) {
    while (running) {
        if (checkKeys(&tiempo) == -1) {
            pthread_mutex_lock(&mutex);
            running = 0;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threadSecuencia, threadKeys;
    pthread_mutex_init(&mutex, NULL);
    pioInit();
    sequenceInit();

    void (*secuencias[])(int) = {fantasticar, choque, apilada, theRace, rusky, jumpy, alternate, binary};
    int numSecuencias = sizeof(secuencias) / sizeof(secuencias[0]);

    for (int i = 0; i < numSecuencias; i++) {
        running = 1;

        // Crear el hilo de la secuencia seleccionada.
        if (pthread_create(&threadSecuencia, NULL, secuenciaThread, secuzencias[i]) != 0) {
            perror("Error al crear el hilo de secuencia");
            exit(1);
        }

        // Crear el hilo para checkKeys.
        if (pthread_create(&threadKeys, NULL, checkKeysThread, NULL) != 0) {
            perror("Error al crear el hilo de checkKeys");
            exit(1);
        }

        // Esperar a que los hilos terminen.
        pthread_join(threadSecuencia, NULL);
        pthread_join(threadKeys, NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}

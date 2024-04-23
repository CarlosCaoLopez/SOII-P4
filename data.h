#ifndef SOII_PRACTICA2_DATA_H
#define SOII_PRACTICA2_DATA_H

#include <semaphore.h>

/* Tamaño de los buffers */
#define N_1 8
#define N_2 10

#define BUC 10

typedef struct{
    int buffer_size;
    int * buffer;
    /* Semáforos buffer para este buffer */
    sem_t* mutex; 
    sem_t* empty;
    sem_t* full;
}mem_shared;


#endif //SOII_PRACTICA2_DATA_H

#include "producer.h"


int produce_item(){

    return(get_rand());
}

void insert_item(mem_shared *mem_map, int item){
    int count, i;
    struct timeval tv;

    /* Inicializamos la semilla */
    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    /* Obtemos el número de elementos en el buffer */
    sem_getvalue(mem_map->full, &count);

    /* Insertamos en la posición correspondiente */
    mem_map->buffer[count] = item;
    
    /* Printeamos el buffer para constatar posibles carreras críticas */
    printf("Buffer [%d] PRODUCTOR : ", mem_map->buffer_size);
    for(i = 0; i<mem_map->buffer_size; i++){
        printf("| %d |\t", mem_map->buffer[i]);
    }
    printf("\n");
}


void producer(mem_shared *mem_map, int iter){
    int item; /* Elemento a extraer del buffer */
    int count;
    int i=0; /* Contador del bucle*/

     /* Comprobación para evitar el interbloqueo */
    sem_getvalue(mem_map->full, &count);
    if(count + iter > mem_map->buffer_size){ // En caso de que se vayan a quitar más elementos 
        return;
    }

    while(i<iter){

        /* Metemos un sleep fuera de la región crítica */

        sleep(rand()%3);
        item = produce_item(); /* Producimos un entero aleatorio entre 0 y 10 */
        sem_wait(mem_map->empty); /* down al semáforo empty*/
        sem_wait(mem_map->mutex); /* down al semáforo mutex*/
        insert_item(mem_map, item); /* insertamos el item en el buffer */
        sem_post(mem_map->mutex); /* up al semáforo mutex*/
        sem_post(mem_map->full); /* up al semáforo full*/


        i++; /* Aumentamos el contador */
    }
}

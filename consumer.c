#include "consumer.h"


int remove_item(mem_shared* mem_map, int* sum){
    int i, item, count;

    /* Calculamos count a partir de empty, que está actualizado, en vez de full*/
    sem_getvalue(mem_map->empty, &count);
    count = (mem_map->buffer_size - 1)-count;


    item = mem_map->buffer[count];
    // mem_map->buffer[mem_map->count]=-17; //Indicamos valor no válido
    //item = mem_map[mem_map[0] + 1];

    *sum = item;
    for(i=0; i<count; i++) *sum += mem_map->buffer[i]; // Sumamos todos menos el que hemos eliminado
    mem_map->buffer[count] = -1;

    /* Printeamos el buffer para constatar posibles carreras críticas */
    printf("Buffer [%d] CONSUMIDOR : ", mem_map->buffer_size);
    for(i = 0; i<mem_map->buffer_size; i++){
        printf("| %d |\t", mem_map->buffer[i]);
    }
    printf("\n");

    return item;
}

void consume_item(int item, int sum, mem_shared *mem_map){
    printf("Item extraído buffer [%d] :\t %d\t Suma: %d\n",mem_map->buffer_size, item, sum);
}


void consumer(mem_shared *mem_map, int iter){
    int i=0;    
    int item, sum=0, count;

    /* Comprobación para evitar el interbloqueo */
    sem_getvalue(mem_map->empty, &count);
    if( count + iter > mem_map->buffer_size){ // En caso de que se vayan a quitar más elementos 
        return;
    }
    while(i<iter){

        /* Metemos un sleep fuera de la región crítica */
        sleep(rand()%3);
        sem_wait(mem_map->full);
        sem_wait(mem_map->mutex);
        item = remove_item(mem_map, &sum);
        sem_post(mem_map->mutex);
        sem_post(mem_map->empty);
        consume_item(item, sum, mem_map);

        i++;
    }
}

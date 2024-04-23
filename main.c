#include "consumer.h"
#include "producer.h"
#include <sys/mman.h>


int main() {
    mem_shared * mem_map_1, * mem_map_2;
    pid_t child;
    int i, * iter;

    /* Unlink de los semáforos */

    sem_unlink("empty_1");
    sem_unlink("mutex_1");
    sem_unlink("full_1");

    sem_unlink("empty_2");
    sem_unlink("mutex_2");
    sem_unlink("full_2");



    /* Creación del struct del buffer 1 */
    if((mem_map_1 = (mem_shared*)mmap(NULL, sizeof(mem_shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
        fprintf(stderr, "Error al hacer mmap\n");
        exit(EXIT_FAILURE);
    }
    /* Compartir memoria para el buffer del struct 1 */
    if((mem_map_1->buffer = (int *)mmap(NULL, sizeof(int) * N_1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
        fprintf(stderr, "Error al hacer mmap\n");
        exit(EXIT_FAILURE);
    } 
    mem_map_1->buffer_size = N_1;

    /* Creación del buffer 2 */
    if((mem_map_2 = (mem_shared*)mmap(NULL, sizeof(mem_shared), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
        fprintf(stderr, "Error al hacer mmap\n");
        exit(EXIT_FAILURE);
    }
    /* Compartir memoria para el buffer del struct 2 */
    if((mem_map_2->buffer = (int *)mmap(NULL, sizeof(int) * N_2, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
        fprintf(stderr, "Error al hacer mmap\n");
        exit(EXIT_FAILURE);
    } 
    mem_map_2->buffer_size = N_2;

    /* Creación de una variable compartida para generar numeros aleatorios */
    /* Su modificación se hace mediante un mutex para asegurar que en cada iteración se tiene el mismo valor en ambos procesos */
    if((iter = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED){
        fprintf(stderr, "Error al hacer mmap\n");
        exit(EXIT_FAILURE);
    } 



    /* Inicializamos los semáforos del buffer 1 */
    mem_map_1->empty = sem_open("empty_1", O_CREAT, 0700, N_1);
    mem_map_1->mutex = sem_open("mutex_1", O_CREAT, 0700, 1);
    mem_map_1->full = sem_open("full_1", O_CREAT, 0700, 0);

    /* Inicializamos los semáforos del buffer 2 */
    mem_map_2->empty = sem_open("empty_2", O_CREAT, 0700, N_2);
    mem_map_2->mutex = sem_open("mutex_2", O_CREAT, 0700, 1);
    mem_map_2->full = sem_open("full_2", O_CREAT, 0700, 0);


    if((child=fork()) == 0){ /* Código asociado al hijo */

        /* El hijo abre los semáforos inicializados previamente */
        mem_map_1->empty = sem_open("empty_1",0);
        mem_map_1->mutex = sem_open("mutex_1",0);
        mem_map_1->full = sem_open("full_1",0);

        mem_map_2->empty = sem_open("empty_2",0);
        mem_map_2->mutex = sem_open("mutex_2",0);
        mem_map_2->full = sem_open("full_2",0);
        for(i = 0; i<BUC; i++){

            consumer(mem_map_1, (int) get_rand() % 4 );
            producer(mem_map_2, (int) get_rand() % 4 );
        }
    }
    else{ /* Código asociado al padre */
        for(i = 0; i<BUC; i++){ // Para que no se bloqueen mutuamente, hay que cambiar los roles de orden con respecto al otro proceso

            producer(mem_map_1, (int) get_rand() % 4);
            consumer(mem_map_2, (int) get_rand() % 4);
        }
        
    }

    /* Cerramos los semáforos */

    sem_close(mem_map_1->mutex);
    sem_close(mem_map_1->empty);
    sem_close(mem_map_1->full);

    sem_close(mem_map_2->mutex);
    sem_close(mem_map_2->empty);
    sem_close(mem_map_2->full);

    /* Unlink de los semáforos */

    sem_unlink("empty_1");
    sem_unlink("mutex_1");
    sem_unlink("full_1");

    sem_unlink("empty_2");
    sem_unlink("mutex_2");
    sem_unlink("full_2");

    /* Liberamos las proyecciones de memoria */
    munmap(mem_map_1->buffer, sizeof(int)*N_1);
    munmap(mem_map_2->buffer, sizeof(int)*N_2);
    munmap(mem_map_1, sizeof(mem_shared));
    munmap(mem_map_2, sizeof(mem_shared));

    return 0;
}

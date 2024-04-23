#include "consumer.h"
#include "producer.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>



mqd_t create_postbox(const char* postbox_name, int num_messages, int message_size, int oflag);

int main() {
    pid_t child;
    mqd_t consumer_box, producer_box;

    /* Borrado de los buffers de entrada por si exist´ıan de una ejecuci´on previa*/
    mq_unlink("/buzon1");
    mq_unlink("/buzon2");

    /* Creamos el buzón del productor y del consumidor */
    consumer_box = create_postbox("/buzon1", N, MAX_MSG_SIZE, O_CREAT | O_RDWR);
    producer_box = create_postbox("/buzon2", N, MAX_MSG_SIZE,O_CREAT | O_RDWR);



    if((child = fork()) == 0){ /* Código asociado al hijo */

        producer(consumer_box, producer_box);

        exit(1);

    }
    else{

        consumer(consumer_box, producer_box);

    }

    /* Cerramos los buzones */
    mq_close(consumer_box);
    mq_close(producer_box);

    return 0;
}

mqd_t create_postbox(const char* postbox_name, int num_messages, int message_size, int oflag){

    /* Comprobamos que los argumentos estén bien */
    if(num_messages <= 0 || message_size < 1){
        perror("Error en los argumentos, se necesita:\t num_messages > 0 y message_size > 1");
        exit(EXIT_FAILURE);
    }

    // Atributos de la cola de mensajes
    struct mq_attr attr;
    attr.mq_flags = 0;          // Bandera de la cola de mensajes
    attr.mq_maxmsg = num_messages;        // Número máximo de mensajes en la cola
    attr.mq_msgsize = message_size;     // Tamaño máximo de cada mensaje

    // Crear el buzón
    mqd_t mq = mq_open(postbox_name, oflag, 0777, &attr);
    if (mq == (mqd_t)-1) {
        perror("Error creando el buzón");
        exit(EXIT_FAILURE);
    }

    // Imprimir mensaje de éxito
    printf("Buzón creado con éxito.\n");

    return mq;
}

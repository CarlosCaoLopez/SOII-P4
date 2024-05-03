#include "productor_LIFO.h"

int main() {
    mqd_t consumer_box, producer_box;

    /* Borrado de los buffers de entrada por si existían de una ejecución previa*/
    mq_unlink("/buzon1");
    mq_unlink("/buzon2");

    /* Creamos el buzón del productor y del consumidor */
    consumer_box = create_postbox("/buzon1", MAX_BUFFER, MAX_MSG_SIZE, O_CREAT | O_WRONLY);
    producer_box = create_postbox("/buzon2", MAX_BUFFER, MAX_MSG_SIZE,O_CREAT | O_RDONLY);

    producer(consumer_box, producer_box);

    return 0;
}

char produce_item(int i){;

    return 'a' + (i % MAX_BUFFER);
}

void producer(mqd_t postbox_1, mqd_t postbox_2){
    char item; /* Elemento a extraer del buffer */
    int i = 0; /* Contador del bucle*/

    char mensaje[MAX_MSG_SIZE];

    while(i < DATOS_A_PRODUCIR){

        item = produce_item(i); /* Producimos una letra en el intervalo [a,e]*/

        // Espera a que llegue un mensaje vacío.
        mq_receive(postbox_2,mensaje, MAX_MSG_SIZE, NULL);

        // Crear mensaje para enviarlo.
        snprintf(mensaje, MAX_MSG_SIZE, "%c", item);

        // Enviar mensaje al consumidor.
        sscanf(mensaje, "%c", &item);
        printf("Elemento enviado: %c\n", item);
        // El mensaje siempre tendrá menos prioridad que próximo que se vaya a enviar.
        if ( mq_send(postbox_1, mensaje, strlen(mensaje) + 1, i) == -1 )
        {
            perror("mq_send");
            exit(1);
        }

        i++; /* Aumentamos el contador */

    }
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

    // Crear un buzón o abrir el existente
    mqd_t mq = mq_open(postbox_name, oflag, 0777, &attr);
    if (mq == (mqd_t)-1) {
        perror("Error creando el buzón");
        exit(EXIT_FAILURE);
    }

    // Imprimir mensaje de éxito
    printf("Buzón creado con éxito.\n");

    return mq;
}
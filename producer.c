#include "producer.h"


int produce_item(){

    return(get_rand());
}

void producer(mqd_t postbox_1, mqd_t postbox_2){
    int item; /* Elemento a extraer del buffer */
    int i=0; /* Contador del bucle*/

    char mensaje[MAX_MSG_SIZE];

    while(i<iter)
    {


        item = produce_item(); /* Producimos un entero aleatorio entre 0 y 10 */

        // Espera a que llegue un mensaje vacío.
        mq_receive(postbox_2,mensaje, MAX_MSG_SIZE, NULL);

        // Crear mensaje para enviarlo.
        snprintf(mensaje, MAX_MSG_SIZE, "%d", item);

        // Enviar mensaje al consumidor.
        sscanf(mensaje, "%d", &item);
        printf("Elemento enviado: %d\n", item);
        if ( mq_send(postbox_1, mensaje, strlen(mensaje) + 1, 0) == -1 )
        {
            perror("mq_send");
            exit(1);
        }

        i++; /* Aumentamos el contador */
    }
}

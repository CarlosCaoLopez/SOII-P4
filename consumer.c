#include "consumer.h"


/*void consume_item(int item, int sum, mem_shared *mem_map){
    printf("Item extraído buffer [%d] :\t %d\t Suma: %d\n",mem_map->buffer_size, item, sum);
}*/


void consumer(mqd_t postbox_1, mqd_t postbox_2){

    int i=0, j;
    int item;

    char mensaje[MAX_MSG_SIZE];

    /* Envía n mensajes vacíos */
    for (j=0;j<N;j++)
    {
        if ( mq_send(postbox_2, mensaje, strlen(mensaje) + 1, 0) == -1 )
        {
            perror("mq_send");
            exit(1);
        }
    }

    while(i<iter)
    {
        // Obtiene el mensaje que contiene el elemento
        mq_receive(postbox_1,mensaje, MAX_MSG_SIZE, NULL);

        // Extrae el elemento del mensaje
        sscanf(mensaje, "%d", &item);
        printf("Elemento recibido: %d\n", item);

        // Envía de vuelta respuesta con mensaje vacío
        if (mq_send(postbox_2, "", 0, 0) == -1)
        {
            perror("mq_send");
            exit(1);
        }

        i++;
    }
}

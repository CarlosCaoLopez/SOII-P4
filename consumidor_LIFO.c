#include "consumidor_FIFO.h"




int main() {
    char mensaje[MAX_MSG_SIZE];
    mqd_t consumer_box, producer_box;
    int i;

    /* Creamos/abrimos el buzón del productor y del consumidor */
    consumer_box = create_postbox("/buzon1", MAX_BUFFER, MAX_MSG_SIZE, O_RDONLY);
    /* Lo inicializamos de forma que también podamos leer de él ya que será este proceso el encargado de que no queden menajes vacíos en el buffer*/
    producer_box = create_postbox("/buzon2", MAX_BUFFER, MAX_MSG_SIZE, O_RDWR);

    consumer(consumer_box, producer_box);

    /* Como el consumidor envía MAX_BUFFER mensajes antes de entrar al bucle principal, quedarán elementos en ese buzón sin consumir ya que se envían
     * MAX_BUFFER + DATOS_A_PRODUCIR mensajes vacíos.
     * Como desde el productor no sabemos cuando termina el consumidor, los eliminamos desde aquí
     * */

    for (i=0;i<MAX_BUFFER-1;i++)
    {
        if (mq_receive(producer_box,mensaje, MAX_MSG_SIZE, NULL) == -1 )
        {

            perror("mq_receive");
            exit(1);
        }
    }

    /* Cerramos los buzones */
    mq_close(consumer_box);
    mq_close(producer_box);

    return 0;
}

void consumer(mqd_t postbox_1, mqd_t postbox_2){
    int i=0, j;
    char item;

    char mensaje[MAX_MSG_SIZE];

    /* Envía n mensajes vacíos */
    for (j=0;j<MAX_BUFFER;j++)
    {
        if ( mq_send(postbox_2, mensaje, strlen(mensaje) + 1, 0) == -1 )
        {
            perror("mq_send");
            exit(1);
        }
    }

    sleep(1); // Forzamos el primer llenado

    while(i < DATOS_A_PRODUCIR){

        // Obtiene el mensaje que contiene el elemento
        mq_receive(postbox_1,mensaje, MAX_MSG_SIZE, NULL);

        // Extrae el elemento del mensaje
        sscanf(mensaje, "%c", &item);
        printf("Elemento recibido: %c\n", item);

        // Envía de vuelta respuesta con mensaje vacío
        if (mq_send(postbox_2, "", 0, 0) == -1)
        {
            perror("mq_send");
            exit(1);
        }

        i++;

        if(item == 'a'){ // Si has detectado el último elemento LIFO
            sleep(1); // Forzamos que se llene
        }
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
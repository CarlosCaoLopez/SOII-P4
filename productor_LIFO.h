#ifndef SOII_PRACTICA2_PRODUCER_H
#define SOII_PRACTICA2_PRODUCER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include "data.h"
#include <string.h>


mqd_t create_postbox(const char* postbox_name, int num_messages, int message_size, int oflag);
void producer(mqd_t postbox_1, mqd_t postbox_2); /* Ya no hace falta pasarle el tamaño, está declarado aquí */



#endif //SOII_PRACTICA2_PRODUCER_H

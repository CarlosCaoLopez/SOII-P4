#ifndef SOII_PRACTICA2_CONSUMER_H
#define SOII_PRACTICA2_CONSUMER_H

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <mqueue.h>
#include "data.h"
#include <string.h>

mqd_t create_postbox(const char* postbox_name, int num_messages, int message_size, int oflag);
void consumer(mqd_t postbox_1, mqd_t postbox_2);


#endif //SOII_PRACTICA2_CONSUMER_H

#ifndef _MAIN_
#define _MAIN_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "tcp_lib.h"
#include "thread_lib.h"
#include "InputUtil.h"

void * server_listen_thread_func(void * data);
void * input_thread_func(void * data);
void * client_read_thread_func(void * data);
void * client_write_thread_func(void * data);


#endif
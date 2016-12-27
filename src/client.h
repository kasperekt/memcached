#ifndef __CLIENT__H__
#define __CLIENT__H__

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct client_connection_info {
    int id;
    int socket;
} client_conn_info_t;

int create_client_thread(int clientSocket, int client_id);
void* client_action(void *arg);
int generate_client_id();

#endif
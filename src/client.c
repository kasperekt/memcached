#include "client.h"

static const int BUFSIZE = 128;
static int next_client_id = 1;

int generate_client_id() {
    return next_client_id++;
}

int create_client_thread(int socket, int id) {
    pthread_t client_thread;
    client_conn_info_t info = {
        .id = id,
        .socket = socket
    };
    
    return pthread_create(&client_thread, NULL, client_action, &info);
}

void* client_action(void* arg) {
    client_conn_info_t info = *((client_conn_info_t*) arg);

    printf("Connected with client %d\n", info.id);
    
    time_t now;
    struct tm* local;
    time(&now);
    local = localtime(&now);
    
    char buffer[BUFSIZE];
    int n = sprintf(buffer, "%s\n", asctime(local));

    write(info.socket, buffer, n);
    close(info.socket);

    sleep(6);
    printf("Connection with client %d closed.\n", info.id);

    pthread_exit(NULL);
}
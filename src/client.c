#include "client.h"

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

    const char* test_file = "test-image.jpg";
    if (file_exists(test_file)) {
        printf("Attempting to read file...\n");
        mc_file_info_t file = read_file(test_file);
        printf("Attempting to write to socket...\n");
        write(info.socket, file.data, file.size);
        printf("Attempting to free file memory...\n");
        destroy_file(file);
    } else {
        fprintf(stderr, "File [%s] doesn't exist!\n", test_file);
    }

    sleep(6);
    close(info.socket);
    printf("Connection with client %d closed.\n", info.id);

    pthread_exit(NULL);
}
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

#ifdef MC_DEBUG
    printf("Connected with client %d\n", info.id);
#endif

    time_t now;
    struct tm* local;
    time(&now);
    local = localtime(&now);

    const char* test_file_path = "test-image.jpg";
    if (file_exists(test_file_path)) {
        mc_file_info_t* file = get_file(test_file_path);
        write(info.socket, file->data, file->size);
    } else {
        fprintf(stderr, "File [%s] doesn't exist!\n", test_file_path);
    }

    close(info.socket);
    printf("Connection with client %d closed.\n", info.id);

    pthread_exit(NULL);
}
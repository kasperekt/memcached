#include "client.h"

const char MC_ACCEPT = 0xFF;
const char MC_FAILURE = 0x00;

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

    size_t files_count;
    char** files;
    files_count = file_list(&files);

    char response[128];
    int bytes_read = read(info.socket, response, 128);

#ifdef MC_DEBUG
    printf("[%d] client wants to download %s file\n", info.id, response);
#endif

    if (valid_request(files, files_count, response)) {
        write(info.socket, &MC_ACCEPT, 1);
        mc_file_info_t* file = get_file(response);
        write(info.socket, &file->size, sizeof(file->size));
        write(info.socket, file->data, file->size);
    } else {
        write(info.socket, &MC_FAILURE, 1);
        fprintf(stderr, "File %s doesn't exist\n", response);
    }

    free(files);
    close(info.socket);
    printf("Connection with client %d closed.\n", info.id);

    pthread_exit(NULL);
}

int valid_request(char** valid_files, size_t valid_files_size, char* request) {
    for (int i = 0; i < valid_files_size; ++i) {
        if (strcmp(valid_files[i], request) == 0) {
            return 1;
        }
    }

    return 0;
}
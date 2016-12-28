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

    const char* intro_msg = "--- Available files (choose one) ---\n";
    write(info.socket, intro_msg, strlen(intro_msg));

    size_t files_count;
    char** files;
    files_count = file_list(&files);
    
    for (int i = 0; i < files_count; ++i) {
        write(info.socket, files[i], strlen(files[i]));
        write(info.socket, "\n", 1);
    }

    char response[128];
    do {
        read(info.socket, response, 128);
        size_t res_len = strlen(response);
        if (response[res_len - 1] == '\n') {
            response[res_len - 1] = '\0';
        }
    } while (!valid_request(files, files_count, response));

#ifdef MC_DEBUG
    printf("[%d] client wants to download %s file\n", info.id, response);
#endif

    free(files);

    if (file_exists(response)) {
        mc_file_info_t* file = get_file(response);
        write(info.socket, file->data, file->size);
    } else {
        fprintf(stderr, "File %s doesn't exist\n", response);
    }

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
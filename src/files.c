#include "files.h"

static char* STORAGE_PATH = "/tmp";

static char* remove_trailing_slash(const char *path) {
    char *buffer = 0;
    strcpy(buffer, path);
    size_t len = strlen(buffer);

    if ((len > 0) && (buffer[len - 1] == '/')) {
        buffer[len - 1] = 0;
    }

    return buffer;
}

void set_file_storage_path(const char* path) {
    STORAGE_PATH = remove_trailing_slash(path);
}

mc_file_info_t read_file(const char* path) {
    FILE* fileptr;
    mc_file_info_t info;

    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", STORAGE_PATH, path);

    fileptr = fopen(full_path, "rb");
    fseek(fileptr, 0, SEEK_END);
    info.size = ftell(fileptr);
    rewind(fileptr);

    info.data = (char*)malloc((info.size + 1) * sizeof(char));
    fread(info.data, info.size, 1, fileptr);
    fclose(fileptr);

    return info;
}

int file_exists(const char *path) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", STORAGE_PATH, path);

    return access(full_path, F_OK) != -1 ? 1 : 0;
}
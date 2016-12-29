#include "files.h"

static char* STORAGE_PATH = "/tmp";

void set_file_storage_path(const char* path) {
    STORAGE_PATH = calloc(1, sizeof(char) * strlen(path));
    STORAGE_PATH = strdup(path);
}

mc_file_info_t* read_file(const char* path) {
    FILE* fileptr;
    mc_file_info_t* info = malloc(sizeof(mc_file_info_t));

    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", STORAGE_PATH, path);

    fileptr = fopen(full_path, "rb");
    fseek(fileptr, 0, SEEK_END);
    info->size = ftell(fileptr);
    rewind(fileptr);

    info->data = (char*)malloc((info->size + 1) * sizeof(char));
    fread(info->data, info->size, 1, fileptr);
    fclose(fileptr);

    return info;
}

int file_exists(const char *path) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", STORAGE_PATH, path);

    return access(full_path, F_OK) != -1 ? 1 : 0;
}

void destroy_file(mc_file_info_t* file) {
    if (file == NULL) {
        return;
    }

    if (file->data != NULL) {
        free(file->data);
    }

    free(file);
}

size_t file_list(char ***ls) {
    size_t count = 0;
    DIR *dp = NULL;
    struct dirent *ep = NULL;

    dp = opendir(STORAGE_PATH);
    if (dp == NULL) {
        fprintf(stderr, "no such directory: '%s'", STORAGE_PATH);
        return 0;
    }

    *ls = NULL;
    ep = readdir(dp);
    while (ep != NULL) {
        count++;
        ep = readdir(dp);
    }

    rewinddir(dp);
    *ls = calloc(count, sizeof(char*));

    count = 0;
    ep = readdir(dp);
    while (ep != NULL) {
        if (strcmp(ep->d_name, ".") != 0 && strcmp(ep->d_name, "..") != 0) {
            (*ls)[count++] = strdup(ep->d_name);
        }
        
        ep = readdir(dp);
    }

    closedir(dp);
    return count;
}
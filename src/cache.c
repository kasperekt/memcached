#include "cache.h"

static long cache_size = 0;
static long cache_size_taken = 0;
static file_storage_entry_t* storage = NULL;

void set_cache_size(long size) {
    cache_size = size;
}

int store_file(const char* pathname, mc_file_info_t file) {
    file_storage_entry_t* tmp = malloc(sizeof(file_storage_entry_t));
    tmp->next = NULL;
    tmp->pathname = (char*)pathname;
    tmp->file = file;

    if (storage == NULL) {
        storage = tmp;
        return 0;
    }

    file_storage_entry_t* iterator = storage;
    while (iterator->next != NULL) {
        iterator = iterator->next;
    }
    
    iterator->next = tmp;
    cache_size_taken = cache_size_taken + tmp->file.size;
#ifdef MC_DEBUG
    printf("[%s] file saved in cache!\n", tmp->pathname);
#endif
    return 0;
}
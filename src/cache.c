#include "cache.h"

static pthread_mutex_t lock;
static unsigned long cache_size = 0;
static unsigned long cache_size_taken = 0;
static file_storage_entry_t* storage = NULL;

void print_cache_status() {
    printf("Cache status: %lu/%lu\n", cache_size_taken, cache_size);
}

void init_cache_mutex() {
    pthread_mutex_init(&lock, NULL);
}

void destroy_cache_mutex() {
    pthread_mutex_destroy(&lock);
}

void set_cache_size(const unsigned long size) {
    cache_size = size;
}

int store_file(const char* pathname, mc_file_info_t* file) {
    file_storage_entry_t* tmp = malloc(sizeof(file_storage_entry_t));
    tmp->next = NULL;
    tmp->pathname = strdup(pathname);
    tmp->file = file;
    tmp->timestamp = time(NULL);

    cache_size_taken += tmp->file->size;

    if (storage == NULL) {
        storage = tmp;
#ifdef MC_DEBUG
        printf("[%s] file saved in cache (size=%ld)\n", tmp->pathname, tmp->file->size);
#endif
        return 0;
    }

    file_storage_entry_t* iterator = storage;
    while (iterator->next != NULL) {
        iterator = iterator->next;
    }
    
    iterator->next = tmp;
#ifdef MC_DEBUG
    printf("[%s] file saved in cache!\n", tmp->pathname);
#endif
    return 0;
}

mc_file_info_t* get_cached_file(const char* pathname) {
    file_storage_entry_t* iterator = storage;

    while (iterator != NULL) {
        if (strcmp(iterator->pathname, pathname) == 0) {
            return iterator->file;
        }

        iterator = iterator->next;
    }
    
    return NULL;
}

int refresh_timestamp(const char* pathname) {
    file_storage_entry_t* iterator = storage;
    while (iterator != NULL) {
        if (strcmp(iterator->pathname, pathname) == 0) {
            iterator->timestamp = time(NULL);
#ifdef MC_DEBUG
            printf("[%s] Refreshed timestamp: %lu\n", iterator->pathname, iterator->timestamp);
#endif
            return 1;
        }

        iterator = iterator->next;
    }

    return 0;
}

file_storage_entry_t* remove_oldest_entry() {
    if (storage == NULL) {
        return NULL;
    }

    file_storage_entry_t* oldest = storage;
    file_storage_entry_t* before = NULL;
    file_storage_entry_t* after = oldest->next;
    while ((after != NULL) && (oldest->timestamp < after->timestamp)) {
        before = oldest;
        oldest = after;
        if (oldest->next != NULL) {
            after = oldest->next;
        }
    }

    before->next = after;
    cache_size_taken -= oldest->file->size;
    
    return oldest;
}

int fits_in_storage(const mc_file_info_t *file) {
    return file->size + cache_size_taken < cache_size;
}

mc_file_info_t* get_file(const char* pathname) {
    if (!file_exists(pathname)) {
        return NULL;
    }

    pthread_mutex_lock(&lock);
    mc_file_info_t* new_file = get_cached_file(pathname);
    pthread_mutex_unlock(&lock);
    if (new_file != NULL) {
        pthread_mutex_lock(&lock);
        refresh_timestamp(pathname);
        pthread_mutex_unlock(&lock);
        return new_file;
    }

    new_file = read_file(pathname);
    if (new_file->size > cache_size) {
        return new_file;
    }

    pthread_mutex_lock(&lock);
    while (!fits_in_storage(new_file)) {
        file_storage_entry_t* oldest = remove_oldest_entry();        
        destroy_file(oldest->file);
        free(oldest->pathname);
        free(oldest);
    }

    store_file(pathname, new_file);
    pthread_mutex_unlock(&lock);
#ifdef MC_DEBUG
    print_cache_status();
#endif
    return new_file;
}
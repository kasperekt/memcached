#ifndef __CACHE__H__
#define __CACHE__H__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "files.h"

#define MC_DEBUG 1

typedef struct file_storage_entry {
    struct file_storage_entry *next;
    char* pathname;
    mc_file_info_t* file;
    unsigned long timestamp;
} file_storage_entry_t;

void set_cache_size(const unsigned long size);
void init_cache_mutex();
void destroy_cache_mutex();
mc_file_info_t* get_file(const char* pathname);
mc_file_info_t* get_cached_file(const char* pathname);
int store_file(const char* pathname, mc_file_info_t* file);
int fits_in_storage(const mc_file_info_t* file);
file_storage_entry_t* remove_oldest_entry();
int refresh_timestamp(const char* pathname);
void print_cache_status();

#endif
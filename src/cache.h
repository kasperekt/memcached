#ifndef __CACHE__H__
#define __CACHE__H__

#include <stdio.h>

#include "files.h"

typedef struct file_storage_entry {
    struct file_storage_entry *next;
    char* pathname;
    mc_file_info_t file;
} file_storage_entry_t;

void set_cache_size(long size);
mc_file_info_t* get_file(const char* pathname);
int store_file(const char* pathname, mc_file_info_t file);

#endif
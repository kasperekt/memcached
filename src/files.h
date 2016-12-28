#ifndef __FILES__H__
#define __FILES__H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define MC_DEBUG 1

typedef struct memcached_file_info {
    long size;
    char *data;
} mc_file_info_t;

void set_file_storage_path(const char* path);
int file_exists(const char* path);
mc_file_info_t* read_file(const char* path);
void destroy_file(mc_file_info_t* file);

#endif
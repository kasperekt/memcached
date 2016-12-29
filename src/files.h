#ifndef __FILES__H__
#define __FILES__H__

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MC_DEBUG 1

typedef struct memcached_file_info {
    unsigned long size;
    char *data;
} mc_file_info_t;

size_t file_list(char ***ls);
void set_file_storage_path(const char* path);
int file_exists(const char* path);
mc_file_info_t* read_file(const char* path);
void destroy_file(mc_file_info_t* file);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "client.h"
#include "cache.h"

#define MC_DEBUG 1

static const int QUEUE_SIZE = 5;

typedef struct mc_arguments {
    unsigned long cache_size;
    u_int32_t ip;
    int port;
} mc_args_t;

mc_args_t parse_arguments(int argc, char** argv) {
    mc_args_t args = {
        .cache_size = 1024 * 1024 * 30, // 30 MB
        .ip = INADDR_ANY,
        .port = 1234
    };

    if (argc > 1) {
        char* ip_str = argv[1];
        inet_pton(AF_INET, ip_str, &args.ip);
    }   

    if (argc > 2) {
        char* port_str = argv[2];
        args.port = atoi(port_str);
    } 

    if (argc > 3) {
        char* cache_size_str = argv[3];
        args.cache_size = atol(cache_size_str);
    }

    return args;
}

int main(int argc, char** argv) {
    mc_args_t args = parse_arguments(argc, argv);

    struct sockaddr_in stAddr;
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(args.ip);
    stAddr.sin_port = htons(args.port);

    int nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (nSocket < 0) {
        fprintf(stderr, "Can't create a socket\n");
        exit(1);
    }

    int nFoo = 1;
    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    int nBind = bind(nSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0) {
        fprintf(stderr, "Can't bind name to a socket\n");
        exit(1);
    }

    int nListen = listen(nSocket, QUEUE_SIZE);
    if (nListen < 0) {
        fprintf(stderr, "Can't set queue size\n");
        exit(1);
    }

    set_cache_size(args.cache_size);
    
    int cache_size_mb = args.cache_size / (1024 * 1024);
    printf("Server working on %s:%d\nCache size: %d MB\n", inet_ntoa(stAddr.sin_addr), args.port, cache_size_mb);

    socklen_t nTmp;
    int nClientSocket;
    struct sockaddr_in stClientAddr;
    while (1) {
        nTmp = sizeof(struct sockaddr);
        nClientSocket = accept(nSocket, (struct sockaddr*)&stClientAddr, &nTmp);
        if (nClientSocket < 0) {
            fprintf(stderr, "Can't create a connection's socket\n");
            exit(2);
        }

        int client_id = generate_client_id();
        int rc = create_client_thread(nClientSocket, client_id);
        if (rc) {
            fprintf(stderr, "Error. return code from create_client_thread() is %d\n", rc);
            exit(1);
        }
    }

    close(nSocket);
    return 0;
}
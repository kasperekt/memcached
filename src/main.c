#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

static const int SERVER_PORT = 1234;
static const int QUEUE_SIZE = 5;
static const int BUFSIZE = 128;

int main() {
    struct sockaddr_in stAddr;
    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(SERVER_PORT);

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

        printf("Connection from [%s]\n", inet_ntoa((struct in_addr)stClientAddr.sin_addr));
        
        time_t now;
        struct tm* local;
        time(&now);
        local = localtime(&now);
        
        char buffer[BUFSIZE];
        int n = sprintf(buffer, "%s\n", asctime(local));
        
        write(nClientSocket, buffer, n);
        close(nClientSocket);
    }

    close(nSocket);
    return 0;
}
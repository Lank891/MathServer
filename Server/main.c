#define _GNU_SOURCE
#include <HsFFI.h>
#ifdef __GLASGOW_HASKELL__
#include "solver_stub.h"
#endif
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BACKLOG 3
#define MAXLEN 512

#define ERR(source) (fprintf(stderr, "%s: %d\n\t", __FILE__, __LINE__),\
                perror(source), exit(EXIT_FAILURE))

void Usage(){
    fputs("Usage: server (1024 < port < 65536)\n", stderr);
    exit(EXIT_FAILURE);
}
void Log(const char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
}

int BulkReadLine(int fd, char *buf, size_t size) {
    char c;
    int len = 0;

    for(int i = 0; i < size; i++) {
        buf[i] = 0;
    }

    do {
        int s = TEMP_FAILURE_RETRY( read(fd, &c, 1) );
        if(s < 0) return s;
        if(s == 0 || c == 0 || c == '\n') return len;
        buf[len] = c;
        len += 1;
    } while(len < size);

    return len;
}
int BulkWrite(int fd, char *buf, size_t size) {
    int c;
    size_t len = 0;
    do {
        c = TEMP_FAILURE_RETRY(write(fd, buf, size));
        if(c < 0)
            return c;
        buf += c;
        len += c;
        size -= c;
    } while(size > 0);
    return len;
}

int MakeSocket(int domain, int type) {
    int sock;
    sock = socket(domain, type, 0);
    if(sock < 0)
        ERR("socket");
    return sock;
}
int BindTcpSocket(uint16_t port) {
    struct sockaddr_in addr;
    int socketfd, t = 1;
    socketfd = MakeSocket(PF_INET, SOCK_STREAM);
    Log("Socket created");
    memset(&addr, 0, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(t)))
        ERR("setsockopt");
    if(bind(socketfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        ERR("bind");
    Log("Socket binded");
    if(listen(socketfd, BACKLOG) < 0)
        ERR("listen");
    Log("Socked listening");
    return socketfd;
}

void ServerMain(int socket) {
    char buf[MAXLEN];
    char* ans = 0;

    while(1) {
        int client = accept(socket, NULL, NULL);
        if(client < 0)
            ERR("Accept");
        Log("Client accepted");

        if(BulkReadLine(client, buf, MAXLEN) < 0)
            ERR("Read");
        Log(buf);

        ans = rpn(buf);
        if(ans == 0)
            ERR("Haskell function error");
        Log("Answer calculated:");
        Log(ans);

        if(BulkWrite(client, ans, strlen(ans)) < 0)
            ERR("Write");
        Log("Answer sent");

        if(close(client) < 0)
            ERR("Client cleanup");
        free(ans);
        ans = 0;
        Log("Client cleaned");
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2)
        Usage();
    
    int port = atoi(argv[1]);
    if(port < 1025 || port > 65535)
        Usage();

    hs_init(&argc, &argv);

    int socket = BindTcpSocket(port);
    Log("Socket ready");
    ServerMain(socket);

    hs_exit();

    if(close(socket) < 0)
        ERR("close socket");

    Log("Socket closed, returning 0");
    return 0;
}

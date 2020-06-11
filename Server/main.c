#define _GNU_SOURCE
#include <HsFFI.h>
#ifdef __GLASGOW_HASKELL__
#include "solver_stub.h"
#endif
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define ERR(source) (fprintf(stderr, "%s: %d\n\t", __FILE__, __LINE__),\
                perror(source), exit(EXIT_FAILURE))

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

int main(int argc, char *argv[]) {
    char buf[256];
    if( BulkReadLine(0, buf, 256) <= 0 ) 
        ERR("Read line");

    hs_init(&argc, &argv);

    printf("%s", rpn(buf));

    hs_exit();
    return 0;
}

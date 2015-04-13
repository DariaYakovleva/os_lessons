#include "../lib/helpers.h"
#include "../lib/bufio.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

size_t const MAX_LEN = 4096;



int main(int argc, char *argv[]) {
    char *curStr = malloc(MAX_LEN);
    struct buf_t *buffer = buf_new(MAX_LEN);
    struct buf_t *bufWrite = buf_new(MAX_LEN);
    ssize_t len;
    char* args[argc];

    int i = 0;
    while (i < argc - 1) {
        args[i] = argv[i + 1];
        i++;
    }
    if (buffer == NULL) {
        write(STDERR_FILENO, "can't create buffer", 19);
    }
    while ((len = buf_getline(STDIN_FILENO, buffer, curStr)) > 0) {
        *(curStr + len) = 0;
        args[argc - 1] = curStr;
        args[argc] = NULL;
        int res = spawn(argv[1], args);
        if (res == 0) {
            buf_write(STDOUT_FILENO, bufWrite, curStr, len);
        }
    }

    buf_flush(STDOUT_FILENO, bufWrite, bufWrite->size);

    buf_free(buffer);
    buf_free(bufWrite);
    free(curStr);

    return 0;
}


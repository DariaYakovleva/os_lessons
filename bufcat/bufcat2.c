#include "../lib/bufio.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    size_t MAX_LEN = 1024;
    struct buf_t *buffer = buf_new(MAX_LEN);
    if (buffer == NULL) {
        write(STDERR_FILENO, "can't create buffer", 19);
    }
    while ((buf_fill(STDIN_FILENO, buffer, buf_capacity(buffer))) > 0) {
        ssize_t error = buf_flush(STDOUT_FILENO, buffer, buf_size(buffer));
        if (error == -1) {
            write(STDERR_FILENO, "write error", 11);
        }
    }
    buf_free(buffer);
    return 0;
}

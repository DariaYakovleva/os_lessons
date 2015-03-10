#include "../lib/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    size_t const MAX_LEN = 10000;
    void *buf = malloc(MAX_LEN);
    ssize_t len;
    while ((len = read_(STDIN_FILENO, buf, MAX_LEN)) > 0) {
        write_(STDOUT_FILENO, buf, len);
    }
    if (len < 0) {
        write_(STDERR_FILENO, "read error", 5);
    }
    free(buf);
    return 0;
}


#include "../lib/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    size_t const MAX_LEN = 10000;
    void *buf = malloc(MAX_LEN);
    ssize_t len = read_(STDIN_FILENO, buf, MAX_LEN);
    write_(STDOUT_FILENO, buf, len);
    free(buf);
    return 0;
}


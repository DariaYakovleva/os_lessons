#include "../lib/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main() {
    size_t const MAX_LEN = 10000;
    void *buf = malloc(MAX_LEN);
    ssize_t len;
    ssize_t error;
    while ((len = read_(STDIN_FILENO, buf, MAX_LEN)) > 0) {
        error = write_(STDOUT_FILENO, buf, len);
	if (error == -1) {
	    write_(STDERR_FILENO, "write error", 5);
	}
    }
    if (len < 0) {
        write_(STDERR_FILENO, "read error", 5);
    }
    free(buf);
    return 0;
}


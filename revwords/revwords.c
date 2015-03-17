#include "../lib/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

size_t const MAX_LEN = 1 << 17;
ssize_t lenBuf = 0;

ssize_t reversew(void* ans, void* w) {
    int lenw = 0;
    while (lenw < lenBuf && (*((char *)w + lenw)) != ' ' && (*((char *)w + lenw)) != EOF) {
        lenw++;
    }
    if (lenw == lenBuf) {
        return 0;
    }
    ssize_t cur = lenw;
    while (cur > 0) {
        (*((char *)ans + lenw - cur)) = (*((char *)w + cur - 1));
        cur--;
    }
    if ((*((char *)w + lenw)) == ' ') {
        (*((char *)ans + lenw)) = (*((char *)w + lenw));
        lenw++;
    }
    int pos = 0;
    while (pos < lenBuf - lenw) {
        (*((char *)w + pos)) = (*((char *)w + lenw + pos));
        pos++;
    }
    lenBuf -= lenw;
    return lenw;
}

int main() {
    void *buf = malloc(MAX_LEN);
    void *rev = malloc(MAX_LEN);
    ssize_t len;
    while ((len = read_until(STDIN_FILENO, buf + lenBuf, MAX_LEN, ' ')) > 0) {
        lenBuf += len;
        ssize_t lenw;
        while ((lenw = reversew(rev, buf)) > 0) {
            write_(STDOUT_FILENO, rev, lenw);
        }
    }
    if (len < 0) {
        write_(STDERR_FILENO, "read error", 5);
    }
    if (lenBuf > 0) {
        (*((char *)buf + lenBuf)) = EOF;
        lenBuf++;
        ssize_t lenw;
        while ((lenw = reversew(rev, buf)) > 0) {
            write_(STDOUT_FILENO, rev, lenw);
        }
    }
    free(buf);
    free(rev);
    return 0;
}


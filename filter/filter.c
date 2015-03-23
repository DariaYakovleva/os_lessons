#include "../lib/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

size_t const MAX_LEN = 1 << 17;
ssize_t lenBuf = 0;

ssize_t getStr(void* ans, void* w) {
    int lenw = 0;
    while (lenw < lenBuf && (*((char *)w + lenw)) != '\n' && (*((char *)w + lenw)) != EOF) {
        (*((char *)ans + lenw)) = (*((char *)w + lenw));
        lenw++;
    }
    if (lenw == lenBuf) {
        return 0;
    }
    int pos = 0;
    while (pos < lenBuf - lenw) {
        (*((char *)w + pos)) = (*((char *)w + lenw + pos));
        pos++;
    }
    lenBuf -= lenw;
    return lenw;
}

int main(int argc, char *argv[]) {
    //    char* args[] = {"ls", "/bin", NULL};
    //    int res = spawn("/bin/ls", args);
    //    printf("res=%d\n", res);
    void *buf = malloc(MAX_LEN);
    void *curStr = malloc(MAX_LEN);
    ssize_t len;
    while ((len = read_until(STDIN_FILENO, buf + lenBuf, MAX_LEN, '\n')) > 0) {
        lenBuf += len;
        ssize_t lenw;
        while ((lenw = getStr(curStr, buf)) > 0) {
            printf("res=%s\n", (char*)curStr);
            int res = spawn(argv[0], argv);
            if (res == 0) {
                write_(STDOUT_FILENO, curStr, lenw);
            }
        }
    }
    if (len < 0) {
        write_(STDERR_FILENO, "read error", 5);
    }
    if (lenBuf > 0) {
        (*((char *)buf + lenBuf)) = EOF;
        lenBuf++;
        ssize_t lenw;
        while ((lenw = getStr(curStr, buf)) > 0) {
            int res = spawn(argv[0], argv);
            if (res == 0) {
                write_(STDOUT_FILENO, curStr, lenw);
            }
        }
    }
    free(buf);
    free(curStr);
    return 0;
}


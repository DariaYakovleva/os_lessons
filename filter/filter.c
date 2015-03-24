#include "../lib/helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

size_t const MAX_LEN = 1 << 17;
ssize_t lenBuf = 0;

ssize_t getStr(char* ans, char* w) {
    int lenw = 0;
    while (lenw < lenBuf && (*(w + lenw)) != '\n') {
        (*(ans + lenw)) = (*(w + lenw));
        lenw++;
    }
    (*(ans + lenw)) = '\0';
    if (lenw == lenBuf) {
        return 0;
    }
    int pos = 0;
    lenw++;
    while (pos + lenw < lenBuf) {
        (*(w + pos)) = (*(w + lenw + pos));
        pos++;
    }
    lenBuf -= lenw;
    return lenw - 1;
}

int main(int argc, char *argv[]) {
//    char* args[] = {"ls", "/bin", NULL};
//    int res = spawn("ls", args);
//    printf("res=%d\n", res);
    void *buf = malloc(MAX_LEN);
    void *curStr = malloc(MAX_LEN);
    ssize_t len;
    while ((len = read_until(STDIN_FILENO, buf + lenBuf, MAX_LEN, '\n')) > 0) {
        lenBuf += len;
        ssize_t lenw;
        while ((lenw = getStr((char*)curStr, (char*)buf)) > 0) {
//            printf("res=%s\n", (char*)curStr);
            argv[argc] = curStr;
            int res = spawn(argv[1], argv + 2);
            if (res == 0) {
                write_(STDOUT_FILENO, curStr, lenw);
            }
        }
    }
    if (len < 0) {
        write_(STDERR_FILENO, "read error", 5);
    }
    if (lenBuf > 0) {
        (*((char *)buf + lenBuf)) = '\n';
        lenBuf++;
        ssize_t lenw;
        while ((lenw = getStr((char*)curStr, (char*)buf)) > 0) {
            int res = spawn(argv[0], argv);
            argv[argc] = curStr;
            if (res == 0) {
                write_(STDOUT_FILENO, curStr, lenw);
            }
        }
    }
    free(buf);
    free(curStr);
    return 0;
}


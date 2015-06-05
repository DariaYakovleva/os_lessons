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
    if (lenw == lenBuf) {
        return 0;
    }
    (*(ans + lenw)) = '\0';
    lenw++;
    int pos = 0;
    while (pos + lenw < lenBuf) {
        (*(w + pos)) = (*(w + lenw + pos));
        pos++;
    }
    lenBuf -= lenw;
    return lenw;
}

int main(int argc, char *argv[]) {
//    char* args[] = {"ls", "/bin", NULL};
//    int res = spawn("ls", args);
//    printf("res=%d\n", res);
    void *buf = malloc(MAX_LEN);
    void *curStr = malloc(MAX_LEN);
    ssize_t len;
    char* args[argc];
    int i = 0;
    while (i < argc - 1) {
        args[i] = argv[i + 1];
        i++;
    }
    while ((len = read_until(STDIN_FILENO, buf + lenBuf, MAX_LEN, '\n')) > 0) {
        lenBuf += len;
        ssize_t lenw;
        while ((lenw = getStr((char*)curStr, (char*)buf)) > 0) {
            args[argc - 1] = (char*)curStr;
            args[argc] = NULL;
            int res = spawn(argv[1], args);
//            printf("!res=%d %s!\n", res, args[argc - 1]);
            if (res == 0) {
		*((char*)curStr + lenw) = '\n';
		lenw++;
                write_(STDOUT_FILENO, curStr, lenw);
            }
        }
    }
    if (len < 0) {
        write_(STDERR_FILENO, "read error", 10);
    }
    if (lenBuf > 0) {
        (*((char *)buf + lenBuf)) = '\n';
        lenBuf++;
        ssize_t lenw;
        while ((lenw = getStr((char*)curStr, (char*)buf)) > 0) {
            args[argc - 1] = (char*)curStr;
            args[argc] = NULL;
            int res = spawn(argv[1], args);
            if (res == 0) {
		*((char*)curStr + lenw) = '\n';
		lenw++;
                write_(STDOUT_FILENO, curStr, lenw);
            }
        }
    }
    free(buf);
    free(curStr);
    return 0;
}


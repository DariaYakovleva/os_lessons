#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

int pids[100];
int cpid = 0;
int ret = 0;

void stop_work() {
	int j;
	for (j = 0; j < cpid; j++) {
		kill(pids[j], SIGKILL);
		waitpid(pids[j], NULL, 0);
	}
}

void shandler() {
//	fprintf(stderr, "DDDD\n");
	stop_work();
}


ssize_t read_ (int fd, void *buffer, size_t count) {
    ssize_t curS = 0;
    while ((size_t)curS < count) {
        ssize_t cur = read(fd, buffer + curS, count - curS);
        if (cur == 0) {
            return curS;
        }
        if (cur == -1) {
            return cur;
        }
        curS += cur;
    }
    return curS;
}

ssize_t write_ (int fd, const void *buffer, size_t count) {
    ssize_t curS = 0;
    while ((size_t) curS < count) {
        ssize_t cur = write(fd, buffer + curS, count - curS);
        if (cur == -1) {
            return curS;
        }
        curS += cur;
    }
    return curS;
}

ssize_t read_until(int fd, void *buffer, size_t count, char delimiter) {
    ssize_t curS = 0;
    while ((size_t)curS < count) {
        ssize_t cur = read(fd, buffer + curS, count - curS);
        if (cur == 0) {
            return curS;
        }
        if (cur == -1) {
            return cur;
        }
        int i = curS;
        curS += cur;
        while (i < curS) {
            if ((*((char *)buffer + i)) == delimiter) {
                return curS;
            }
            i++;
        }
    }
    return curS;	
}

int spawn(const char * file, char * const argv[]) {
    pid_t pid = fork();
    if (pid == -1) {
        printf("fail\n");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        pid_t w = waitpid(pid, &status, 0);
        if (w == -1) {
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else {
            exit(EXIT_FAILURE);
        }
    } else {
        if (execvp(file, argv) != -1) {
            exit(0);
        }
        exit(EXIT_FAILURE);
    }
}

struct execargs_t *createExec() {
	struct execargs_t* newExec = (struct execargs_t*)malloc(sizeof(struct execargs_t));
	return newExec;
}

int exec(struct execargs_t* args) {
//    return spawn(args->file, args->argv);
	execvp(args->file, args->argv);
}


int runpiped(struct execargs_t **programs, size_t n) {
	struct sigaction act2;
	memset(&act2, 0, sizeof(act2));
	act2.sa_handler = shandler;
	sigaction(SIGINT, &act2, 0);
	int next_input = STDIN_FILENO;
	int pipes[100];
	int cpip = 0;
	cpid = 0;
	size_t i;
	for (i = 0; i < n; i++) {
		int pipefd[2];
		if (pipe(pipefd) == -1) {	
			fprintf(stderr, "pipe failed\n");
			stop_work();
			return -1;
		}
		pipes[cpip] = pipefd[0];
		cpip++;
		pipes[cpip] = pipefd[1];
		cpip++;
    	        pid_t pid = fork();
		if (pid == -1) {
			fprintf(stderr, "can't create child\n");
			stop_work();
			return -1;
		}
		if (pid == 0) {	//child
//			fprintf(stderr, "%d: to = %d\n", (int)i, pipefd[1]);
			if (i < n - 1) {
				if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
					fprintf(stderr, "dup2 output failed\n");
					exit(EXIT_FAILURE);
				}
			}
			if (dup2(next_input, STDIN_FILENO) == -1) {
				fprintf(stderr, "dup2 input failed\n");
				exit(EXIT_FAILURE);
			}
			int j;
			for (j = 0; j < cpip; j++) {
				if (pipes[j] != next_input && pipes[j] != pipefd[1]) {
					close(pipes[j]);
				}
			}
			exec(*(programs + i));
	    } else {	//parent
			pids[cpid] = pid;
			cpid++;
			next_input = pipefd[0];
//			fprintf(stderr, "read = %d, write %d\n", pipefd[0], pipefd[1]);
		}
	}
	int j;
	for (j = 0; j < cpip; j++) {
		close(pipes[j]);
//		fprintf(stderr, "pipe close %d\n", pipes[j]);
	}
	int bad = 0;
	for (j = 0; j < cpid; j++) {
		int status;
		pid_t w = waitpid(pids[j], &status, 0);
//		fprintf(stderr, "pid end %d w = %d\n", pids[j], w);
		if (w == -1) {
			fprintf(stderr, "exit fail from process %d\n", pids[j]);
			bad = -1;
		}
		if (!WIFEXITED(status)) {
			fprintf(stderr, "exit fail from process %d\n", pids[j]);
			bad = -1;
		}
	}
    return bad;
}




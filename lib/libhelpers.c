#include "libhelpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>

struct execargs_t *createExec() {
	struct execargs_t* newExec = (struct execargs_t*)malloc(sizeof(struct execargs_t));
	return newExec;
}

int exec(struct execargs_t* args) {
	printf("EXEC file = %s\n", args->file);
    return spawn(args->file, args->argv);
/*    pid_t pid = fork();
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
*/
}

int exec_program(int input, struct execargs_t *program, int output) {
	int pipefd[2];
	printf("input child = %d\n", input);
	int in = dup2(STDIN_FILENO, input);
	if (in == -1) {
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1) {	
		perror("pipe failed");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	int out = STDOUT_FILENO;
	if (output) out = dup2(STDOUT_FILENO, pipefd[1]);
	if (out == -1) {
		perror("dup2 failed");
		exit(EXIT_FAILURE);
	}
	exec(program);
	printf("child in = %d, out = %d \n", input, out);
	return pipefd[0];
}

int runpiped(struct execargs_t **programs, size_t n) {
	printf("RUN %d\n", (int)n);
	printf("programs[0] = %s, argv = %s\n", (*programs)->file, (*programs)->argv[0]);
	int current_input = STDIN_FILENO;
	size_t i;
	for (i = 0; i < n; i++) {
	    pid_t pid = fork();
		printf("pid = %d\n", (int)pid); 
		if (pid == -1) {
			printf("can't create child\n");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {	//child
			int output = 1;
			if (i == n - 1) output = 0;
			current_input = exec_program(current_input, *(programs + i), output);
//			printf("!! = %d\n", current_input);
//			exit(EXIT_SUCCESS);
	    } else {	//parent
			printf("parent %d \n", (int)i);
			int status;
			pid_t w = waitpid(pid, &status, 0);
			printf("wait ok\n");
	        if (w == -1) {
				exit(EXIT_FAILURE);
			}
			if (WIFEXITED(status)) {
				exit(EXIT_SUCCESS);
	        } else {
				exit(EXIT_FAILURE);
	        }
		}
	}
	exit(EXIT_SUCCESS);
    return 0;
}



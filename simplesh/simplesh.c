//#include "../lib/helpers.h"
#include "../lib/libhelpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

size_t const MAX_LEN = (1 << 12) + 1;
ssize_t lenBuf = 0;

struct execargs_t* add_program(char* str) {
	struct execargs_t* newExec = (struct execargs_t*)malloc(sizeof(struct execargs_t));
//	newExec->argv[] = (char*)malloc(MAX_LEN);
	int cnt = 0;
	ssize_t pos = 0;
	while (*(str + pos) != 0) {
		while (*(str + pos) == ' ' && *(str + pos) != 0) pos++;
		if (*(str + pos) == 0) break;
		char* param = malloc(MAX_LEN);
		char* curp = param;
		while (*(str + pos) != ' ' && *(str + pos) != 0) {
			*curp = *(str + pos);
			curp++;
			pos++;
		}
		*curp = 0;
		newExec->argv[cnt] = param;
		printf("subprog = !%s!\n", newExec->argv[cnt]);
		printf("0 = !%s!\n", newExec->argv[0]);
		cnt++;
	}
	newExec->argv[cnt] = NULL;
	newExec->file = newExec->argv[0];
	return newExec;
}

int cntProg = 0;
struct execargs_t* programs[10];
struct execargs_t** split_programs(char* str, ssize_t len) {
	char* cur_prog = malloc(MAX_LEN);
	cntProg = 0;
	ssize_t clen = 0;
	while (*(str + clen) != '\n') {
		char* curs = cur_prog;
		while (*(str + clen) != '|' && *(str + clen) != '\n') {
			*curs = *(str + clen);
			curs++;
			clen++;
		}
		*curs = 0;
		printf("prog = !%s!\n", cur_prog);
		struct execargs_t* progr = add_program(cur_prog);
		programs[cntProg] = progr;
		cntProg++;
		if (*(str + clen) != '\n') clen++;
	}
	programs[cntProg] = NULL;
	printf("cntProg = %d, file = %s, arg[1] = %s\n", cntProg, programs[0]->file, programs[0]->argv[1]);
	return programs;
}

int main(int argc, char *argv[]) {
//    char* args[] = {"ls", "/bin", NULL};
//    int res = spawn("ls", args);
//    printf("res=%d\n", res);
    printf("START\n");
    
	void *buf = malloc(MAX_LEN);
	ssize_t len;
	while (1) {
		write_(STDOUT_FILENO, "$", 1);
		len = read_until(STDIN_FILENO, buf, MAX_LEN, '\n');
		printf("BUF = !%s!\n", (char*)buf);
		int res = runpiped(split_programs((char*)buf, len), 1);//cntProg);
		printf("RES = %d\n", res);
    }
    
	free(buf);
    return 0;
}


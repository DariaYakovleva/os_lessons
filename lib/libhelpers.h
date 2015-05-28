#ifndef LIBHELPERS_H
#define LIBHELPERS_H

#include "helpers.h"
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

struct execargs_t {
    char* file;
    char* argv[10];		
};

struct execargs_t *createExec();

int exec(struct execargs_t *args);

int runpiped(struct execargs_t **programs, size_t n);



#endif // LIBHELPERS_H

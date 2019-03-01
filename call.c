#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



static int exists(char * file)
{
    return access( file, F_OK ) != -1;
}

static void compile(char * fn)
{
    char * template;
    size_t length;
    char * command;

    template = "gcc -Wall -ansi -pedantic %s.c -o %s";
    length = strlen(template) + strlen(fn) * 2;
    command = malloc(sizeof(char) * (length + 1));

    sprintf(command, template, fn, fn);

    system(command);

    free(command);
}

static void execute(char * fn, char * args)
{
    char * template;
    size_t length;
    char * command;

    template = "./%s %s";
    length = strlen(template) + strlen(fn) + strlen(args);
    command = malloc(sizeof(char) * (length + 1));

    sprintf(command, template, fn, args);

    system(command);

    free(command);
}

void call(char * fn, char * args)
{
    if ( ! exists(fn) ) {
        compile(fn);
    }

    execute(fn, args);
}

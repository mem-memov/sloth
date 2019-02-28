#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void call(char * fn)
{
    char * template;
    size_t length;
    char * command;

    printf("I am calling %s!\n", fn);

    template = "gcc -Wall -ansi -pedantic %s.c -o %s && ./%s";
    length = strlen(template) + strlen(fn) * 3;
    command = malloc(sizeof(char) * (length + 1));

    sprintf(command, template, fn, fn, fn);

    system(command);

    free(command);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void call(char * fn, char * args)
{
    char * template;
    size_t length;
    char * command;

    printf("I am calling %s!\n", fn);

    template = "gcc -Wall -ansi -pedantic %s.c -o %s && ./%s %s";
    length = strlen(template) + strlen(fn) * 3 + strlen(args);
    command = malloc(sizeof(char) * (length + 1));

    sprintf(command, template, fn, fn, fn, args);

    system(command);

    free(command);

}

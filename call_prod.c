#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void call(char * fn, char * args)
{
    char * template;
    size_t length;
    char * command;
    int fileExists;

    printf("I am calling %s!\n", fn);

    template = "./%s %s";
    length = strlen(template) + strlen(fn) + strlen(args);
    command = malloc(sizeof(char) * (length + 1));

    sprintf(command, template, fn, args);

    system(command);

    free(command);
}

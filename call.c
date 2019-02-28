#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void call(char * fn, char * args)
{
    char * template;
    size_t length;
    char * command;
    int fileExists;

    printf("I am calling %s!\n", fn);

    fileExists = access( fn, F_OK ) != -1;

    if( fileExists ) {

        template = "./%s %s";
        length = strlen(template) + strlen(fn) + strlen(args);
        command = malloc(sizeof(char) * (length + 1));

        sprintf(command, template, fn, args);

        system(command);

        free(command);

    } else {

        template = "gcc -Wall -ansi -pedantic %s.c -o %s && ./%s %s";
        length = strlen(template) + strlen(fn) * 3 + strlen(args);
        command = malloc(sizeof(char) * (length + 1));

        sprintf(command, template, fn, fn, fn, args);

        system(command);

        free(command);

    }
}

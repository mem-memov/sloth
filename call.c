#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static int lock(char * fn)
{
    char * lockExtension;
    char * lockFile;
    size_t length;
    FILE * filePointer;

    lockExtension = ".lock";
    length = strlen(fn) + strlen(lockExtension);
    lockFile = malloc(sizeof(char) * (length + 1));

    filePointer = fopen(lockFile, "w");
    
    free(lockFile);

    if ( ! filePointer ) {
        return 0;
    }

    fclose(filePointer);

    return 1;
}

void call(char * fn, char * args)
{
    int hasManagedLocking;

    hasManagedLocking = lock(fn);

    if ( hasManagedLocking ) {
        compile(fn);
    }

    execute(fn, args);
}

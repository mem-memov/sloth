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
    char * template;
    char * lockFile;
    size_t length;
    FILE * filePointer;
    char content;

    printf("locking %s\n", fn);

    template = "%s.lock";
    length = strlen(template) + strlen(fn);
    lockFile = malloc(sizeof(char) * (length + 1));

    sprintf(lockFile, template, fn);

    printf("locking %s\n", lockFile);

    filePointer = fopen(lockFile, "w+");
    
    free(lockFile);

    if ( ! filePointer ) {
        return 0;
    }

    content = fgetc(filePointer);

    if (content == 's') {
        return 0;
    }

    fputc('s', filePointer);
    
    fclose(filePointer);

    return 1;
}

void call(char * fn, char * args)
{
    int hasManagedLocking;

    printf("calling %s\n", fn);

    hasManagedLocking = lock(fn);

    printf("hasManagedLocking %d\n", hasManagedLocking);

    if ( hasManagedLocking ) {
        compile(fn);
    }

    execute(fn, args);
}

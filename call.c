#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>

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

static int exists(char * fn)
{
    char * template;
    char * lockFile;
    size_t length;

    template = "%s.lock";
    length = strlen(template) + strlen(fn);
    lockFile = malloc(sizeof(char) * (length + 1));

    sprintf(lockFile, template, fn);

    printf("locking %s\n", lockFile);

    filePointer = fopen(lockFile, "r");

    if ( filePointer ) {
        return 1;
    }

    return 0;
}

static int lock(char * fn)
{
    char * template;
    char * lockFile;
    size_t length;
    FILE * filePointer;
    char content;

    template = "%s.lock";
    length = strlen(template) + strlen(fn);
    lockFile = malloc(sizeof(char) * (length + 1));

    sprintf(lockFile, template, fn);

    printf("locking %s\n", lockFile);

    filePointer = fopen(lockFile, "r");

    if ( filePointer ) {
        return 0;
    }

    filePointer = fopen(lockFile, "w+");
    
    free(lockFile);

    if ( ! filePointer ) {
        return 0;
    }

    content = fgetc(filePointer);

    printf("reading %d\n", content);

    if (content == 33) {
        return 0;
    }

    fputc(33, filePointer);
    
    fclose(filePointer);

    return 1;
}

void call(char * fn, char * args)
{
    int lockExists;
    int hasManagedLocking;

    printf("calling %s\n", fn);

    lockExists = exists(fn);

    if () {

    }

    hasManagedLocking = lock(fn);

    printf("hasManagedLocking %s %d\n", fn, hasManagedLocking);

    if ( hasManagedLocking ) {
        compile(fn);
    }

    execute(fn, args);
}

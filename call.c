#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

static void nameLockFile(char * lockFile, char * fn)
{
    char * template;
    char * lockFile;
    size_t length;

    template = "%s.lock";
    length = strlen(template) + strlen(fn);
    lockFile = malloc(sizeof(char) * (length + 1));

    sprintf(lockFile, template, fn);
}

static int checkLockFileExists(char * lockFile)
{
    FILE * filePointer;
    int lockFileExists;

    filePointer = fopen(lockFile, "r");

    if ( filePointer ) {
        lockFileExists = 1;
    } else {
        lockFileExists = 0;
    }

    fclose(filePointer);

    return lockFileExists;
}

static int checkCompilationFinished(char * lockFile)
{
    FILE * filePointer;
    int compilationFinished;
    char content[255];

    filePointer = fopen(lockFile, "r");

    fgets(content, 255, (FILE*)filePointer);

    if ( strcmp(content, "compiled") == 0) {
        compilationFinished = 1;
    } else {
        compilationFinished = 0;
    }

    fclose(filePointer);

    return compilationFinished;
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

static void executeAfterCompilation(char * lockFile, char * fn, char * args)
{
    int compilationFinished;

    do {
        compilationFinished = checkCompilationFinished(lockFile);
        if ( compilationFinished ) {
            execute(fn, args);
        }
    } while ( ! compilationFinished );
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

static int lock(char * lockFile)
{
    FILE * filePointer;
    char content;

    printf("locking %s\n", lockFile);

    filePointer = fopen(lockFile, "w+");

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

static void finishCompilation(char * lockFile)
{

}

void call(char * fn, char * args)
{
    char * lockFile;
    int lockFileExists;
    int hasManagedLocking;

    printf("calling %s\n", fn);

    nameLockFile(lockFile, fn);

    lockFileExists = checkLockFileExists(lockFile);

    if ( lockFileExists ) {
        executeAfterCompilation(lockFile, fn, args);
    } else {
        hasManagedLocking = lock(lockFile);
        if (hasManagedLocking) {
            compile(fn);
            finishCompilation(lockFile);
        } else {
            executeAfterCompilation(lockFile, fn, args);
        }
    }

    free(lockFile);
}

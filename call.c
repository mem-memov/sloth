#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

static char * nameLockFile(char * fn)
{
    char * template;
    size_t length;
    char * lockFile;

    template = "%s.lock";
    length = strlen(template) + strlen(fn);
    lockFile = malloc(sizeof(char) * (length + 1));

    sprintf(lockFile, template, fn);

    return lockFile;
}

static int checkLockFileExists(char * lockFile)
{
    FILE * filePointer;

    filePointer = fopen(lockFile, "r");

    if ( filePointer ) {
        fclose(filePointer);
        return 1;
    } else {
        return 0;
    }
}

static int checkCompilationFinished(char * lockFile)
{
    FILE * filePointer;
    int compilationFinished;
    char content[255];

    filePointer = fopen(lockFile, "r");

    fgets(content, 255, filePointer);

    printf("lockFile %s\n", lockFile);
    printf("content %s\n", content);

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

        printf("compilationFinished %d\n", compilationFinished);

        if ( compilationFinished ) {
            execute(fn, args);
        }
    } while ( ! compilationFinished );
}

static int lock(char * lockFile)
{
    FILE * filePointer;
    char processIdentifier[255];
    char content[255];
    int hasManagedLocking;

    printf("locking %s\n", lockFile);

    filePointer = fopen(lockFile, "w");

    if ( ! filePointer ) {

        hasManagedLocking = 0;

    } else {

        printf("processIdentifier %d\n", getpid());

        sprintf(processIdentifier, "%d", getpid()); 

        fputs(processIdentifier, filePointer);
        fclose(filePointer);

        do {
            filePointer = fopen(lockFile, "r");
        } while( ! filePointer );
        
        fgets(content, 255, filePointer);
        fclose(filePointer);

        printf("content %s\n", content);

        if ( strcmp(content, processIdentifier) == 0) {
            hasManagedLocking = 1;
        } else {
            hasManagedLocking = 0;
        }
    }

    return hasManagedLocking;
}

static void finishCompilation(char * lockFile)
{
    FILE * filePointer;
    filePointer = fopen(lockFile, "w");
    fputs("compiled", filePointer);
}

void call(char * fn, char * args)
{
    char * lockFile;
    int lockFileExists;
    int hasManagedLocking;

    printf("calling %s\n", fn);

    lockFile = nameLockFile(fn);

    lockFileExists = checkLockFileExists(lockFile);

    printf("nameLockFile %s\n", lockFile);
    printf("lockFileExists %d\n", lockFileExists);

    if ( lockFileExists ) {
        executeAfterCompilation(lockFile, fn, args);
    } else {
        hasManagedLocking = lock(lockFile);
        printf("hasManagedLocking %d\n", hasManagedLocking);
        if (hasManagedLocking) {
            compile(fn);
            finishCompilation(lockFile);
        } else {
            executeAfterCompilation(lockFile, fn, args);
        }
    }

    free(lockFile);
}

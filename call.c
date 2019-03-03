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

    if ( filePointer ) {

        fgets(content, 255, filePointer);

        if ( strcmp(content, "compiled") == 0) {
            compilationFinished = 1;
        } else {
            compilationFinished = 0;
        }

        fclose(filePointer);

    } else {
        
    }

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

static void execute(char * fn, int argc, char * argv[])
{
    pid_t pid;
    char ** args;

    args = malloc(sizeof(char *) * (argc + 1));

    memcpy(args, argv, sizeof(char *) * argc);

    args[argc] = NULL;

    pid = fork();

    if (pid == 0) {
        execv(fn, args);
    }

    free(args);
}

static void executeAfterCompilation(char * lockFile, char * fn, int argc, char * argv[])
{
    int compilationFinished;

    do {
        compilationFinished = checkCompilationFinished(lockFile);

        if ( compilationFinished ) {
            execute(fn, argc, argv);
        }
    } while ( ! compilationFinished );
}

static int lock(char * lockFile)
{
    FILE * filePointer;
    char processIdentifier[255];
    char content[255];
    int hasManagedLocking;

    filePointer = fopen(lockFile, "w");

    if ( ! filePointer ) {

        hasManagedLocking = 0;

    } else {

        sprintf(processIdentifier, "%d", getpid()); 

        fputs(processIdentifier, filePointer);
        fclose(filePointer);

        do {
            filePointer = fopen(lockFile, "r");
        } while( ! filePointer );
        
        fgets(content, 255, filePointer);
        fclose(filePointer);

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
    fclose(filePointer);
}

void call(char * fn, int argc, char * argv[])
{
    char * lockFile;
    int lockFileExists;
    int hasManagedLocking;

    lockFile = nameLockFile(fn);

    lockFileExists = checkLockFileExists(lockFile);

    if ( lockFileExists ) {
        executeAfterCompilation(lockFile, fn, argc, argv);
    } else {
        hasManagedLocking = lock(lockFile);
        if (hasManagedLocking) {
            compile(fn);
            finishCompilation(lockFile);
            execute(fn, argc, argv);
        } else {
            executeAfterCompilation(lockFile, fn, argc, argv);
        }
    }

    free(lockFile);
}

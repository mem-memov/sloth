#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
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

    printf("compiling %s\n", fn);

    template = "gcc -Wall %s.c -o %s";
    length = strlen(template) + strlen(fn) * 2;
    command = malloc(sizeof(char) * (length + 1));

    sprintf(command, template, fn, fn);

    system(command);

    free(command);
}

static void execute(char * fn[])
{
    pid_t childProcessIdentifier;

    printf("executing %s\n", fn[0]);

    childProcessIdentifier = fork();

    if ( childProcessIdentifier == -1 ) {
        printf("Faild forking %s", fn[0]);
    }

    if (childProcessIdentifier == 0) {
        execv(fn[0], fn);
    }

    if (childProcessIdentifier > 0) {
        waitpid(childProcessIdentifier, 0, 0);
    }
}

static void executeAfterCompilation(char * lockFile, char * fn[])
{
    int compilationFinished;

    do {
        compilationFinished = checkCompilationFinished(lockFile);

        if ( compilationFinished ) {
            execute(fn);
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

void call(char * fn[])
{
    char * lockFile;
    int lockFileExists;
    int hasManagedLocking;

    printf("Call to %s\n", fn[0]);

    lockFile = nameLockFile(fn[0]);

    lockFileExists = checkLockFileExists(lockFile);

    if ( lockFileExists ) {
        executeAfterCompilation(lockFile, fn);
    } else {
        hasManagedLocking = lock(lockFile);
        if (hasManagedLocking) {
            compile(fn[0]);
            finishCompilation(lockFile);
            execute(fn);
        } else {
            executeAfterCompilation(lockFile, fn);
        }
    }

    free(lockFile);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

void call(char * fn)
{
    printf("I am calling %s!\n", fn);

    char * template = "gcc %s.c -o %s && ./%s";
    size_t length = strlen(template) + strlen(fn) * 2;
    char * command = malloc(sizeof(char) * (length + 1));
    sprintf(command, template, fn, fn, fn);

    system(command);

    free(command);
}

int main()
{
    call("app");
    return 0;
}

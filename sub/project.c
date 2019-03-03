#include <stdio.h>
#include "../call.c"

int main(int argc, char *argv[])
{
    printf("I am projecting from sub space!\n");
    static char * fn[] = {"sub/deep/project", NULL};
    call(fn);
    return 0;
}

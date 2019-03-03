#include <stdio.h>
#include "../call.c"

char deepProject[] = "sub/deep/project";

int main(int argc, char *argv[])
{
    printf("I am projecting from sub space!\n");

    static char * fn[] = {deepProject, NULL};
    call(fn);

    return 0;
}

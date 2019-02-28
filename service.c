#include <stdio.h>
#include "call.c"

int main(int argc, char *argv[])
{
    printf("I am serving %s!\n", argv[1]);
    call("sub/project", "");
    return 0;
}

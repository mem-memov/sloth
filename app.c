#include <stdio.h>
#include "call.c"

int main(int argc, char *argv[])
{
    int i;
    i = 10;

    while (i > 0) {
        i = i-1;

        printf("I am applying!\n");
        call("service", "user");
        call("service", "myself");
    }

    return 0;
}

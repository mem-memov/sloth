#include "call.c"

int main(int argc, char *argv[])
{
    static char * fn[] = {"sub/project", NULL};
    call(fn);

    return 0;
}

#include "call.c"

int main(int argc, char *argv[])
{
    call("sub/project", argc-1, argv + 1);
    return 0;
}

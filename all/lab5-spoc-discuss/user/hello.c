#include <stdio.h>
#include <ulib.h>

int
main(void) {
    cprintf("Hello world!!.\n");
    cprintf("I am user process A, my pid is %d.\n", getpid());
    cprintf("hello pass.\n");
    return 0;
}


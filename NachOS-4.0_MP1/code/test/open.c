#include "syscall.h"

int main() {
    char file[1000];
    int result;

    result = Open("num_100.txt");
    PrintInt(result);
    Halt();
    /* not reached */
}


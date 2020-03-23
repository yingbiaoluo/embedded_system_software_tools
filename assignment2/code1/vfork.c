#include <stdio.h>
#include <unistd.h>

void stack1() {
    vfork();
}

void stack2() {
    _exit(0);
}

int main() {
    stack1();
    printf("%d goes 1\n", getpid());
    stack2();
    printf("%d goes 2\n", getpid());
    return 0;
}

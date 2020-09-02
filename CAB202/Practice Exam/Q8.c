#include <stdio.h>

int main() {
    int x = (1 << 7) | (1 << 5) | (1 << 4) | (1 << 2) | (1 << 1);
    printf("x is %d \n", x);
    return 0;
}
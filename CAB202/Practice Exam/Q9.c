#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void process(uint8_t b) {
    printf("%d ->", b);
    for (int i = 7; i >= 0; i--) {
        if (!((b >> i) & 1)) printf(" %d", i);
    }
    printf("\n");
}

int main(void) {
    int x;
    while (1 == scanf("%d", &x)) {
        if (x & 255) process(x & 255);
    }
    return 0;
}
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t reg = 0;

uint8_t process(void) {
#define B (3)
    static uint8_t p = 0;
    uint8_t q = reg & (1 << B);
    uint8_t r = p && (!q);
    p = q;
    return r;
}

int main(void) {
    int x;
    int d = 0;
    while (1 == scanf("%d", &x)) {
        reg = x & 255;
        uint8_t b = process();
        printf("%s%d", (d ? " " : ""), b);
        d = 1;
    }
    printf("\n");
    return 0;
}
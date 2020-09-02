#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void process(char ch);
int n = 0;

void process(char ch) {
    static int p = 0;
    int q = isalnum(ch);
    if ((!p) && q) n++;
    p = q;
}

int main() {
    char x;
    while (1 == scanf("%c", &x)) {
        process(x);
    }
    printf("n = %d\n", n);
    return 0;
}
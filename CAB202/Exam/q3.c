#include <stdio.h>

int main(int argc, char* argv[]) {
    int i = 0;
    while (i <= argc) {
        printf("Argument %d is '%s'\n", i, argv[i]);
    }
    return 0;
}
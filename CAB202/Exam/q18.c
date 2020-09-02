#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VAL 30
#define MIN_VAL -10

// Declare and initialise any additional global variables here, as required.
int term_count[41] = {0};
int lowest_count = 41;
int lower[41] = {0};
int second_lowest_count = 41;
int second[41] = {0};
int total = 0;
int m = 0;
int n = 0;

void process(int x) {
    // Process one value here.
    total++;
    if (total < INT_MAX) {
        if (!(x < -10 || x > 30)) {
            term_count[x + 10]++;
        }
    }
}

void post_process() {
    // Compute derived results here.
    lowest_count = term_count[0];
    second_lowest_count = term_count[0];

    for (int i = 0; i <= 40; i++) {
        if (term_count[i] < lowest_count) {
            second_lowest_count = lowest_count;
            lowest_count = term_count[i];
        }
    }

    for (int j = 0; j <= 40; j++) {
        if (term_count[j] == lowest_count) {
            lower[m++] = j - 10;
        }
        if (term_count[j] == second_lowest_count) {
            second[n++] = j - 10;
        }
    }
}

void print() {
    // Print results here
    printf("The lowest count is %d, at", lowest_count);
    for (int i = 0; i < m - 1; i++) {
        printf(" %d,", lower[i]);
    }
    printf(" %d\n", lower[m - 1]);
    printf("The second-lowest count is %d, at", second_lowest_count);
    for (int i = 0; i < n - 1; i++) {
        printf(" %d,", second[i]);
    }
    printf(" %d\n", second[n - 1]);
    printf("The complete histogram is:\n");
    for (int i = 0; i < 41; i++) {
        printf("%d -> %d\n", i - 10, term_count[i]);
    }
}

int main(void) {
    int x;

    while (1 == scanf("%d", &x)) {
        process(x);
    }
    post_process();
    print();
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

#define M (100)
void print(double x[], int n);
void process(double x[], int n, int w, double y[]);

int main(void) {
    double x, a[M], b[M];
    int n = 0;
    int w;
    scanf("%d", &w);
    while (1 == scanf("%lf", &x)) {
        if (n < M) a[n++] = x;
    }
    process(a, n, w, b);
    print(b, n);
    return 0;
}

void print(double x[], int n) {
    for (int i = 0; i < n; i++) printf("%3.2f ", x[i]);
    printf("\n");
}

void process(double x[], int n, int w, double y[]) {
    for (int i = 0; i < n; i++) {
        y[i] = 0;
        int m = 0;
        for (int j = -w / 2; j < w - w / 2; j++) {
            int k = i - j;
            if (k < 0 || k >= n) continue;
            y[i] += x[k];
            m++;
        }
        y[i] /= m;
    }
}
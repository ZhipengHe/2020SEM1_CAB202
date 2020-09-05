#include <stdio.h>

//  Declare Global variables here.
double item[34];
int item_count;
double item_mean;
double item_min;
double item_max;
double item_sum;

void arraystats() {
    //  Insert your solution here.
    item_count = 0;
    while (item_count < 34 && scanf("%lf", &item[item_count]) == 1) {
        item_count++;
    }

    item_sum = 0;
    item_mean = 0;

    if (item_count == 0) {
        item_max = 0;
        item_min = 0;
    } else {
        item_max = item[0];
        item_min = item[0];
    }

    for (int i = 0; i < item_count; i++) {
        item_sum += item[i];
        if (item_max < item[i]) {
            item_max = item[i];
        }
        if (item_min > item[i]) {
            item_min = item[i];
        }
    }
    if (item_count != 0) {
        item_mean = item_sum / item_count;
    }
}

#include <stdlib.h>
#include <time.h>

int main() {
    // AMS will do something a bit like this before it calls your solution.
    // Do not change any of this; instead, write your solution in such a way 
    //  that it works correctly even with this.
    srand( time( NULL ) );

    for ( int i = 0; i < 34; i++ ) {
        double randVal = ((double) rand()) / RAND_MAX; 
        item[i] = randVal;

        if (randVal != item[i]) {
            printf( "Incorrect numeric type appears to be in use.\n" );
            return 1;
        }
    }
    
    item_count = rand();
    item_mean = ((double) rand()) / RAND_MAX;
    item_min = ((double) rand()) / RAND_MAX;
    item_max = ((double) rand()) / RAND_MAX;

    // Call submitted code.
    arraystats();

    // Display contents of array item.
    for (int i = 0; i < item_count; i++) {
        printf("%f ", item[i]);
    }

    printf("\n");
    printf("Item count: %d\n", item_count);
    printf("Item mean : %f\n", item_mean);
    printf("Item min  : %f\n", item_min);
    printf("Item max  : %f\n", item_max);

    return 0;
}

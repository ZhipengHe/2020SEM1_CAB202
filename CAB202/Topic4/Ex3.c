#include <stdio.h>
#include <stdlib.h>

//  HERE: Declare an array called list with capacity to hold 33 double 
//  precision floating point values.
double list[33];
//  HERE: Declare an integer called list_size which will track the number of items
//  that have been appended to the list stored.
int list_size;
//  HERE: Declare a double precision floating point value called list_sum which will 
//  be used to remember the total sum of the items stored in the list.
double list_sum;

void arraysum() {
    //  Initially the list is empty, so assign 0 to list_size.
    list_size = 0;
    //  Append items to the list by reading from standard input until either
    //  list_size >= 33, or the value returned by scanf is not 1.
    while (list_size < 33 && scanf("%lf", &list[list_size]) == 1) {
        list_size++;
    }
    //  Assign 0 to list_sum.
    list_sum = 0;
    //  Traverse the items that have been appended to the list, and accumulate 
    //  the sum in list_sum.
    for (int i = 0; i < list_size; i++) {
        list_sum += list[i]; 
    }
}

#include <stdlib.h>
#include <time.h>

int main(void) {
    // AMS will do something a bit like this before it calls your solution.
    // Do not change any of this; instead, write your solution in such a way 
    //  that it works correct even with this.
    for ( int i = 0; i < 33; i++ ) {
        double randVal = ((double) rand()) / RAND_MAX;
        list[i] = randVal;

        if (randVal != list[i]) {
            printf("Incorrect numeric type appears to be in use.\n");
            return 1;
        }
    }

    list_size = rand();
    list_sum = rand();

    // Call submitted code.
    arraysum();

    // Display contents of array list.
    for (int i = 0; i < list_size; i++) {
        printf("%f ", list[i]);
    }

    printf("\n");
    printf("Item count: %d\n", list_size);
    printf("Item sum  : %f\n", list_sum);

    return 0;
}

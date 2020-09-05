#include <stdio.h>
#include <stdlib.h>

//  Declare an array called sample with capacity for 78 elements
//  of the default signed integer type.
//
//  Note well:
//  *   Make sure the array is declared as a global variable, that is, outside 
//      all the functions.
//  *   Do not use the "static" keyword.
//  *   Do not assume that any initial values you place in this, or any other 
//      global variable, will be as you imagine when the roll_dice function is 
//      called. The function may be called multiple times, in which case the 
//      values of any global variable will probably change after each run.

int sample[78];

void roll_dice( void ) {
    //  Seed the random number generator with the value 378534.
    srand(378534);
    //  Generate 78 random values between 1 and 4 inclusive. To do 
    //  this, use a counter-controlled loop which will visit every element of 
    //  sample. The counter should start at zero, and the loop should continue 
    //  while the counter is less than 78.
    for (int i = 0; i < 78; i++) {
            //  Generate a random value between 1 and 4 inclusive. Store this 
            //  value in the current element of sample - that is, the element 
            //  indexed by the loop counter.
        sample[i] = rand() % 4 + 1;
    //  End the loop.
    }
}

int main( void ) {
    //  Fill array with distinctive data pattern to help identify bugs. If you 
    //  see the number 13283842 in your output there must a bug in your code.
    for ( int i = 0; i < 78; i++ ) {
        sample[i] = 0xCAB202;
    }

    //  Call your code.
    roll_dice();

    //  Display contents of array sample.
    for ( int i = 0; i < 78; i++ ) {
        if ( i > 0 ) {
            printf( "," );
        }
        printf( "%d", sample[i] );
    }

    printf( "\n" );

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

//  Here: Declare a global array called term with capacity to hold 34 elements
//  of the default signed integer type.
int term[34];

void generate_terms( void ) {
    //  Here: Insert code to populate the array with a generalised Fibonacci 
    //  sequence here.
    scanf("%d", &term[0]);
    scanf(" %d", &term[1]);
    for (int i = 2; i < 34; i++) {
        term[i] = term[i-1] + term[i-2];
    }
}

int main(void) {
    //  Fill array with distinctive data pattern to help identify bugs. If you 
    //  see the number 13283842 in your output there must a bug in your code.
    for ( int i = 0; i < 34; i++ ) {
        term[i] = 0xCAB202;
    }

    //  Run your code.
    generate_terms();

    //  Display contents of array term.
    for (int i = 0; i < 34; i++) {
        printf("%d\n", term[i]);
    }

    return 0;
}

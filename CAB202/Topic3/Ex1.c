#include <stdio.h>

void list_integers( void ) {
    // Display the title message.
	printf("The integer sub-range from 29 to 307...\n");
    //  Enter a counter-controlled loop which will start at 29 and
    //  continues up to and including 307, advancing by increments of 1. 
	for (int i = 29; i <= 307; ++i) {
        //  Print the current value of the counter variable.
		printf("%d\n", i);
    // End the loop.
	}
}

int main( void ) {
	list_integers( );
	return 0;
}

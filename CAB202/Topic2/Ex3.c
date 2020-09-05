#include <stdio.h>

void is_divisible( void ) {
    //  Prompt the user to enter a pair of integers, separated by one or more spaces.
    //  Use the text:
    //  "Please enter integers X and Y, separated by one or more space characters:"
    //  displayed on a line by itself.
    printf("Please enter integers X and Y, separated by one or more space characters:\n");
    //  Declare a pair of integer variables, and scan their values from the 
    //  standard input stream.
    int first;
    int second;
    scanf(" %d", &first);
    scanf(" %d", &second);
    // If the first value (X) is divisible by the second (Y), emit the phrase:
    //  "X is divisible by Y."
    // Otherwise, emit the phrase:
    //  "X is not divisible by Y."
    // This text should appear on a line by itself.
    if (first % second == 0) {
        printf("X is divisible by Y.\n");
    } else {
        printf("X is not divisible by Y.\n");
    }
}


int main() {
	is_divisible();
	return 0;
}

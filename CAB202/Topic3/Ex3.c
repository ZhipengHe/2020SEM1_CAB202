#include <stdio.h>

void flag_multiples( void ) {
    //  Enter a counter controlled loop which starts at 96 and
    //  advances by increments of 1 up to and including 3295
        //  Send the current value of the counter to standard output as a 
        //  decimal formatted integer.
    for (int i = 96; i <= 3295; ++i) {
        //  If the current value of the counter is divisible by 3 then 
        //  highlight the current line by sending the pattern " <===" to 
        //  standard output.
        if (i % 3 != 0) {
            printf("%d\n", i);
        } else {
        //  Send a linefeed to standard output.
            printf("%d <===\n", i);
        }
    //  End loop.
    }
}

int main( void ) {
	flag_multiples( );
	return 0;
}

#include <stdio.h>

void count_down( void ) {
    //  Insert your solution here
    for (int i = 3953; i >= 7; i -= 5) {
		printf("%d\n", i);
    }
}

int main( void ) {
	count_down( );
	return 0;
}
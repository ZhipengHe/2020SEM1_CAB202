#include <stdio.h>
#include <string.h>


int main ( void ) {
	char me[] = "Larry";
	char you[] =  "Larry"; // "someone else";

	if ( me == you ) {
		printf( "That's a coincidence!\n" );
	}
	else {
		printf( "That's to be expected!\n" );
	}

	if ( strcmp(me, you) == 0 ) {
		printf( "That's a coincidence!\n" );
	}
	else {
		printf( "That's to be expected!\n" );
	}
}
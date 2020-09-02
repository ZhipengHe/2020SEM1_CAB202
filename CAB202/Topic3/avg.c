/**
 *	avg.c, by Lawrence Buckingham on 08 March 2020.
 *	(C) 2020 Queensland University of Technology.
 */
#include <stdio.h>
#include <ctype.h>

int main () {
	double x;
	double sum = 0;
	int n = 0;

	while(1) {
		// Try to read next numeric value in file.
		int scanf_res = scanf("%lf", &x);

		if ( scanf_res < 0 ) {
			// Out of data
			break;
		}
		else if ( scanf_res == 0 ) {
			// Non-numeric: throw away up to next space.
			char discardChar;
			
			do {
				// 	Try to read 1 character. If there's data this should 
				//	return 1.
				scanf_res = scanf( "%c", &discardChar );
			}
			while ( scanf_res == 1 && ! isspace(discardChar) );
		}
		else {
			sum += x;
			n ++;
		}
	}
	
	printf( "the average of %d entries is %f\n", n, sum / n );
	
	return 0;
} 

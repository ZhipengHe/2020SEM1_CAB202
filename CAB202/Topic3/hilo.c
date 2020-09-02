/**
 *	hilo.c, by Lawrence Buckingham on 08 March 2020.
 *	(C) 2020 Queensland University of Technology.
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main () {
	// Think of a number
	srand(time(NULL));
	int secret = 1 + rand() % 100;
	int guess;
	
	// Forever...
	while (1) {
		printf("I'm thinking of a number between 1 and 100. Can you guess what it is?\n");
		
		int conversions = scanf("%d", &guess);
		
		if ( conversions < 0 ) {
			// Out of data
			printf( "End of input reached, bye.\n" );
			break;
		}
		else if ( conversions == 0 ){
			// Data, but not numeric - throw away the rest of the line.
			printf( "You need to enter a whole number!\nTry again.\n" );
		
			char discardChar;
			
			do {
				conversions = scanf( "%c", &discardChar );
			}
			while ( conversions == 1 && discardChar != '\n' );
		} 
		else {
			// printf( "conversions = %d\n", conversions );
			// printf( "guess = %d\n", guess );
			
			if ( guess > secret ) {
				printf("Your guess is too high.\n");
			}
			else if ( guess < secret ) {
				printf("Your guess is too low.\n");
			}
			else {
				printf("You. Are. A. Genius.\nWell done for guessing the answer.\n");
				break;
			}
		}
	}
	
	printf( "Goodbye. Please play again some day.\n" );
	
	return 0;
} 

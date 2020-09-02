/**
 *	toggle_case.c, by Lawrence Buckingham on 08 March 2020.
 *	(C) 2020 Queensland University of Technology.
 */
#include <stdio.h>
#include <ctype.h>

int main () {
	char ch;

	while(1) {
		// Try to read next symbol in file.
		int scanf_res = scanf("%c", &ch);

		if ( scanf_res <= 0 ) {
			// Out of data, or for some other reason no char
			break;
		}
		else if ( isupper(ch) ) {
			printf("%c", tolower(ch));
		}
		else if ( islower(ch) ) {
			printf("%c", toupper(ch));
		}
		else {
			printf("%c", ch);
		}
	}
	
	return 0;
} 

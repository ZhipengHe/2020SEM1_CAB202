#include <stdio.h>
#include <string.h>

#define SIZE (256+1)
char input_buffer[SIZE];
int num_chars;

int main ( void ) {
	
	for (num_chars = 0; num_chars < SIZE-1; num_chars++) {
		int scanf_res = scanf("%c", &input_buffer[num_chars]);
		
		if ( scanf_res <= 0 ) {
			break;
		}
	}
	
	input_buffer[num_chars] = 0;
	
	printf("input_buffer = '%s'\n", input_buffer);
	
	for ( int i = 0; input_buffer[i] != 0; i++) {
		printf( "char %d is '%c'\n", i, input_buffer[i] );
	}
	
	
}
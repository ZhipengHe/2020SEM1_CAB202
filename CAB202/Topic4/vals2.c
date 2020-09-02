#include <stdio.h>

// GLOBAL VARABLES HERE.
#define SIZE 10
int vals[SIZE];
int num_items;

int main ( void ) {
	printf("Pls enter %d integer values\n", SIZE);
	
	for ( num_items = 0; num_items < SIZE; num_items++ ) {
		int scanf_result = scanf("%d", &vals[num_items]);
		
		if (scanf_result <= 0) break;
	} 
	
	printf( "num_items = %d\n", num_items );
	
	for ( int i = 0; i < num_items; i++){
		printf("vals[%d] = %d\n", i, vals[i]);
	}
	
	
}
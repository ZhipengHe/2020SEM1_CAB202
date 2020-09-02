#include <stdio.h>

// GLOBAL VARABLES HERE.
#define SIZE 10
int vals[SIZE];
int num_items;

int odd_vals[SIZE];
int odd_count;

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
	
	int sum = 0;
	
	for ( int i = 0; i < num_items; i++){
		sum += vals[i];
	}
	
	printf( "sum = %d\n", sum );
	
	// Copy odd values to another array
	odd_count = 0;
	
	for (int i = 0; odd_count < SIZE && i < num_items; i++) {
		if ( vals[i] % 2 == 1 ) {
			odd_vals[odd_count] = vals[i];
			odd_count ++;
		}
	}
	
	printf( "odd_count = %d\n", odd_count );
	
	for ( int i = 0; i < odd_count; i++) {
		printf("odd_vals[%d] = %d\n", i, odd_vals[i]);
	}
}
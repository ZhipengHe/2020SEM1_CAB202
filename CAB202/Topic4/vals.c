#include <stdio.h>

int main ( void ) {
	printf("Pls enter 3 integer values\n");
	int vals[3];
	
	for ( int i = 0; i < 3; i++ ) {
		scanf("%d", &vals[i]);
	} 
	
	for ( int i = 0; i < 3; i++){
		printf("vals[%d] = %d\n", i, vals[i]);
	}
	
	
}
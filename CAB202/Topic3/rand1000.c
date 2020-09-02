#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main () {
	srand(time(NULL));
	
	for (int i = 0; i < 1000; i ++ ) {
		printf("%d\n", 1 + rand() % 1000 );
	}
	
	return 0;
}
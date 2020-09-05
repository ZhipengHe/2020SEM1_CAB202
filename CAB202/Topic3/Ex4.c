#include <stdio.h>

void cab_202( void ) {
    // Insert your solution here.
    for (int i = 96; i <= 3295; ++i) {
    	if (i % 3 != 0 && i % 13 != 0) {
    		printf("%d\n", i);
    	} else if (i % 3 == 0 && i % 13 != 0) {
    		printf("CAB\n");
    	} else if (i % 3 != 0 && i % 13 == 0) {
    		printf("202\n");
    	} else {
    		printf("CAB202\n");
    	}
    }

}

int main( void ) {
	cab_202( );
	return 0;
}

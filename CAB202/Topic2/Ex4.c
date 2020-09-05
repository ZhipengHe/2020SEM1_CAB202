#include <stdio.h>

void is_leap( void ) {
    //  Insert your solution here
    printf("What year would you like to check?\n");
    int year;
    scanf("%d", &year);
    if (year % 4 != 0) {
    	printf("It turns out that %d is not a leap year.\n", year);
    } else {
    	if (year % 100 == 0 && year % 400 != 0) {
    		printf("It turns out that %d is not a leap year.\n", year);	
    	} else {
    		printf("It turns out that %d is a leap year.\n", year);
    	}
    }
}


int main() {
	is_leap();
	return 0;
}

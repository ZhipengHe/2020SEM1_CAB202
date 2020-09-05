#include <stdio.h>
#include <ctype.h>

void tennis_anyone( void ) {
    // Insert your solution here
    printf("How does the weather look?\ns - sunny\no - overcast\nr - rainy\n");
    char choice1;
    char choice2;
    scanf(" %c", &choice1);
    if (choice1 == 's') {
    	printf("Please rate the humidity:\nh - high\nn - normal\n");
    	scanf(" %c", &choice2);
    	if (choice2 == 'h') {
    		printf("It is just too hot and sticky for tennis.\n");
    	} else if (choice2 == 'n') {
    		printf("This is a good day for tennis.\n");
    	} else {
    		printf("Invalid choice for humidity: '%c'\nValid options are 'h', and 'n'.\n", choice2);
    	}
    } else if (choice1 == 'o') {
    	printf("This is a good day for tennis.\n");
    } else if (choice1 == 'r') {
    	printf("Please rate the wind strength:\nw - windy\nc - calm\n");
    	scanf(" %c", &choice2);
    	if (choice2 == 'w') {
    		printf("It is just too wild and windy for tennis.\n");
    	} else if (choice2 == 'c') {
    		printf("This is a good day for tennis.\n");
    	} else {
    		printf("Invalid choice for wind: '%c'\nValid options are 'w', and 'c'.\n", choice2);
    	}
    } else {
    	printf("Invalid choice for outlook: '%c'\nValid options are 's', 'o', and 'r'.\n", choice1);
    }
}


int main(void) {
	tennis_anyone();
	return 0;
}
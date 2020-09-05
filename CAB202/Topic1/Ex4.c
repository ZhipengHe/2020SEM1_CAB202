#include <stdio.h>

void cel2far(void) {
	// Insert your solution here.
	printf("Please enter the temperature, measured in degrees Celsius:\n");
	double celsius;
	double fahrenheit;
	scanf("%lf", &celsius);
	fahrenheit = 9.0 / 5 * celsius +32;
	printf("Celsius%.4lf is equal to Fahrenheit%.4lf.\n", celsius, fahrenheit);
}

int main(void) {
	cel2far();
	return 0;
}
#include <stdio.h>

void far2cel(void) {
	// (a) Prompt user to enter value.
	printf("Please enter the temperature, measured in degrees Fahrenheit:\n");
	// (b) Declare local variable to hold input value.
	double celsius;
	double fahrenheit;
	// (c) Read input value.
	scanf("%lf", &fahrenheit);
	// (d) Compute output value.
	celsius = 5.0 / 9 * (fahrenheit - 32.0);
	// (e) Display the result.
	printf("Fahrenheit %.4lf is equal to Celsius %.4lf.\n", fahrenheit, celsius);
}

int main(void) {
	far2cel();
	return 0;
}
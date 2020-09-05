#include <stdio.h>
#include <math.h>

void area_circ(void) {
	// (a) Prompt user to enter value.
	printf("Please enter the radius:\n");
	// (b) Declare local variable to hold input value.
	double areas;
	double radius;
	// (c) Read input value.
	scanf("%lf", &radius);
	// (d) Compute output value.
	areas = M_PI * pow(radius, 2);
	// (e) Display the result.
	printf("The area corresponding to a radius of%.5lf units is%.5lf square units.\n", radius, areas);
}

int main(void) {
	area_circ();
	return 0;
}

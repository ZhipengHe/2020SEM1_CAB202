#include <stdio.h>
#include <math.h>

void vol_cyl(void) {
	// Insert your solution here.
	printf("Please enter the radius:\n");
	double radius;
	double volume;
	scanf("%lf", &radius);
	printf("Please enter the height:\n");
	double height;
	scanf("%lf", &height);
	volume = M_PI * pow(radius, 2) * height;
	printf("The volume of a cylinder with a radius of %.5lf and a height of %.5lf units is %.5lf cubic units.\n", radius, height, volume);

}

int main(void) {
	vol_cyl();
	return 0;
}
#include <stdio.h>

void point_3d( void ) {
    // Insert your solution here.
    printf("Please enter the values for X and Y:\n");
    double x;
    double y;
    double z;
    scanf("%lf", &x);
    scanf("%lf", &y);
    if (x >= 0 && y >= 0) {
    	z = 1.75001 * x + 5.84707 * y;
    } else if (x >= 0 && y < 0) {
    	z = 1.75001 * x + y * y;
    } else if (x < 0 && y >= 0) {
    	z = x * x + 5.84707 * y;
    } else {
    	z = x * x + y * y;
    }
    printf("(%.4lf, %.4lf) -> %.4lf.\n", x, y, z);
}


int main() {
	point_3d();
	return 0;
}

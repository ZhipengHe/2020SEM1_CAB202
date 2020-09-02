#include <stdint.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	char m[5] = {
            "", // Empty string
            "A", // String with one symbol.
            "Hello from CAB202!", // Multiple symbols
            "1234567890abcdefghijklmnopqrstuvwxyz", // Longer than buffer size.
            NULL, // End of list 
        };
        int i = 0;
    while (m[i] != 0) {
    	printf("this is %c \n", m[i]);
    	i++;
    }
	return 0;
}
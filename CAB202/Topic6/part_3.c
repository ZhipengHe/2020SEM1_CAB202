#include <stdio.h>

void show_byte(unsigned char val){
	printf("0b");
	
	for (int i = 7; i >= 0; i--) {
		printf("%d", (val & (1 << i)) >> i);
	}
}

#define prt(expr) {printf("  " #expr " base 10 is "); show_byte(expr); printf(" base 2\n");}

int main () {
	unsigned char seventy_five = 75;
	unsigned char twelve = 12;
	unsigned char minus_twelve = -12;
	
	printf(" 75 base 10 is "); show_byte(seventy_five); printf(" base 2\n");
	printf(" 12 base 10 is "); show_byte(twelve); printf(" base 2\n");
	printf("-12 base 10 is "); show_byte(minus_twelve); printf(" base 2\n");
	printf("-12 base 10 is also %d base 10!!!\n", minus_twelve);
	
	printf("Multiplication by powers of 2.\n");
	prt(7);
	prt(7*2);
	prt(7*4);
	prt(7*8);
	prt(7*16);
	prt(7*32);
	// prt(7*64);
	// prt(7*128);
	
	printf("Left shift operator.\n");
	prt(7<<0);
	prt(7<<1);
	prt(7<<2);
	prt(7<<3);
	prt(7<<4);
	prt(7<<5);
	// prt(7<<6);
	// prt(7<<7);
	
	// In general: x << y  ==  x * (two to the power of y).
	// ----------------------------------------------------
	
	printf("Division by powers of 2.\n");
	prt((7<<3));
	prt((7<<3)/2);
	prt((7<<3)/4);
	prt((7<<3)/8);
	prt((7<<3)/16);
	prt((7<<3)/32);
	
	printf("Right shift operator.\n");
	prt((7<<3)>>0);
	prt((7<<3)>>1);
	prt((7<<3)>>2);
	prt((7<<3)>>3);
	prt((7<<3)>>4);
	prt((7<<3)>>5);
	
	// In general: x >> y  ==  x / (two to the power of y).
	// ----------------------------------------------------
	
	printf("Bitwise AND operator compared to logical AND operator.\n");
	prt( 0b00110011 & 0b10101010 );  // bitwise AND -> 0b00100010
	prt( 0b00110011 && 0b10101010 ); // logical AND -> 0b00000001
	// anything non zero is considerd to be "true". 0b00110011 is non zero, so
	// it's true. The second operand is not zero, so its true. (true && true == 1). 
	
	printf("Bitwise OR operator compared to logical OR operator.\n");
	prt( 0b00110011 | 0b10101010 );  // bitwise OR -> 0b10111011
	prt( 0b00110011 || 0b10101010 ); // logical OR -> 0b00000001
	// anything non zero is considerd to be "true". 0b00110011 is non zero, so
	// it's true. The second operand is immaterial in this case. (true || anything == 1). 
	
	printf("Bitwise AND operator compared to logical AND operator.\n");
	prt( 0b01010101 & 0b10101010 );  // bitwise AND -> 0b00000000
	prt( 0b01010101 && 0b10101010 ); // logical AND -> 0b00000001
	
	printf("Bitwise XOR operator.\n");
	prt( 0b01010101 ^ 0b10101010 );  // bitwise XOR -> 0b11111111
	prt( 0b00110011 ^ 0b10101010 );  // bitwise XOR -> 0b10011001
    //     --------
	// bit 76543210
	
	// & identifies positions where bit values are both 1
    // | identifies where	either bit is 1
	// ^ identifies where both bits are different
	
	printf("Bitwise COMPLEMENT operator.\n");
	prt( ~ 0b01010101 );  // bitwise COMP -> 0b10101010
	prt( (unsigned char)(~(0b00110011 ^ 0b10101010)) );  // COMPLEMENT of XOR -> 0b01100110
	
	// combining complement with xor finds bits that are the same.
	
	
	
	return 0;
}
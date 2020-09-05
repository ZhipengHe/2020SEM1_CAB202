#include <stdio.h>
#include <stdlib.h>

// INSERT GLOBALS HERE.
unsigned char var_01;
unsigned char var_02;
unsigned char var_03;
unsigned char var_04;
unsigned char var_05;
unsigned char var_06;
unsigned char var_07;
unsigned char var_08;
unsigned char var_09;
unsigned char var_10;

void turn_off_bits () {
    // INSERT ASSIGNMENT OPERATIONS HERE.
    var_01 = var_01 & ~1;

    var_02 = var_02 & ~(1 << 3);
    
    var_03 = var_03 & ~(1 << 5 | 1);
    
    var_04 = var_04 & ~(1 << 1 | 1 << 2);
    
    var_05 = var_05 & ~(1 << 3 | 1 << 7 | 1);
    
    var_06 = var_06 & ~(1 << 1 | 1 << 6 | 1 << 2);
    
    var_07 = var_07 & ~(1 | 1 << 1 | 1 << 5 | 1 << 7);
    
    var_08 = var_08 & ~(1 << 1 | 1 << 2 | 1 << 4 | 1 << 5);
    
    var_09 = var_09 & ~(1 | 1 << 1 | 1 << 4 | 1 << 5 | 1 << 7);
    
    var_10 = var_10 & ~(1 << 1 | 1 << 3 | 1 << 4 | 1 << 6 | 1 << 7);
}


void print_bits( const char * label, unsigned char val){
    printf("%s bits = {", label);
    
    int deja = 0;

    for (int i = 0; i <= 7; i++) {
        if ( (val & (1 << i)) ) {
            printf("%s%d", (deja ? ", " : ""), i);
            deja = 1;
        }
    }

    printf("}\n");
}

int main(int argc, char * argv[]) {
    unsigned char init_val = 0;
    
    if (argc > 1) init_val = atoi(argv[1]) & 255;

    var_01 = init_val;
    var_02 = init_val;
    var_03 = init_val;
    var_04 = init_val;
    var_05 = init_val;
    var_06 = init_val;
    var_07 = init_val;
    var_08 = init_val;
    var_09 = init_val;
    var_10 = init_val;


    turn_off_bits();

    print_bits("Initial", init_val);
    print_bits("var_01", var_01);
    print_bits("var_02", var_02);
    print_bits("var_03", var_03);
    print_bits("var_04", var_04);
    print_bits("var_05", var_05);
    print_bits("var_06", var_06);
    print_bits("var_07", var_07);
    print_bits("var_08", var_08);
    print_bits("var_09", var_09);
    print_bits("var_10", var_10);


    return 0;
}

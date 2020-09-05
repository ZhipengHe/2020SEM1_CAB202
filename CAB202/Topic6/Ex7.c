#include <stdio.h>
#include <stdlib.h>

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_01 and out_var_01.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_02 and out_var_02.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_03 and out_var_03.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_04 and out_var_04.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_05 and out_var_05.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_06 and out_var_06.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_07 and out_var_07.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_08 and out_var_08.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_09 and out_var_09.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

//  Declare two global variables, each of them suitable to store a single byte.
//  The names of the variables should be input_var_10 and out_var_10.
//  *  DO NOT ASSIGN VALUEs TO THESE VARIABLES AT THE POINT OF DECLARATION.

// INSERT CODE HERE.

unsigned char input_var_01;
unsigned char out_var_01;
unsigned char input_var_02;
unsigned char out_var_02;
unsigned char input_var_03;
unsigned char out_var_03;
unsigned char input_var_04;
unsigned char out_var_04;
unsigned char input_var_05;
unsigned char out_var_05;
unsigned char input_var_06;
unsigned char out_var_06;
unsigned char input_var_07;
unsigned char out_var_07;
unsigned char input_var_08;
unsigned char out_var_08;
unsigned char input_var_09;
unsigned char out_var_09;
unsigned char input_var_10;
unsigned char out_var_10;


void modify_bits_helper (
    unsigned char out_bit,
    unsigned char *out_reg,
    unsigned char input_mask,
    unsigned char input_reg
) {
    unsigned char output_mask = (1 << out_bit);

    if ((input_reg & input_mask) == input_mask) {
        (*out_reg) |= output_mask;
    } else {
        (*out_reg) &= ~output_mask;
    }
}



void change_bits () {
    modify_bits_helper(2, &out_var_01, (1 << 0), input_var_01);
    //  Modify the value of bit 2 of global variable out_var_01,
    //  such that it is equal to 1 if and only if bit 0
    //  of input_var_01 is set. Leave all other bits unaltered.
    //  Do not alter input_var_01. 

    // INSERT CODE HERE.
    // if ( (input_var_01 & (1 << 0)) == (1 << 0) ) {
    //     out_var_01 = out_var_01 | (1 << 2);
    // } else {
    //     out_var_01 = out_var_01 & ~(1 << 2);
    // }


    modify_bits_helper(7, &out_var_02, (1 << 4), input_var_02);
    //  Modify the value of bit 7 of global variable out_var_02,
    //  such that it is equal to 1 if and only if bit 4
    //  of input_var_02 is set. Leave all other bits unaltered.
    //  Do not alter input_var_02. 

    // INSERT CODE HERE.
    // if (input_var_02 & (1 << 4) == (1 << 4)) {
    //     out_var_02 = out_var_02 | (1 << 7);
    // } else {
    //     out_var_02 = out_var_01 & ~(1 << 7);
    // }


    modify_bits_helper(4, &out_var_03, (1 << 0) | (1 << 2), input_var_03);
    //  Modify the value of bit 4 of global variable out_var_03,
    //  such that it is equal to 1 if and only if bits 0, 2
    //  of input_var_03 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_03. 

    // INSERT CODE HERE.
    // if (input_var_03 & ((1 << 0) | (1 << 2)) == ((1<<0) | (1<<2)) ) {
    //     out_var_03 = out_var_03 | (1 << 4);
    // } else {
    //     out_var_03 = out_var_03 & ~(1 << 4);
    // }


    modify_bits_helper(7, &out_var_04, (1 << 1) | (1 << 7), input_var_04);
    //  Modify the value of bit 7 of global variable out_var_04,
    //  such that it is equal to 1 if and only if bits 1, 7
    //  of input_var_04 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_04. 

    // INSERT CODE HERE.
    // if (input_var_04 & ((1 << 1) | (1 << 7)) == ((1 << 1) | (1 << 7)) ) {
    //     out_var_04 = out_var_04 | (1 << 7);
    // } else {
    //     out_var_04 = out_var_04 & ~(1 << 7);
    // }




    modify_bits_helper(1, &out_var_05, (1 << 0) | (1 << 2) | (1 << 5), input_var_05);
    //  Modify the value of bit 1 of global variable out_var_05,
    //  such that it is equal to 1 if and only if bits 0, 2, 5
    //  of input_var_05 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_05. 

    // INSERT CODE HERE.
    // if (input_var_05 & ((1 << 0) | (1 << 2) | (1 << 5)) == ((1 << 0) | (1 << 2) | (1 << 5)) ) {
    //     out_var_05 = out_var_05 | (1 << 1);
    // } else {
    //     out_var_05 = out_var_05 & ~(1 << 1);
    // }


    modify_bits_helper(1, &out_var_06, (1 << 3) | (1 << 4) | (1 << 6), input_var_06);
    //  Modify the value of bit 1 of global variable out_var_06,
    //  such that it is equal to 1 if and only if bits 3, 4, 6
    //  of input_var_06 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_06. 

    // INSERT CODE HERE.
    // if (input_var_06 & ((1 << 3) | (1 << 4) | (1 << 6) )== (1 << 3) | (1 << 4) | (1 << 6) ) {
    //     out_var_06 = out_var_06 | (1 << 1);
    // } else {
    //     out_var_06 = out_var_06 & ~(1 << 1);
    // }


    modify_bits_helper(7, &out_var_07, (1 << 0) | (1 << 3) | (1 << 4) | (1 << 6), input_var_07);
    //  Modify the value of bit 7 of global variable out_var_07,
    //  such that it is equal to 1 if and only if bits 0, 3, 4, 6
    //  of input_var_07 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_07. 

    // INSERT CODE HERE.
    // if (input_var_07 & ((1 << 0) | (1 << 3) | (1 << 4) | (1 << 6)) == ((1 << 0) | (1 << 3) | (1 << 4) | (1 << 6))) {
    //     out_var_07 = out_var_07 | (1 << 7);
    // } else {
    //     out_var_07 = out_var_07 & ~(1 << 7);
    // }
    


    modify_bits_helper(5, &out_var_08, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6), input_var_08);
    //  Modify the value of bit 5 of global variable out_var_08,
    //  such that it is equal to 1 if and only if bits 1, 4, 5, 6
    //  of input_var_08 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_08. 

    // INSERT CODE HERE.
    // if (input_var_08 & ((1 << 1) | (1 << 4) | (1 << 5) | (1 << 6)) == (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6)) {
    //     out_var_08 = out_var_08 | (1 << 5);
    // } else {
    //     out_var_08 = out_var_08 & ~(1 << 5);
    // }



    modify_bits_helper(3, &out_var_09, (1 << 0) | (1 << 1) | (1 << 2) | (1 << 5) | (1 << 7), input_var_09);
    //  Modify the value of bit 3 of global variable out_var_09,
    //  such that it is equal to 1 if and only if bits 0, 1, 2, 5, 7
    //  of input_var_09 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_09. 

    // INSERT CODE HERE.
    // if (input_var_09 & ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 5) | (1 << 7)) == (1 << 0) | (1 << 1) | (1 << 2) | (1 << 5) | (1 << 7))  {
    //     out_var_09 = out_var_09 | (1 << 3);
    // } else {
    //     out_var_09 = out_var_09 & ~(1 << 3);
    // }

    modify_bits_helper(7, &out_var_10, (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7), input_var_10);
    //  Modify the value of bit 7 of global variable out_var_10,
    //  such that it is equal to 1 if and only if bits 3, 4, 5, 6, 7
    //  of input_var_10 are all set. Leave all other bits unaltered.
    //  Do not alter input_var_10. 

    // INSERT CODE HERE.
    // if (input_var_10 & ((1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)) == (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)){
    //     out_var_10 = out_var_10 | (1 << 7);
    // } else {
    //     out_var_10 = out_var_10 & ~(1 << 7);
    // }

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
    unsigned char out_init = 0;
    
    if (argc > 1) init_val = atoi(argv[1]) & 255;
    if (argc > 2) out_init = atoi(argv[2]) & 255;

    input_var_01 = init_val;
    out_var_01 = out_init;

    input_var_02 = init_val;
    out_var_02 = out_init;

    input_var_03 = init_val;
    out_var_03 = out_init;

    input_var_04 = init_val;
    out_var_04 = out_init;

    input_var_05 = init_val;
    out_var_05 = out_init;

    input_var_06 = init_val;
    out_var_06 = out_init;

    input_var_07 = init_val;
    out_var_07 = out_init;

    input_var_08 = init_val;
    out_var_08 = out_init;

    input_var_09 = init_val;
    out_var_09 = out_init;

    input_var_10 = init_val;
    out_var_10 = out_init;

    change_bits();

    print_bits("Input Value         ", init_val);
    print_bits("Initial output Value", out_init);

    printf("\nSearch for bit 0.\n");
    printf("Set or clear bit 2:\n");
    print_bits("out_var_01", out_var_01);

    printf("\nSearch for bit 4.\n");
    printf("Set or clear bit 7:\n");
    print_bits("out_var_02", out_var_02);

    printf("\nSearch for bits 0, 2.\n");
    printf("Set or clear bit 4:\n");
    print_bits("out_var_03", out_var_03);

    printf("\nSearch for bits 1, 7.\n");
    printf("Set or clear bit 7:\n");
    print_bits("out_var_04", out_var_04);

    printf("\nSearch for bits 0, 2, 5.\n");
    printf("Set or clear bit 1:\n");
    print_bits("out_var_05", out_var_05);

    printf("\nSearch for bits 3, 4, 6.\n");
    printf("Set or clear bit 1:\n");
    print_bits("out_var_06", out_var_06);

    printf("\nSearch for bits 0, 3, 4, 6.\n");
    printf("Set or clear bit 7:\n");
    print_bits("out_var_07", out_var_07);

    printf("\nSearch for bits 1, 4, 5, 6.\n");
    printf("Set or clear bit 5:\n");
    print_bits("out_var_08", out_var_08);

    printf("\nSearch for bits 0, 1, 2, 5, 7.\n");
    printf("Set or clear bit 3:\n");
    print_bits("out_var_09", out_var_09);

    printf("\nSearch for bits 3, 4, 5, 6, 7.\n");
    printf("Set or clear bit 7:\n");
    print_bits("out_var_10", out_var_10);

    return 0;
}

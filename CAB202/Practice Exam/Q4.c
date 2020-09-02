#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
#include <ctype.h> 

#define MIN_VAL 'a' 
#define MAX_VAL 'z' 

// Declare and initialise any additional global variables here, as required. 
#define CHAR_SET_SIZE (1 + 'z' - 'a')

void process(int x) { 
    // Process one value here. 
} 

void post_process() { 
    // Compute derived results here. 
} 

void print() { 
    // Print results here 
    printf( )
    for ( int i = 0; i < CHAR_SET_SIZE; i++ ) {
        printf( "%c\t%d\n", 'a' + i, term_count[i] );
    }
} 

int main(void) { 
    char ch; 
    while (1 == scanf("%c", &ch)) { 
        process(ch); 
    } 
    post_process(); 
    print(); 
    return 0; 
}
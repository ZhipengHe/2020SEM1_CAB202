#include <ctype.h>
#include <stdio.h>

//  HERE: Declare a global array of char called val with capacity to hold
//  89 elements. AMS will insert data into this array prior to calling your
//  function.
char val[89];
//  HERE: Declare a global array of int called term_count with capacity to hold
//  one element for each symbol in the designated character range.
//  AMS will read results from this array after calling your function.
//  The contents of this array will be overwritten by AMS before calling your
//  function, so you should not make any assumptions about it.
int term_count[26];

void string_char_count_2() {
    //  Insert your solution here.
    for (int j = 0; j < 26; j++) {
        term_count[j] = 0;
    }

    for (int i = 0; i < 89; i++) {
        if (val[i] == '\0') {
            break;
        }
        char temp = val[i];
        for (int m = 0; m < 26; m++) {
            if (temp == 'a' + m) {
                term_count[m]++;
            }
        }
    }
}

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHAR_SET_SIZE (1 + 'z' - 'a')

int main() {
    // AMS will do something a bit like this before it calls your solution.
    // Do not change any of this; instead, write your solution in such a way
    //  that it works correctly even with this.
    srand(time(NULL));

    for (int i = 0; i < CHAR_SET_SIZE; i++) {
        term_count[i] = rand();
    }

    //  Read as much data from stdin as we can, and append each char to the
    //  buffer.
    int chars_read = 0;

    for (chars_read = 0; chars_read < (89 - 1); chars_read++) {
        int scanf_result = scanf("%c", &val[chars_read]);

        if (1 != scanf_result) break;
    }

    //  Terminate the string in the buffer.
    val[chars_read] = 0;

    // Pad remainder of string with garbage to help with debugging.
    for (int i = chars_read + 1; i < (89); i++) {
        val[i] = 'a' + rand() % CHAR_SET_SIZE;
    }

    // Call submitted code.
    string_char_count_2();

    // Display contents of string, and number of occurrences of each symbol.
    printf("Input string        = '%s'\n", val);
    printf("Input string length = %zu\n", strlen(val));
    printf("Array capacity      = %d\n", 89);
    printf("Symbol frequencies:\n");

    for (int i = 0; i < CHAR_SET_SIZE; i++) {
        printf("%c\t%d\n", 'a' + i, term_count[i]);
    }

    return 0;
}
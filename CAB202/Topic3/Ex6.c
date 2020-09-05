#include <stdio.h>
#include <ctype.h>

void word_count() {
    //  Insert your solution here.
    int line = 0;
    int word = 0;
    int character = 0;
    char ch_cur = ' ';
    char ch_pre = ' ';
    while(1) {
    	ch_pre = ch_cur;
    	int scanf_ret = scanf("%c", &ch_cur);
    	if (scanf_ret < 0) {
    		break;
    	} else {
    		character++;
            if (ch_cur == '\n') {
    			line++;
    		}
    		if (isspace(ch_pre) && !isspace(ch_cur)) {
    			word++;
    		}
    	}
    }
    printf("Lines: %d, Words: %d, Characters: %d.\n", line, word, character);
}


int main() {
	word_count();
	return 0;
}

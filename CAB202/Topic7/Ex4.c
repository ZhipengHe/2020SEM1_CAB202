#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define SET_BIT(reg, pin)           (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)         (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value)  (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin)         (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)        (BIT_VALUE((reg),(pin))==1)

// Declare a global variable named was_on which is suitable to store a 
//  single byte. Do not initialise the variable at the point of declaration. 
//  The value of the variable will be managed by the setup_switch and is_rising_edge
//  functions.

// INSERT DECLARATION HERE

uint8_t was_on;

//  Define a parameterless function called setup_switch which returns nothing.
//
//  The function must enable digital input from a pull-down switch connected to 
//  the I/O pin labelled "9" on an Arduino Uno board. It should also
//  assign zero to global variable .
//
//  Apart from updating the global variable, the function must update only 
//  those bits required to enable digital input from the designated switch. The
//  function must achieve the required side effect regardless of the prior 
//  state of all registers. In particular, **do not assume** that memory has
//  previously been initialised with some value, zero or otherwise. 

// INSERT CODE HERE.

void setup_switch() {
    CLEAR_BIT(DDRB, 1);
    was_on = 0;
}

//  Define a parameterless function called is_rising_edge which returns an 
//  int which indicates that a rising edge has been detected in the state of 
//  a pull-down switch connected to I/O pin labelled "9" on an 
//  Arduino Uno board.
//
//  The function returns 1 if a rising edge is detected in the signal from the 
//  switch and 0 otherwise.
//
//  The function may read and update the value of global variable was_on 
//  but apart from that it may not alter the value of any other register or 
//  global variable. 


// INSERT CODE HERE.

int is_rising_edge() {
    int result = 0;
    if (was_on == 0 && BIT_IS_SET(PINB, 1)) {
        result = 1;
    }
    was_on = BIT_VALUE(PINB, 1);
    return result;
}




void setup_builtin_led(void);
void toggle_builtin_led(void);
void simulate_other_setup();

int main(void) {
    simulate_other_setup();
    setup_builtin_led();
    setup_switch();

    while(1) {
        int rising = is_rising_edge();
        
        if (rising) toggle_builtin_led();
    } 
}

void setup_builtin_led(void) {
    DDRB |= (1<<5);
}

void toggle_builtin_led(void) {
    PORTB ^= (1<<5);
}

void simulate_other_setup(void) {
    //  Simulate situation where other important setup actions have been 
    //  performed prior to calling our functions.
    srand(5840);
     DDRB = rand() & 255;
     PINB = rand() & 255;
    PORTB = rand() & 255;
     DDRC = rand() & 255;
     PINC = rand() & 255;
    PORTC = rand() & 255;
     DDRD = rand() & 255;
     PIND = rand() & 255;
    PORTD = rand() & 255;
    was_on =  rand() & 255;
}


#include <stdint.h>
#include <stdio.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

/*  Setting data directions in a data direction register (DDR)
 *
 *
 *  Setting, clearing, and reading bits in registers.
 *  reg is the name of a register; pin is the index (0..7)
 *  of the bit to set, clear or read.
 *  (WRITE_BIT is a combination of CLEAR_BIT & SET_BIT)
 */

#define SET_BIT(reg, pin)           (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)         (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value)  (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin)         (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)        (BIT_VALUE((reg),(pin))==1)


#define F_CPU 16000000UL


//	(a) Implement a function called uart_setup which requires no arguments and
//	returns no value.
    //	(a.a) Set the data transmission rate to 4800 bits per second,
    //	using the formula provided in the lecture notes and slides. This depends 
    //  on the UART operating mode, baud rate, and CPU clock speed.

    //	(a.b) Set the UART operating mode to normal speed.

    //	(a.c) Enable receive and transmit, but do not enable any UART-related
    //	interrupts.

    //	(a.d) Set the character size to 7 bits, with even parity
    //	and 1 stop bit.

    //	(b) End of function uart_setup

void uart_setup() {
    unsigned int baud = 4800;
    unsigned int ubrr = (F_CPU/16/baud) - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)(ubrr);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UPM01) | (0 << USBS0);
}



/*
***************************************************************************
**  Send one byte, protecting against overrun in the transmit buffer.
**
**  Param: data - a byte to be transmitted.
**
**  Returns: Nothing.
***************************************************************************
*/
void uart_putbyte(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));

    // Send data by assigning into UDR0
    UDR0 = data;
}

/*
***************************************************************************
**  Attempt to receive one byte, returning immediately to sender.
**
**  Param: buffer - the address of a byte in which a result may be stored.
**
**  Returns: If a byte is available returns 1 and stores the incoming byte in 
**      location referenced by buffer. Otherwise returns 0 and makes no other
**      change to the state.
***************************************************************************
*/
int uart_getbyte(unsigned char *buffer) {
    // If receive buffer contains data...
    if (UCSR0A & (1 << RXC0)) {
        // Copy received byte from UDR0 into memory location (*buffer)
        *buffer = UDR0;
        // 
        return 1;
    }
    else {
        return 0;
    }
}

/*
***************************************************************************
**  Initialise LEDS to display byte values as "binary".
***************************************************************************
*/
void setup_leds(void) {
    // Use pins 2, 3, 4, 5, 6, 7 of port D and pins 0, 1 of port B as output. 
    DDRD |= ~3;
    DDRB |= 3;
}

/*
***************************************************************************
**  Display value of a byte via LED.
**
**  Param: value - a byte to display.
**
**  Side effects: The LEDs connected to pins 0, 1 of Port B and 2,...,7 of
**      port D are illuminated (or otherwise) to display the binary 
**      representation of the argument. 
***************************************************************************
*/
void display(unsigned char value) {
    // Pins 2, 3, 4, 5, 6, 7 of port D are bits 2,...,7 of the argument.
    // Pins 0, 1 of port B are bits 0, 1 of the argument.
    PORTD = (PORTD & 3) | (value & ~3);
    PORTB = (PORTB & ~3) | (value & 3);
}

/*
***************************************************************************
**  Implement main event loop.
***************************************************************************
*/
void process() {
    // One device sends +counter, the other sends -counter.
    // Both display the value received from the other.
    const int is_positive = 1;

    // Private variable which keeps its value between function calls.
    static unsigned char counter = 0;

    if (is_positive) {
        uart_putbyte(counter);
    }
    else {
        uart_putbyte(256-counter);
    }
    counter++;

    unsigned char received_byte;

    if (uart_getbyte(&received_byte)) {
        display(received_byte);
    }

    _delay_ms(300);
}


int main(void) {
    setup_leds();
    uart_setup();

    while (1) {
        process();
    }
}

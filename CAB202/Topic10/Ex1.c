#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

#define SET_BIT(reg, pin)           (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)         (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value)  (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin)         (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)        (BIT_VALUE((reg),(pin))==1)

/*
**  Define function adc_setup which sets up the registers required for analogue
**  to digital conversion on an ATmega328P microcontroller. The function requires
**  one argument, as noted below.
**  
**  Parameter: division_factor - an 8-bit unsigned integer by which the clock is
**      pre-scaled. Your function should carry out the appropriate initialisation
**      action for any value of division_factor in {4, 8, 16, 32, 64, 128}.
**  
**  To do this:
**  (1) Assign a value to ADMUX such that bits (REFS1,REFS0) together specify
**      that AVcc with external capacitor at AREF pin is used, and all other
**      bits are zero.
**  
**      Ref: Section 24.9.1 of the ATmega328P data sheet.
**  
**  (2) Assign a value to ADCSRA such that ADC is enabled, and that bits
**      (ADPS2,ADPS1,ADPS0) together specify that the clock pre-scale division
**      factor will be equal to that specified by division_factor.
**  
**      Ref: Section 24.9.2 of ATmega328P data sheet, with particular attention
**      to table 24.5.
**  
**      Note: A division factor of 2 will not be used in this exercise.
*/
void adc_setup( uint8_t division_factor ) {
 // INSERT CODE
    ADMUX = (0 << REFS1) | (1 << REFS0);
    if (division_factor == 4) {
        ADCSRA = (1 << ADEN) | (0 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
    } else if (division_factor == 8) {
        ADCSRA = (1 << ADEN) | (0 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    } else if (division_factor == 16) {
        ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1) | (0 << ADPS0);
    } else if (division_factor == 32) {
        ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0);
    } else if (division_factor == 64) {
        ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);
    } else if (division_factor == 128) {
        ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    }
}

/*
**  Complete function adc_read which reads a numeric value from a designated
**  ADC channel on an ATmega328P microcontroller. The function requires one
**  argument, as noted below.
**  
**  Parameter: channel - an 8-bit unsigned integer which specifies the analogue
**      input channel to be sampled. Your function should work correctly for any
**      channel in {0,...,7}.
**  
**  Return: A 16-bit int containing the 10-bit analogue result.
**  
**  To do this:
**  (1) Assign a new value to ADMUX such that bits 4, 5, 6, and 7 are not changed,
**      and bits (MUX3,MUX2,MUX1,MUX0) are equal to the supplied channel value.
**  
**  (2) Start the conversion by setting bit ADSC of register ADCSRA.
**  
**  (3) Wait for the conversion to complete.
**  
**  (4) Return the value of register ADC.
**  
**  Ref: Section 24.9.1 of the ATmega328P data sheet.
**  Ref: Sample code provided in lecture.
*/
uint16_t adc_read( uint8_t channel ) {
    if (channel == 0)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
    if (channel == 1)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (1 << MUX0);
    if (channel == 2)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (0 << MUX0);
    if (channel == 3)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (0 << MUX2) | (1 << MUX1) | (1 << MUX0);
    if (channel == 4)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (1 << MUX2) | (0 << MUX1) | (0 << MUX0);
    if (channel == 5)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (1 << MUX2) | (0 << MUX1) | (1 << MUX0);
    if (channel == 6)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (0 << MUX0);
    if (channel == 7)
    ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0);
    // Don't change anything else!

    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1 << ADSC);

    // wait for conversion to complete
    // ADSC becomes '0' again
    // till then, run loop continuously
#if !__AMS__
    while (ADCSRA & (1 << ADSC));
#endif

    return (ADC);
}

// uart definitions
#define BAUD (9600)
#define MYUBRR (F_CPU/16/BAUD-1)

// This buffer may be any size from 2 to 256 bytes.
#define  TX_BUFFER_SIZE  64

//uart definitions
static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t tx_buffer_head;
static volatile uint8_t tx_buffer_tail;

//Functions declaration

void setup(void);
void process(void);
void uart_init(unsigned int ubrr);

//uart functions
void uart_putchar(uint8_t c);
void uart_putstring(char* s);

// END function declarations

//main loop
int main() {
    setup();

    for (;; ) {
        process();
        _delay_ms(50);
    }
}

//initialises ADC and UART port
void setup(void) {
    //init uart
    uart_init(MYUBRR);

    // Enable orange LED
    SET_BIT(DDRB, 5);

    //  Enable ADC with pre-scale division factor 128. 
    adc_setup(128);
}

void process(void) {
    char temp_buf[64];

    uint16_t pot = adc_read(0);

    // convert uint16_t to string
    snprintf(temp_buf, sizeof(temp_buf), "%d", pot);

    if (pot > 512)
        SET_BIT(PORTB, PB5);
    else
        CLEAR_BIT(PORTB, PB5);

    //send serial data
    uart_putstring(temp_buf);
    uart_putstring("\r\n");
}


/********** auxiliary functions *************/

//PLEASE NOTE THIS VERSION OF UART USES INTERRUPTS

/*  ****** serial uart definitions ************ */
/******************  interrupt based  ********/

// Initialize the UART
void uart_init(unsigned int ubrr) {
    cli();
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)(ubrr);
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    tx_buffer_head = tx_buffer_tail = 0;
    sei();
}

// Transmit a byte
void uart_putchar(uint8_t c) {
    uint8_t i;

    i = tx_buffer_head + 1;
    if (i >= TX_BUFFER_SIZE) i = 0;
    while (tx_buffer_tail == i); // wait until space in buffer
    //cli();
    tx_buffer[i] = c;
    tx_buffer_head = i;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << UDRIE0);
    //sei();
}

// Transmit a string
void uart_putstring(char* s) {
    // transmit character until NULL is reached
    while (*s > 0) uart_putchar(*s++);
}

// Transmit Interrupt
ISR(USART_UDRE_vect) {
    uint8_t i;

    if (tx_buffer_head == tx_buffer_tail) {
        // buffer is empty, disable transmit interrupt
        UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    }
    else {
        i = tx_buffer_tail + 1;
        if (i >= TX_BUFFER_SIZE) i = 0;
        UDR0 = tx_buffer[i];
        tx_buffer_tail = i;
    }
}

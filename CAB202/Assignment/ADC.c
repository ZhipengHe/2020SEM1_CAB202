#include <avr/io.h>


/*
 *  Setting data directions in a data direction register (DDR)
 *
 *
 *  Setting, clearing, and reading bits in registers.
 *	reg is the name of a register; pin is the index (0..7)
 *  of the bit to set, clear or read.
 *  (WRITE_BIT is a combination of CLEAR_BIT & SET_BIT)
 */

#define SET_BIT(reg, pin)		    (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin)		  (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value)   (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin)		  (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin)	     (BIT_VALUE((reg),(pin))==1)

//uart definitions
#define BAUD (9600)
#define MYUBRR (F_CPU/16/BAUD-1)

// These buffers may be any size from 2 to 256 bytes.
#define  RX_BUFFER_SIZE  64
#define  TX_BUFFER_SIZE  64


//uart definitions
unsigned char rx_buf;

static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t tx_buffer_head;
static volatile uint8_t tx_buffer_tail;
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t rx_buffer_head;
static volatile uint8_t rx_buffer_tail;


//Functions declaration
void setup(void);
void process(void);
void uart_init(unsigned int ubrr);
//uart functions
void uart_putchar(uint8_t c);
uint8_t uart_getchar(void);
uint8_t uart_available(void);
void uart_putstring(unsigned char* s);
void uart_getLine(unsigned char* buf, uint8_t n);
//ADC functions
uint16_t adc_read(uint8_t channel);
void adc_init();
//string convertion functions 
void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);

// END function declarations


//main loop
int main() {
	setup();

	for ( ;; ) {
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
  
    // initialise adc
	// ADC Enable and pre-scaler of 128: ref table 24-5 in datasheet
    // ADEN  = 1
    // ADPS2 = 1, ADPS1 = 1, ADPS0 = 1
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

   // select channel and ref input voltage
   // channel 0, PC0 (A0 on the uno)
   // MUX0=0, MUX1=0, MUX2=0, MUX3=0
   // REFS0=1
   // REFS1=0
    ADMUX = (1 << REFS0);
  
}


void process(void) {
	
   char temp_buf[64];
  
  
   // Start single conversion by setting ADSC bit in ADCSRA
	ADCSRA |= (1 << ADSC);

	// Wait for ADSC bit to clear, signalling conversion complete.
	 while ( ADCSRA & (1 << ADSC) ) {}

	// Result now available in ADC
	 uint16_t pot = ADC;
  
    //convert float to a string 
    // ftoa(pot, temp_buf, 4);
    // convert uint16_t to string
     snprintf(temp_buf, sizeof(temp_buf), "%d", pot);
  
	 //when converted value is above a threshold, perform an action 
     if (pot > 512)
       SET_BIT(PORTB,PB5);
   	 else
       CLEAR_BIT(PORTB,PB5);
    

   //send serial data
   uart_putstring((unsigned char *) temp_buf);
   uart_putchar('\n');

	
}


/********** auxiliary functions *************/


// Reverses a string 'str' of length 'len' 
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
  
// Converts a given integer x to string str[].  
// d is the number of digits required in the output.  
// If d is more than the number of digits in x,  
// then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
  
// Converts a floating-point/double number to a string. 
void ftoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter  
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 




//PLEASE NOTE THIS VERSION OF UART USES INTERRUPTS

/*  ****** serial uart definitions ************ */
/******************  interrupt based  ********/

// Initialize the UART
void uart_init(unsigned int ubrr) {
  
  	cli();
	
	UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)(ubrr);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  	tx_buffer_head = tx_buffer_tail = 0;
	rx_buffer_head = rx_buffer_tail = 0;
  
	sei();
	
}



// Transmit a byte
void uart_putchar(uint8_t c) {
	uint8_t i;

	i = tx_buffer_head + 1;
	if ( i >= TX_BUFFER_SIZE ) i = 0;
	while ( tx_buffer_tail == i ); // wait until space in buffer
	//cli();
	tx_buffer[i] = c;
	tx_buffer_head = i;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << UDRIE0);
	//sei();
}

// Receive a byte
uint8_t uart_getchar(void) {
	uint8_t c, i;

	while ( rx_buffer_head == rx_buffer_tail ); // wait for character
	i = rx_buffer_tail + 1;
	if ( i >= RX_BUFFER_SIZE ) i = 0;
	c = rx_buffer[i];
	rx_buffer_tail = i;
	return c;
}


// Transmit a string
void uart_putstring(unsigned char* s)
{
    // transmit character until NULL is reached
    while(*s > 0) uart_putchar(*s++);
}


// Receive a string
void uart_getLine(unsigned char* buf, uint8_t n)
{
    uint8_t bufIdx = 0;
    unsigned char c;

    // while received character is not carriage return
    // and end of buffer has not been reached
    do
    {
        // receive character
        c = uart_getchar();

        // store character in buffer
        buf[bufIdx++] = c;
    }
    while((bufIdx < n) && (c != '\n'));

    // ensure buffer is null terminated
    buf[bufIdx] = 0;
}



uint8_t uart_available(void) {
	uint8_t head, tail;

	head = rx_buffer_head;
	tail = rx_buffer_tail;
	if ( head >= tail ) return head - tail;
	return RX_BUFFER_SIZE + head - tail;
}


// Transmit Interrupt
ISR(USART_UDRE_vect) {
	uint8_t i;

	if ( tx_buffer_head == tx_buffer_tail ) {
		// buffer is empty, disable transmit interrupt
		UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
	}
	else {
		i = tx_buffer_tail + 1;
		if ( i >= TX_BUFFER_SIZE ) i = 0;
		UDR0 = tx_buffer[i];
		tx_buffer_tail = i;
	}
}

// Receive Interrupt
ISR(USART_RX_vect) {
	uint8_t c, i;
  
	c = UDR0;
	i = rx_buffer_head + 1;
	if ( i >= RX_BUFFER_SIZE ) i = 0;
	if ( i != rx_buffer_tail ) {
		rx_buffer[i] = c;
		rx_buffer_head = i;
	}
}
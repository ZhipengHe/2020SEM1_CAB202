#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

/*
 *  Setting data directions in a data direction register (DDR)
 *
 *
 *  Setting, clearing, and reading bits in registers.
 *	reg is the name of a register; pin is the index (0..7)
 *  of the bit to set, clear or read.
 *  (WRITE_BIT is a combination of CLEAR_BIT & SET_BIT)
 */

#define SET_BIT(reg, pin) (reg) |= (1 << (pin))
#define CLEAR_BIT(reg, pin) (reg) &= ~(1 << (pin))
#define WRITE_BIT(reg, pin, value)                                             \
    (reg) = (((reg) & ~(1 << (pin))) | ((value) << (pin)))
#define BIT_VALUE(reg, pin) (((reg) >> (pin)) & 1)
#define BIT_IS_SET(reg, pin) (BIT_VALUE((reg), (pin)) == 1)

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT        11
// PP      PP       AA  AA        RR      RR          TT          1111
// PP      PP     AA      AA      RR      RR          TT            11
// PPPPPPPPP      AA      AA      RRRRRRRR            TT            11
// PP             AAAAAAAAAA      RR  RR              TT            11
// PP             AA      AA      RR    RR            TT            11
// PP             AA      AA      RR      RR          TT          111111
//
//===============================================================================
//  Part 1 ---- LCD Library
//  source from Topic 11 Lecture note
//  https://blackboard.qut.edu.au/bbcswebdav/pid-8394136-dt-content-rid-31504044_1/courses/CAB202_20se1/Topic11/CAB202%20Topic%2011%20%E2%80%93%20LCD.html
//===============================================================================

// --== WIRING ==--
// LCD GND  -> GND
// LCD VCC  -> 5V
// LCD V0   -> GND
// LCD RW   -> GND
// LCD LED Anode    -> 220 Ohm -> 5V
// LCD LED Cathode  -> GNDp
// Change the values in these defines to reflect
//  how you've hooked up the screen
// In 4-pin mode only DATA4:7 are used

#define LCD_USING_4PIN_MODE (1)

// #define LCD_DATA0_DDR (DDRD)
// #define LCD_DATA1_DDR (DDRD)
// #define LCD_DATA2_DDR (DDRD)
// #define LCD_DATA3_DDR (DDRD)
#define LCD_DATA4_DDR (DDRD)
#define LCD_DATA5_DDR (DDRD)
#define LCD_DATA6_DDR (DDRD)
#define LCD_DATA7_DDR (DDRD)

// #define LCD_DATA0_PORT (PORTD)
// #define LCD_DATA1_PORT (PORTD)
// #define LCD_DATA2_PORT (PORTD)
// #define LCD_DATA3_PORT (PORTD)
#define LCD_DATA4_PORT (PORTD)
#define LCD_DATA5_PORT (PORTD)
#define LCD_DATA6_PORT (PORTD)
#define LCD_DATA7_PORT (PORTD)

// #define LCD_DATA0_PIN (0)
// #define LCD_DATA1_PIN (1)
// #define LCD_DATA2_PIN (2)
// #define LCD_DATA3_PIN (3)
#define LCD_DATA4_PIN (4)
#define LCD_DATA5_PIN (5)
#define LCD_DATA6_PIN (6)
#define LCD_DATA7_PIN (7)

#define LCD_RS_DDR (DDRB)
#define LCD_ENABLE_DDR (DDRB)

#define LCD_RS_PORT (PORTB)
#define LCD_ENABLE_PORT (PORTB)

#define LCD_RS_PIN (1)
#define LCD_ENABLE_PIN (0)

// DATASHEET:
// https://s3-us-west-1.amazonaws.com/123d-circuits-datasheets/uploads%2F1431564901240-mni4g6oo875bfbt9-6492779e35179defaf4482c7ac4f9915%2FLCD-WH1602B-TMI.pdf

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

void lcd_init(void);
void lcd_write_string(uint8_t x, uint8_t y, char string[]);
void lcd_write_char(uint8_t x, uint8_t y, char val);

void lcd_clear(void);
void lcd_home(void);

void lcd_createChar(uint8_t, uint8_t[]);
void lcd_setCursor(uint8_t, uint8_t);

void lcd_noDisplay(void);
void lcd_display(void);
void lcd_noBlink(void);
void lcd_blink(void);
void lcd_noCursor(void);
void lcd_cursor(void);
void lcd_leftToRight(void);
void lcd_rightToLeft(void);
void lcd_autoscroll(void);
void lcd_noAutoscroll(void);
void scrollDisplayLeft(void);
void scrollDisplayRight(void);

size_t lcd_write(uint8_t);
void lcd_command(uint8_t);

void lcd_send(uint8_t, uint8_t);
void lcd_write4bits(uint8_t);
void lcd_write8bits(uint8_t);
void lcd_pulseEnable(void);

uint8_t _lcd_displayfunction;
uint8_t _lcd_displaycontrol;
uint8_t _lcd_displaymode;

/* ********************************************/
// START LIBRARY FUNCTIOMNS

void lcd_init(void) {
    // dotsize
    if (LCD_USING_4PIN_MODE) {
        _lcd_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    } else {
        _lcd_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
    }

    _lcd_displayfunction |= LCD_2LINE;

    // RS Pin
    LCD_RS_DDR |= (1 << LCD_RS_PIN);
    // Enable Pin
    LCD_ENABLE_DDR |= (1 << LCD_ENABLE_PIN);

#if LCD_USING_4PIN_MODE
    // Set DDR for all the data pins
    LCD_DATA4_DDR |= (1 << 4);
    LCD_DATA5_DDR |= (1 << 5);
    LCD_DATA6_DDR |= (1 << 6);
    LCD_DATA7_DDR |= (1 << 7);

#else
    // Set DDR for all the data pins
    LCD_DATA0_DDR |= (1 << LCD_DATA0_PIN);
    LCD_DATA1_DDR |= (1 << LCD_DATA1_PIN);
    LCD_DATA2_DDR |= (1 << LCD_DATA2_PIN);
    LCD_DATA3_DDR |= (1 << LCD_DATA3_PIN);
    LCD_DATA4_DDR |= (1 << LCD_DATA4_PIN);
    LCD_DATA5_DDR |= (1 << LCD_DATA5_PIN);
    LCD_DATA6_DDR |= (1 << LCD_DATA6_PIN);
    LCD_DATA7_DDR |= (1 << LCD_DATA7_PIN);
#endif

    // SEE PAGE 45/46 OF Hitachi HD44780 DATASHEET FOR INITIALIZATION
    // SPECIFICATION!

    // according to datasheet, we need at least 40ms after power rises
    // above 2.7V before sending commands. Arduino can turn on way before 4.5V
    // so we'll wait 50
    _delay_us(50000);
    // Now we pull both RS and Enable low to begin commands (R/W is wired to
    // ground)
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
    LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);

    // put the LCD into 4 bit or 8 bit mode
    if (LCD_USING_4PIN_MODE) {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        lcd_write4bits(0b0111);
        _delay_us(4500); // wait min 4.1ms

        // second try
        lcd_write4bits(0b0111);
        _delay_us(4500); // wait min 4.1ms

        // third go!
        lcd_write4bits(0b0111);
        _delay_us(150);

        // finally, set to 4-bit interface
        lcd_write4bits(0b0010);
    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
        _delay_us(4500); // wait more than 4.1ms

        // second try
        lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
        _delay_us(150);

        // third go
        lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
    }

    // finally, set # lines, font size, etc.
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);

    // turn the display on with no cursor or blinking default
    _lcd_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcd_display();

    // clear it off
    lcd_clear();

    // Initialize to default text direction (for romance languages)
    _lcd_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

/********** high level commands, for the user! */
void lcd_write_string(uint8_t x, uint8_t y, char string[]) {
    lcd_setCursor(x, y);
    for (int i = 0; string[i] != '\0'; ++i) {
        lcd_write(string[i]);
    }
}

void lcd_write_char(uint8_t x, uint8_t y, char val) {
    lcd_setCursor(x, y);
    lcd_write(val);
}

void lcd_clear(void) {
    lcd_command(LCD_CLEARDISPLAY); // clear display, set cursor position to zero
    _delay_us(2000);               // this command takes a long time!
}

void lcd_home(void) {
    lcd_command(LCD_RETURNHOME); // set cursor position to zero
    _delay_us(2000);             // this command takes a long time!
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    lcd_command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        lcd_write(charmap[i]);
    }
}

void lcd_setCursor(uint8_t col, uint8_t row) {
    if (row >= 2) {
        row = 1;
    }

    lcd_command(LCD_SETDDRAMADDR | (col + row * 0x40));
}

// Turn the display on/off (quickly)
void lcd_noDisplay(void) {
    _lcd_displaycontrol &= ~LCD_DISPLAYON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_display(void) {
    _lcd_displaycontrol |= LCD_DISPLAYON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turns the underline cursor on/off
void lcd_noCursor(void) {
    _lcd_displaycontrol &= ~LCD_CURSORON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_cursor(void) {
    _lcd_displaycontrol |= LCD_CURSORON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_noBlink(void) {
    _lcd_displaycontrol &= ~LCD_BLINKON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_blink(void) {
    _lcd_displaycontrol |= LCD_BLINKON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
    lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void) {
    lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_leftToRight(void) {
    _lcd_displaymode |= LCD_ENTRYLEFT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This is for text that flows Right to Left
void lcd_rightToLeft(void) {
    _lcd_displaymode &= ~LCD_ENTRYLEFT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(void) {
    _lcd_displaymode |= LCD_ENTRYSHIFTINCREMENT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'left justify' text from the cursor
void lcd_noAutoscroll(void) {
    _lcd_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

/*********** mid level commands, for sending data/cmds */

inline void lcd_command(uint8_t value) {
    //
    lcd_send(value, 0);
}

inline size_t lcd_write(uint8_t value) {
    lcd_send(value, 1);
    return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void lcd_send(uint8_t value, uint8_t mode) {
    // RS Pin
    LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
    LCD_RS_PORT |= (!!mode << LCD_RS_PIN);

    if (LCD_USING_4PIN_MODE) {
        lcd_write4bits(value >> 4);
        lcd_write4bits(value);
    } else {
        lcd_write8bits(value);
    }
}

void lcd_pulseEnable(void) {
    // Enable Pin
    LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
    _delay_us(1);
    LCD_ENABLE_PORT |= (1 << LCD_ENABLE_PIN);
    _delay_us(1); // enable pulse must be >450ns
    LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
    _delay_us(100); // commands need > 37us to settle
}

void lcd_write4bits(uint8_t value) {
    // Set each wire one at a time

    LCD_DATA4_PORT &= ~(1 << LCD_DATA4_PIN);
    LCD_DATA4_PORT |= ((value & 1) << LCD_DATA4_PIN);
    value >>= 1;

    LCD_DATA5_PORT &= ~(1 << LCD_DATA5_PIN);
    LCD_DATA5_PORT |= ((value & 1) << LCD_DATA5_PIN);
    value >>= 1;

    LCD_DATA6_PORT &= ~(1 << LCD_DATA6_PIN);
    LCD_DATA6_PORT |= ((value & 1) << LCD_DATA6_PIN);
    value >>= 1;

    LCD_DATA7_PORT &= ~(1 << LCD_DATA7_PIN);
    LCD_DATA7_PORT |= ((value & 1) << LCD_DATA7_PIN);

    lcd_pulseEnable();
}

void lcd_write8bits(uint8_t value) {
    // Set each wire one at a time

#if !LCD_USING_4PIN_MODE
    LCD_DATA0_PORT &= ~(1 << LCD_DATA0_PIN);
    LCD_DATA0_PORT |= ((value & 1) << LCD_DATA0_PIN);
    value >>= 1;

    LCD_DATA1_PORT &= ~(1 << LCD_DATA1_PIN);
    LCD_DATA1_PORT |= ((value & 1) << LCD_DATA1_PIN);
    value >>= 1;

    LCD_DATA2_PORT &= ~(1 << LCD_DATA2_PIN);
    LCD_DATA2_PORT |= ((value & 1) << LCD_DATA2_PIN);
    value >>= 1;

    LCD_DATA3_PORT &= ~(1 << LCD_DATA3_PIN);
    LCD_DATA3_PORT |= ((value & 1) << LCD_DATA3_PIN);
    value >>= 1;

    LCD_DATA4_PORT &= ~(1 << LCD_DATA4_PIN);
    LCD_DATA4_PORT |= ((value & 1) << LCD_DATA4_PIN);
    value >>= 1;

    LCD_DATA5_PORT &= ~(1 << LCD_DATA5_PIN);
    LCD_DATA5_PORT |= ((value & 1) << LCD_DATA5_PIN);
    value >>= 1;

    LCD_DATA6_PORT &= ~(1 << LCD_DATA6_PIN);
    LCD_DATA6_PORT |= ((value & 1) << LCD_DATA6_PIN);
    value >>= 1;

    LCD_DATA7_PORT &= ~(1 << LCD_DATA7_PIN);
    LCD_DATA7_PORT |= ((value & 1) << LCD_DATA7_PIN);

    lcd_pulseEnable();
#endif
}

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT        222222
// PP      PP       AA  AA        RR      RR          TT          22      22
// PP      PP     AA      AA      RR      RR          TT                  22
// PPPPPPPPP      AA      AA      RRRRRRRR            TT                22
// PP             AAAAAAAAAA      RR  RR              TT              22
// PP             AA      AA      RR    RR            TT            22
// PP             AA      AA      RR      RR          TT          2222222222
//
//===============================================================================
//  Part 2 ---- String convertion
//  Source from Topic 10 example1.c
//  https://blackboard.qut.edu.au/bbcswebdav/pid-8394144-dt-content-rid-31401936_1/courses/CAB202_20se1/example1%281%29.c
//===============================================================================

// string convertion functions
void ftoa(float n, char *res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char *str, int len);

// Reverses a string 'str' of length 'len'
void reverse(char *str, int len) {
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
int intToStr(int x, char str[], int d) {
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
void ftoa(float n, char *res, int afterpoint) {
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

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT      3333333333
// PP      PP       AA  AA        RR      RR          TT                33
// PP      PP     AA      AA      RR      RR          TT              33
// PPPPPPPPP      AA      AA      RRRRRRRR            TT                33
// PP             AAAAAAAAAA      RR  RR              TT                  33
// PP             AA      AA      RR    RR            TT          33      33
// PP             AA      AA      RR      RR          TT            333333
//
//===============================================================================
//  Part 3 ---- Serial I/O uart
//  Source from Topic 10 example1.c
//  https://blackboard.qut.edu.au/bbcswebdav/pid-8394144-dt-content-rid-31401936_1/courses/CAB202_20se1/example1%281%29.c
//===============================================================================

// uart definitions
#define BAUD (9600)
#define MYUBRR (F_CPU / 16 / BAUD - 1)

// These buffers may be any size from 2 to 256 bytes.
#define RX_BUFFER_SIZE 64
#define TX_BUFFER_SIZE 64

// uart definitions
unsigned char rx_buf;

static volatile uint8_t tx_buffer[TX_BUFFER_SIZE];
static volatile uint8_t tx_buffer_head;
static volatile uint8_t tx_buffer_tail;
static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint8_t rx_buffer_head;
static volatile uint8_t rx_buffer_tail;

// uart functions
void uart_init(unsigned int ubrr);
void uart_putchar(uint8_t c);
uint8_t uart_getchar(void);
uint8_t uart_available(void);
void uart_putstring(unsigned char *s);
void uart_getLine(unsigned char *buf, uint8_t n);

// PLEASE NOTE THIS VERSION OF UART USES INTERRUPTS

/*  ****** serial uart definitions ************ */
/******************  interrupt based  ********/

// Initialize the UART
void uart_init(unsigned int ubrr) {

    cli();

    UBRR0H = (unsigned char)(ubrr >> 8);
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
    if (i >= TX_BUFFER_SIZE)
        i = 0;
    while (tx_buffer_tail == i)
        ; // wait until space in buffer
    // cli();
    tx_buffer[i] = c;
    tx_buffer_head = i;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0) | (1 << UDRIE0);
    // sei();
}

// Receive a byte
uint8_t uart_getchar(void) {
    uint8_t c, i;

    while (rx_buffer_head == rx_buffer_tail)
        ; // wait for character
    i = rx_buffer_tail + 1;
    if (i >= RX_BUFFER_SIZE)
        i = 0;
    c = rx_buffer[i];
    rx_buffer_tail = i;
    return c;
}

// Transmit a string
void uart_putstring(unsigned char *s) {
    // transmit character until NULL is reached
    while (*s > 0)
        uart_putchar(*s++);
}

// Receive a string
void uart_getLine(unsigned char *buf, uint8_t n) {
    uint8_t bufIdx = 0;
    unsigned char c;

    // while received character is not carriage return
    // and end of buffer has not been reached
    do {
        // receive character
        c = uart_getchar();

        // store character in buffer
        buf[bufIdx++] = c;
    } while ((bufIdx < n) && (c != '\n'));

    // ensure buffer is null terminated
    buf[bufIdx] = 0;
}

uint8_t uart_available(void) {
    uint8_t head, tail;

    head = rx_buffer_head;
    tail = rx_buffer_tail;
    if (head >= tail)
        return head - tail;
    return RX_BUFFER_SIZE + head - tail;
}

// Transmit Interrupt
ISR(USART_UDRE_vect) {
    uint8_t i;

    if (tx_buffer_head == tx_buffer_tail) {
        // buffer is empty, disable transmit interrupt
        UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    } else {
        i = tx_buffer_tail + 1;
        if (i >= TX_BUFFER_SIZE)
            i = 0;
        UDR0 = tx_buffer[i];
        tx_buffer_tail = i;
    }
}

// Receive Interrupt
ISR(USART_RX_vect) {
    uint8_t c, i;

    c = UDR0;
    i = rx_buffer_head + 1;
    if (i >= RX_BUFFER_SIZE)
        i = 0;
    if (i != rx_buffer_tail) {
        rx_buffer[i] = c;
        rx_buffer_head = i;
    }
}

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT            44
// PP      PP       AA  AA        RR      RR          TT              4444
// PP      PP     AA      AA      RR      RR          TT            44  44
// PPPPPPPPP      AA      AA      RRRRRRRR            TT          44    44
// PP             AAAAAAAAAA      RR  RR              TT          4444444444
// PP             AA      AA      RR    RR            TT                44
// PP             AA      AA      RR      RR          TT                44
//
//===============================================================================
//  Part 4 ---- Timer
//===============================================================================

// timer definitions
#define FREQ (16000000.0)
#define PRESCALE (256.0)
volatile int overflow_counter = 0;
double total_time;

void time1_setup(void) {
    //	(a) Initialise Timer 1 in normal mode so that it overflows
    //	with a period of approximately 1.0 seconds.
    //	Hint: use the table you completed in a previous exercise.
    TCCR1A = 0;
    TCCR1B = 4;
    //	(b) Enable timer overflow for Timer 1.
    TIMSK1 = 1;
    //	(c) Turn on interrupts.
    sei();
}

void time1_pause() {
    TIMSK1 = 0;
    cli();
}

ISR(TIMER1_OVF_vect) {
    overflow_counter++;

    total_time = (overflow_counter * 65536.0 + TCNT1) * PRESCALE / FREQ;
}

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT      5555555555
// PP      PP       AA  AA        RR      RR          TT          55
// PP      PP     AA      AA      RR      RR          TT          55555555
// PPPPPPPPP      AA      AA      RRRRRRRR            TT                  55
// PP             AAAAAAAAAA      RR  RR              TT                  55
// PP             AA      AA      RR    RR            TT          55      55
// PP             AA      AA      RR      RR          TT            555555
//
//===============================================================================
//  Part 5 ---- PWM
//  Source from Topic 10 Ams Ex2
//===============================================================================

// void setup_pwm(uint16_t division_factor) {
//     //  Configure the digital I/O pin corresponding to OCR0A for output.
//     //  Modify at most one pin in the DDR.
//     SET_BIT(DDRD, 3);
//     //  Update the value of TCCR0A so that register OC0A will clear on
//     compare
//     //  match.
//     SET_BIT(TCCR0A, 7);
//     CLEAR_BIT(TCCR0A, 6);
//     CLEAR_BIT(TCCR0A, 5);
//     CLEAR_BIT(TCCR0A, 4);
//     //  Update TCCR0B to disable Force Output Compare functionality.
//     CLEAR_BIT(TCCR0B, 6);
//     CLEAR_BIT(TCCR0B, 7);
//     //  Update TCCR0B to ensure that the clock pre-scaler matches the
//     //  designated division factor.
//     CLEAR_BIT(TCCR0B, 0);
//     CLEAR_BIT(TCCR0B, 1);
//     CLEAR_BIT(TCCR0B, 2);
//     if (division_factor == 1) {
//         TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
//     }
//     if (division_factor == 8) {
//         TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
//     }
//     if (division_factor == 64) {
//         TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
//     }
//     if (division_factor == 256) {
//         TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);
//     }
//     if (division_factor == 1024) {
//         TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
//     }

//     //  Update the values of TCCR0A and TCCR0B so that the Waveform
//     Generation
//     //  Mode corresponds to Fast PWM with a Top value equal to 255.
//     TCCR0A |= (1 << WGM01) | (1 << WGM00);
//     CLEAR_BIT(TCCR0B, 3);
// }

// /*
// ||  Complete function write_pwm which assigns a value to Output Compare
// Register 0A.
// ||
// ||  Parameter: duration - an 8-bit unsigned integer which specifies the
// number of
// ||      timer ticks in the duty cycle.
// ||
// ||  Return: Nothing.
// */

// void write_pwm(uint8_t duration) { OCR0A = duration; }

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT          6666
// PP      PP       AA  AA        RR      RR          TT            66
// PP      PP     AA      AA      RR      RR          TT          66
// PPPPPPPPP      AA      AA      RRRRRRRR            TT          66666666
// PP             AAAAAAAAAA      RR  RR              TT          66      66
// PP             AA      AA      RR    RR            TT          66      66
// PP             AA      AA      RR      RR          TT            666666
//
//===============================================================================
//  Part 6 ---- LCD setup
//===============================================================================

// How Chrome Dino run
// left foot in row 1
uint8_t dino_run_1[8] = {0b00011111, 0b00010111, 0b00011000, 0b00011101,
                         0b00011110, 0b00011100, 0b00011000, 0b00010000};

// right foot in row 1
uint8_t dino_run_2[8] = {0b00011111, 0b00010111, 0b00011000, 0b00011100,
                         0b00011110, 0b00011100, 0b00001100, 0b00000110};

// How Chrome Dino jump
// All in row 0
uint8_t dino_jump_top[8] = {0b00011111, 0b00010111, 0b00011000, 0b00011101,
                            0b00011110, 0b00011100, 0b00011011, 0b00000000};

// Half in the row 0
uint8_t dino_jump_half_1[8] = {0b00000000, 0b00000000, 0b00000000, 0b00000000,
                               0b00000000, 0b00000000, 0b00011111, 0b00010111};

// Half in the row 1
uint8_t dino_jump_half_2[8] = {0b00011000, 0b00011101, 0b00011110, 0b00011100,
                               0b00011011, 0b00000000, 0b00000000, 0b00000000};

// tree in the row 1
uint8_t tree[8] = {0b00000011, 0b00011011, 0b00011011, 0b00011011,
                   0b00011011, 0b00011111, 0b00001110, 0b00001110};

// bird in the row 0
uint8_t bird[8] = {0b00010000, 0b00011101, 0b00011111, 0b00011100,
                   0b00010000, 0b00000000, 0b00000000, 0b00000000};

// dino is not moving
uint8_t dino_still[8] = {0b00011111, 0b00010111, 0b00011000, 0b00011101,
                         0b00011110, 0b00011100, 0b00011100, 0b00011110};

// no image
uint8_t no_image[8] = {0b00000000, 0b00000000, 0b00000000, 0b00000000,
                       0b00000000, 0b00000000, 0b00000000, 0b00000000};

void character_init() {
    lcd_createChar(0, dino_run_1);
    lcd_createChar(1, dino_run_2);
    lcd_createChar(2, dino_jump_top);
    lcd_createChar(3, dino_jump_half_1);
    lcd_createChar(4, dino_jump_half_2);
    lcd_createChar(5, tree);
    lcd_createChar(6, bird);
    lcd_createChar(7, dino_still);
    lcd_createChar(8, no_image);
}

// show the personal detail, when turn on
void welcome() {
    lcd_clear();
    lcd_write_string(0, 0, "n10599070");
    lcd_write_string(10, 0, "CAB202");
    lcd_write_string(0, 1, "Zippo He");
    lcd_write_string(11, 1, "Demo");
}

// promote player to start the game
void start_screen() {
    lcd_clear();
    lcd_write_string(0, 0, "Chrome Dino");
    lcd_write_string(0, 1, "Press to Start!!");
}

// Judge which row display the image, 1 is on, 0 is off
_Bool row_0;
_Bool row_1;

// initialize all to nothing(8)
// 0-15 -> column 0-15
// 16 -> next image need to create
int display_row_0[17] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
int display_row_1[17] = {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};

void dino_stilled() {
    // row 1 is on
    row_1 = 1;
    lcd_setCursor(0, 1);
    lcd_write(7);
}

void dino_running() {
    row_1 = 1;
    lcd_setCursor(0, 1);
    lcd_write(0);
    _delay_ms(100);
    hint_check();
    lcd_setCursor(0, 1);
    lcd_write(1);
    _delay_ms(100);
    hint_check();
}

// jump have 4 actions
void dino_jump() {
    // action 1
    // Half in the row 0, half in the row 1
    row_0 = 1;
    row_1 = 1;
    lcd_setCursor(0, 0);
    lcd_write(3);
    lcd_setCursor(0, 1);
    lcd_write(4);

    hint_check();
    _delay_ms(200);
    barrier_draw();

    // action 2
    // all in the row 0
    row_0 = 1;
    row_1 = 0;
    lcd_setCursor(0, 0);
    lcd_write(2);
    lcd_setCursor(0, 1);
    lcd_write(8);

    hint_check();
    _delay_ms(200);
    barrier_draw();
    hint_check();
    _delay_ms(200);
    barrier_draw();
    hint_check();

    // action 3
    // Half in the row 0, half in the row 1
    row_0 = 1;
    row_1 = 1;
    lcd_setCursor(0, 0);
    lcd_write(3);
    lcd_setCursor(0, 1);
    lcd_write(4);

    _delay_ms(200);

    // action 4
    // all in the row 1
    row_0 = 0;
    row_1 = 1;
    lcd_setCursor(0, 0);
    lcd_write(8);
    hint_check();
}

// flag = 0 -> create bird(6)
// flag = 1 -> create tree(5)
// flag = 2, 3, 4 -> create nothing(8)
unsigned int flag;

void random_flag() {
    srand(10599070);
    flag = rand() % 5;
}

void barrier_init() {
    random_flag();

    // create bird(6), flag = 0
    if (flag == 0) {
        // make sure not create a tree
        display_row_1[16] = 8;
        // check if tree and bird is too close
        if (display_row_1[15] == 5) {
            // if too close, don't create bird
            display_row_0[16] == 8;
        } else {
            // if not close, create bird
            display_row_0[16] == 6;
        }
    }

    // create tree(5), flag = 1
    if (flag == 1) {
        // make sure not create a bird
        display_row_0[16] = 8;
        // check if over two tree in a row
        // or bird and tree too close
        if (display_row_1[13] == 5 || display_row_1[14] == 5 ||
            display_row_1[15] == 5 || display_row_0[15] == 6) {
            display_row_1[16] = 8;
        } else {
            display_row_1[16] = 5;
        }
    }

    if (flag == 2 || flag == 3 || flag == 4) {
        display_row_0[16] = 8;
        display_row_1[16] = 8;
    }
}

// draw tree and bird on LCD
void barrier_draw() {
    lcd_clear();
    // initialize tree and bird
    barrier_init();

    // move the barriers to left 1 space
    for (int i = 0; i < 16; i++) {
        display_row_0[i] = display_row_0[i + 1];
        display_row_1[i] = display_row_1[i + 1];
    }

    // draw lcd(0,0)
    if (display_row_0[0] != 8 || row_0 == 0) {
        lcd_setCursor(0, 0);
        lcd_write(display_row_0[0]);
    }

    // draw lcd(0,1)
    if (display_row_1[0] != 8 || row_1 == 0) {
        lcd_setCursor(0, 1);
        lcd_write(display_row_1[0]);
    }

    // draw others
    for (int j = 1; j < 16; j++) {
        lcd_setCursor(j, 0);
        lcd_write(display_row_0[j]);
        lcd_setCursor(j, 1);
        lcd_write(display_row_1[j]);
    }
}

// check if Dino hint the bird and tree
void hint_check() {
    // if hint game end
    if ((display_row_0[0] == 6 && row_0 == 1) ||
        (display_row_1[0] == 5 && row_1 == 1)) {
        // screen blink three times
        lcd_clear();
        //....
    }
}

void pause_screen() {
    time1_pause();

    lcd_clear();
    lcd_write_string(0, 0, "Game is paused!");
    lcd_write_string(0, 1, "Press to resume!");
}

int score = 0;
int last_score = 0;

void end_screen() {
    time1_pause();
    last_score = score;
    overflow_counter = 0;
    score = 0;
    char score_buff[64];
    intToStr(last_score, score_buff, 8);

    lcd_clear();

    lcd_write_string(0, 0, "Score: ");
    lcd_write_string(7, 0, score_buff);
    lcd_write_string(0, 1, "Game Over!");
}

//  Define a parameterless function called switch_1_click which returns an
//  int which indicates that a falling edge has been detected in the signal
//  coming from I/O pin labelled "D10" on an Arduino Uno
//  board. This corresponds to a release event on switch 1.
//
//  The function returns 1 if a falling edge is detected in the signal from the
//  switch, and 0 otherwise.
int switch_1_click() {
    static unsigned char old_state = 0;
    unsigned char curr_state = PINB & (1 << 2);
    int result = (old_state != 0) && (curr_state == 0);
    old_state = curr_state;
    return result;
}

//  Define a parameterless function called switch_2_click which returns an
//  int which indicates that a falling edge has been detected in the signal
//  coming from I/O pin labelled "D11" on an Arduino Uno
//  board. This corresponds to a release event on switch 2.
//
//  The function returns 1 if a falling edge is detected in the signal from the
//  switch, and 0 otherwise.
int switch_2_click() {
    static unsigned char old_state = 0;
    unsigned char curr_state = PINB & (1 << 3);
    int result = (old_state != 0) && (curr_state == 0);
    old_state = curr_state;
    return result;
}

void setup() {
    uart_init(MYUBRR);
    // setup lcd
    SET_BIT(DDRB, 5);
    // setup two switches
    CLEAR_BIT(DDRB, 2); // left switch (S1)
    CLEAR_BIT(DDRB, 3); // right switch (S2)
    lcd_init();
    welcome();
    _delay_ms(1500);
    start_screen();
}

// flag_play control if game runing
// flag_socre control if game over
_Bool flag_play;
_Bool flag_score;

// flag_play == 1 -> running
// flag_play == 0 -> paused or not started
// flag_score == 1 -> count scores
// flag_score == 0 -> pasued or not started
void process() {
    if (flag_play) {
        SET_BIT(PORTB, 5);
        barrier_draw();
        dino_running();
        if (switch_2_click()) {
            dino_jump();
        }
    }

    if (!flag_play) {
        CLEAR_BIT(PORTB, 5);
        if (score != 0 && !flag_score) {
            pause_screen();
        }
        if (score == 0 && last_score != 0 && !flag_score) {
            end_screen();
        }
    }

    if (switch_1_click()) {
        flag_play = 0;
    }

    if (switch_2_click()) {
        flag_play = 1;
        flag_score = 0;
    }

    if (flag_score) {
        score = (int)total_time * 10;
    }
}

//===============================================================================
//
// MM      MM         AA            IIIIII        NNN     NN
// MMMM  MMMM       AA  AA            II          NNNN    NN
// MM  MM  MM     AA      AA          II          NN NN   NN
// MM      MM     AA      AA          II          NN  NN  NN
// MM      MM     AAAAAAAAAA          II          NN   NN NN
// MM      MM     AA      AA          II          NN    NNNN
// MM      MM     AA      AA        IIIIII        NN     NNN
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT
// PP      PP       AA  AA        RR      RR          TT
// PP      PP     AA      AA      RR      RR          TT
// PPPPPPPPP      AA      AA      RRRRRRRR            TT
// PP             AAAAAAAAAA      RR  RR              TT
// PP             AA      AA      RR    RR            TT
// PP             AA      AA      RR      RR          TT
//
//===============================================================================
//  Main Part
//===============================================================================

int main() {
    setup();
    while (1) {
        process();
    }

    return 0;
}
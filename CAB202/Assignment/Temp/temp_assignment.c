#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

//===============================================================================
//                              Table of contents
//-------------------------------------------------------------------------------
//  Part 0 ---- Definitions                                 Line 23
//  Part 1 ---- LCD Library                                 Line 223
//  Part 2 ---- String convertion                           Line 540
//  Part 3 ---- Serial I/O uart                             Line 613
//  Part 4 ---- ADC reader                                  Line 656
//  Part 5 ---- Temperature Sensor                          Line 699
//  Part 6 ---- Button setting                              Line 754
//  Part 7 ---- LCD setup                                   Line 786
//  Part 8 ---- Timer                                       Line 861
//  Main Part                                               Line 885

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT        000000
// PP      PP       AA  AA        RR      RR          TT          00      00
// PP      PP     AA      AA      RR      RR          TT          00    0000
// PPPPPPPPP      AA      AA      RRRRRRRR            TT          00  00  00
// PP             AAAAAAAAAA      RR  RR              TT          0000    00
// PP             AA      AA      RR    RR            TT          00      00
// PP             AA      AA      RR      RR          TT            000000
//
//===============================================================================
//  Part 0 ---- Definitions
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

// string convertion functions
void ftoa(float n, char *res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char *str, int len);

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

// uart definitions
#define BAUD (9600)
#define MYUBRR (F_CPU / 16 / BAUD - 1)

char buffer[100];

// uart functions
void uart_init(unsigned int ubrr);
void uart_put_byte(unsigned char data);
void uart_printf(const char *fmt, ...);

// ADC functions
uint16_t adc_read(uint8_t channel);
void adc_setup();

// temp sensor
double c_temp = 0;
double f_temp = 0;
double remap(double x, double in_min, double in_max, double out_min,
             double out_max);
void temp_process();

// buttom function
uint8_t bit_count = 0;
uint8_t switch_state;

void buttom_debounce();

// setup timer1
#define FREQ (16000000.0)
#define PRESCALE (1024.0)
volatile int overflow_counter = 0;
double time;
void time1_setup();

// main
void setup();
void process();

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
//  This part is used to send message to serial I/O
//===============================================================================

// Initialize the UART
void uart_init(unsigned int ubrr) {

    cli();

    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)(ubrr);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    sei();
}

void uart_put_byte(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))) { /* Wait */
    }
    UDR0 = data;
}

void uart_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    for (int i = 0; buffer[i]; i++) {
        uart_put_byte(buffer[i]);
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
//  Part 4 ---- ADC Reader
//  Source from Topic 10
//===============================================================================

void adc_setup() {
    // INSERT CODE
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel) {

    if (channel == 0)
        ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (0 << MUX2) |
                (0 << MUX1) | (0 << MUX0);
    if (channel == 1)
        ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << MUX3) | (0 << MUX2) |
                (0 << MUX1) | (1 << MUX0);

    // start single conversion
    ADCSRA |= (1 << ADSC);

    // Wait for ADSC bit to clear, signalling conversion complete.
    while (ADCSRA & (1 << ADSC)) {
    }

    // Result now available in ADC

    return (ADC);
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
//  Part 5 ---- Temperature Sensor
//===============================================================================

// Re-maps a number from one range to another. That is, a value of fromLow would
// get mapped
//  to toLow, a value of fromHigh to toHigh, values in-between to values
//  in-between, etc.
// Source from https://www.arduino.cc/reference/en/language/functions/math/map/
double remap(double x, double in_min, double in_max, double out_min,
             double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// c_flag / f_flag = 1, temp >= 0
// c_flag / f_flag = 0, temp < 0
_Bool c_flag;
_Bool f_flag;

void temp_process() {
    // chosse channel 0
    uint16_t pot = adc_read(0);

    // TMP36 temperature range is from -40°C to +125°C
    // Source from TMP36 Datasheet
    // https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf
    c_temp = remap(((pot - 20) * 3.04), 0, 1023, -40, 125);
    f_temp = (c_temp * 9.0) / 5.0 + 32;

    // check the temp positive or nagetive
    if (c_temp < 0) {
        c_flag = 0;
    } else {
        c_flag = 1;
    }

    if (f_temp < 0) {
        f_flag = 0;
    } else {
        f_flag = 1;
    }

    // send massage to serial I/O
    uart_printf("Read Temp successfully\r\n");
}

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
//  Part 6 ---- Button Setting
//===============================================================================

void buttom_debounce() {
    TCCR0A = 0;
    TCCR0B = 4;
    TIMSK0 = 1;
    sei();
}

ISR(TIMER0_OVF_vect) {
    uint8_t mask = 0b00001111;

    bit_count = ((bit_count << 1) & mask) | BIT_VALUE(PINB, 2);
    if (bit_count == mask) {
        switch_state = 1;
    } else if (bit_count == 0) {
        switch_state = 0;
    }
}

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT      7777777777
// PP      PP       AA  AA        RR      RR          TT                  77
// PP      PP     AA      AA      RR      RR          TT                77
// PPPPPPPPP      AA      AA      RRRRRRRR            TT                77
// PP             AAAAAAAAAA      RR  RR              TT              77
// PP             AA      AA      RR    RR            TT              77
// PP             AA      AA      RR      RR          TT              77
//
//===============================================================================
//  Part 7 ---- LCD setup
//===============================================================================

uint8_t o[8] = {0b00000111, 0b00000101, 0b00000111, 0b00000000,
                0b00000000, 0b00000000, 0b00000000, 0b00000000};

void welcome() {
    lcd_clear();
    lcd_write_string(0, 0, "n10599070");
    lcd_write_string(10, 0, "CAB202");
    lcd_write_string(0, 1, "Zippo He");
    lcd_write_string(11, 1, "Demo");
}

void start_screen() {
    lcd_clear();
    lcd_write_string(0, 0, "Temperature");
    lcd_write_string(0, 1, "Monitor");
}

void lcd_menu() {
    char c_temp_buff[64];
    char f_temp_buff[64];
    int c_int = (int)c_temp;
    int f_int = (int)f_temp;

    if (!c_flag) {
        c_int = -c_int;
    }

    if (!f_flag) {
        f_int = -f_int;
    }

    intToStr(c_int, c_temp_buff, 3);
    intToStr(f_int, f_temp_buff, 3);
    lcd_createChar(0, o);
    lcd_clear();

    lcd_write_string(0, 0, "Temperature is: ");

    if (c_flag) {
        lcd_write_string(1, 1, "+");
    } else {
        lcd_write_string(1, 1, "-");
    }

    if (f_flag) {
        lcd_write_string(9, 1, "+");
    } else {
        lcd_write_string(9, 1, "-");
    }

    lcd_write_string(2, 1, c_temp_buff);
    lcd_setCursor(5, 1);
    lcd_write(0);
    lcd_write_string(6, 1, "C");

    lcd_write_string(10, 1, f_temp_buff);
    lcd_setCursor(13, 1);
    lcd_write(0);
    lcd_write_string(14, 1, "F");
}

//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT        888888
// PP      PP       AA  AA        RR      RR          TT          88      88
// PP      PP     AA      AA      RR      RR          TT          88      88
// PPPPPPPPP      AA      AA      RRRRRRRR            TT            888888
// PP             AAAAAAAAAA      RR  RR              TT          88      88
// PP             AA      AA      RR    RR            TT          88      88
// PP             AA      AA      RR      RR          TT            888888
//
//===============================================================================
//  Part 8 ---- Timer
//===============================================================================

void time1_setup() {
    TCCR1A = 0;
    TCCR1B = 4;
    TIMSK1 = 1;

    sei();
}

ISR(TIMER1_OVF_vect) { overflow_counter++; }

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

void setup() {
    uart_init(MYUBRR);
    adc_setup();
    CLEAR_BIT(DDRC, 0);
    // setup led
    SET_BIT(DDRB, 5);
    // setup switch
    CLEAR_BIT(DDRB, 2); // switch (S1)
    lcd_init();
    welcome();
    _delay_ms(1500);
    start_screen();
    _delay_ms(1500);

    buttom_debounce();
}

void process() {
    temp_process();
    lcd_menu();
    SET_BIT(PORTB, 5);
    _delay_ms(500);
    CLEAR_BIT(PORTB, 5);
    _delay_ms(500);
    SET_BIT(PORTB, 5);
    _delay_ms(500);
    CLEAR_BIT(PORTB, 5);
    _delay_ms(500);
    SET_BIT(PORTB, 5);
    _delay_ms(500);
    CLEAR_BIT(PORTB, 5);
    _delay_ms(500);
}

int main() {
    setup();
    time1_setup();
    process();
    while (1) {
        time = (overflow_counter * 65536.0 + TCNT1) * PRESCALE / FREQ;
        int int_time = (int)time;
        if ((int_time - 3 ) % 20 == 0 && (int_time - 3 ) >= 20) {
            process();
            _delay_ms(3000);
        }

        if (switch_state == 1) {
            process();
            _delay_ms(3000);
            switch_state = 0;
        }
    }

    return 1;
}
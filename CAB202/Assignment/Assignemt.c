#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>


//===============================================================================
//                              Table of contents                     
//-------------------------------------------------------------------------------
//  Part 0 ---- Definitions                                 Line  - Line
//  Part 1 ---- LCD Library                                 Line  - Line 
//  Part 2 ---- String convertion                           Line  - Line
//  Part 3 ---- Serial I/O uart                             Line  - Line  
//  Part 4 ---- ADC reader                                  Line  - Line
//  Part 5 ---- PWM                                         Line  - Line
//  Part 6 ---- Temperature Sensorand Light Sensor          Line  - Line
//  Part 7 ---- Button setting                              Line  - Line
//  Part 8 ---- LCD setup                                   Line  - Line
//  Part 9 ---- Timer                                       Line  - Line
//  Main Part                                               Line  - Line



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
//Change the values in these defines to reflect 
//  how you've hooked up the screen
//In 4-pin mode only DATA4:7 are used

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


//DATASHEET: https://s3-us-west-1.amazonaws.com/123d-circuits-datasheets/uploads%2F1431564901240-mni4g6oo875bfbt9-6492779e35179defaf4482c7ac4f9915%2FLCD-WH1602B-TMI.pdf

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


//string convertion functions 
void ftoa(float n, char* res, int afterpoint);
int intToStr(int x, char str[], int d);
void reverse(char* str, int len);


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

//uart functions
void uart_init(unsigned int ubrr);
void uart_putchar(uint8_t c);
uint8_t uart_getchar(void);
uint8_t uart_available(void);
void uart_putstring(unsigned char* s);
void uart_getLine(unsigned char* buf, uint8_t n);

//ADC functions
uint16_t adc_read(uint8_t channel);
void adc_setup(uint8_t division_factor);

//PWM function
void setup_pwm(uint16_t division_factor);
void write_pwm(uint8_t duration);

//temp sensor
int temperature = 0;
long map(long x, long in_min, long in_max, long out_min, long out_max);
void temp_process();
void temp_setup();


//timer definitions
#define FREQ (16000000.0)
#define PRESCALE (256.0)

#define DAYS_PER_WEEK 7
#define MONTHS 12

int Oyear = 2020;
int Omonth = 1;
int Oday = 1;
int Ohour = 0;
int Ominute = 0;
int Osecond = 0;
int Oweekday = 0;
int total_time = 0;
int initi_time = 0;


volatile int overflow_counter = 0;

double add_time = 0.0;

void time1_setup(void);
void time_process();

void time_calculator();
void day_calculator();
_Bool is_leap_year(int year);
int days_of_month(int year, int month);
int days_of_year(int year, int month, int day);
int get_days(int year, int month, int day);
int day_of_week(int year, int month, int day);



//===============================================================================
//  Part 0 ---- Definitions
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================



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

void lcd_init(void){
  //dotsize
  if (LCD_USING_4PIN_MODE){
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
    //Set DDR for all the data pins
    LCD_DATA4_DDR |= (1 << 4);
    LCD_DATA5_DDR |= (1 << 5);
    LCD_DATA6_DDR |= (1 << 6);    
    LCD_DATA7_DDR |= (1 << 7);

  #else
    //Set DDR for all the data pins
    LCD_DATA0_DDR |= (1 << LCD_DATA0_PIN);
    LCD_DATA1_DDR |= (1 << LCD_DATA1_PIN);
    LCD_DATA2_DDR |= (1 << LCD_DATA2_PIN);
    LCD_DATA3_DDR |= (1 << LCD_DATA3_PIN);
    LCD_DATA4_DDR |= (1 << LCD_DATA4_PIN);
    LCD_DATA5_DDR |= (1 << LCD_DATA5_PIN);
    LCD_DATA6_DDR |= (1 << LCD_DATA6_PIN);
    LCD_DATA7_DDR |= (1 << LCD_DATA7_PIN);
  #endif 

  // SEE PAGE 45/46 OF Hitachi HD44780 DATASHEET FOR INITIALIZATION SPECIFICATION!

  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  _delay_us(50000); 
  // Now we pull both RS and Enable low to begin commands (R/W is wired to ground)
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
  
  //put the LCD into 4 bit or 8 bit mode
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
    _delay_us(4500);  // wait more than 4.1ms

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
void lcd_write_string(uint8_t x, uint8_t y, char string[]){
  lcd_setCursor(x,y);
  for(int i=0; string[i]!='\0'; ++i){
    lcd_write(string[i]);
  }
}

void lcd_write_char(uint8_t x, uint8_t y, char val){
  lcd_setCursor(x,y);
  lcd_write(val);
}

void lcd_clear(void){
  lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  _delay_us(2000);  // this command takes a long time!
}

void lcd_home(void){
  lcd_command(LCD_RETURNHOME);  // set cursor position to zero
  _delay_us(2000);  // this command takes a long time!
}


// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  lcd_command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    lcd_write(charmap[i]);
  }
}


void lcd_setCursor(uint8_t col, uint8_t row){
  if ( row >= 2 ) {
    row = 1;
  }
  
  lcd_command(LCD_SETDDRAMADDR | (col + row*0x40));
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
  //RS Pin
  LCD_RS_PORT &= ~(1 << LCD_RS_PIN);
  LCD_RS_PORT |= (!!mode << LCD_RS_PIN);

  if (LCD_USING_4PIN_MODE) {
    lcd_write4bits(value>>4);
    lcd_write4bits(value);
  } else {
    lcd_write8bits(value); 
  } 
}

void lcd_pulseEnable(void) {
  //Enable Pin
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
  _delay_us(1);    
  LCD_ENABLE_PORT |= (1 << LCD_ENABLE_PIN);
  _delay_us(1);    // enable pulse must be >450ns
  LCD_ENABLE_PORT &= ~(1 << LCD_ENABLE_PIN);
  _delay_us(100);   // commands need > 37us to settle
}

void lcd_write4bits(uint8_t value) {
  //Set each wire one at a time

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
  //Set each wire one at a time

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
//  Part 1 ---- LCD Library 
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================




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


//===============================================================================
//  Part 2 ----  String convertion
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================




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



//===============================================================================
//  Part 3 ---- Serial I/O uart
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================




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
//  Source from Topic 10 Ams Ex1
//===============================================================================


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




//===============================================================================
//  Part 4 ---- ADC Reader
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================







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

void setup_pwm( uint16_t division_factor ) {
    //  Configure the digital I/O pin corresponding to OCR0A for output. 
    //  Modify at most one pin in the DDR.
    SET_BIT(DDRD, 3);
    //  Update the value of TCCR0A so that register OC0A will clear on compare 
    //  match. 
    SET_BIT(TCCR0A, 7);
    CLEAR_BIT(TCCR0A, 6);
    CLEAR_BIT(TCCR0A, 5);
    CLEAR_BIT(TCCR0A, 4);
    //  Update TCCR0B to disable Force Output Compare functionality.
    CLEAR_BIT(TCCR0B, 6);
    CLEAR_BIT(TCCR0B, 7);
    //  Update TCCR0B to ensure that the clock pre-scaler matches the 
    //  designated division factor.
    CLEAR_BIT(TCCR0B, 0);
    CLEAR_BIT(TCCR0B, 1);
    CLEAR_BIT(TCCR0B, 2);
    if (division_factor == 1) {
        TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00);
    }
    if (division_factor == 8) {
        TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00);
    }
    if (division_factor == 64) {
        TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00);
    }
    if (division_factor == 256) {
        TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00);
    }
    if (division_factor == 1024) {
        TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
    }

    //  Update the values of TCCR0A and TCCR0B so that the Waveform Generation 
    //  Mode corresponds to Fast PWM with a Top value equal to 255.
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    CLEAR_BIT(TCCR0B, 3);

}

/*
||  Complete function write_pwm which assigns a value to Output Compare Register 0A.
||  
||  Parameter: duration - an 8-bit unsigned integer which specifies the number of 
||      timer ticks in the duty cycle.
||  
||  Return: Nothing.
*/

void write_pwm( uint8_t duration ) {
    OCR0A = duration;
}


//===============================================================================
//  Part 5 ---- PWM
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================







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
//  Part 6 ---- Temperature Sensor
//===============================================================================


// Re-maps a number from one range to another. That is, a value of fromLow would get mapped 
//  to toLow, a value of fromHigh to toHigh, values in-between to values in-between, etc.
// Source from https://www.arduino.cc/reference/en/language/functions/math/map/
long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void temp_setup() {
    adc_setup(128);
}

void temp_process() {
    char temp_buf[64];

    // chosse channel 0
    uint16_t pot = adc_read(0);

    // TMP36 temperature range is from -40°C to +125°C
    // Source from TMP36 Datasheet
    // https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf
    temperature = map(((pot - 20) * 3.04), 0, 1023, -40, 125);

    // convert uint16_t to string
    snprintf(temp_buf, sizeof(temp_buf), "%d", pot);

    //send serial data
    //uart_putstring(temp_buf);
    //uart_putstring("\r\n");
}


//===============================================================================
//  Part 6 ---- Temperature Sensor
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================









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
//  Part 7 ---- Button Setting
//===============================================================================

uint8_t bit_count_1 = 0;
uint8_t bit_count_2 = 0;
uint8_t bit_count_3 = 0;
uint8_t switch_state_1 = 0;
uint8_t switch_state_2 = 0;
uint8_t switch_state_3 = 0;

void buttom_debounce();

void buttom_debounce() {
    TCCR2A = 0;
    TCCR2B = 4;
    TIMSK2 = 1;
    sei();
    CLEAR_BIT(DDRB, 2);
    CLEAR_BIT(DDRB, 3);
    CLEAR_BIT(DDRB, 4);
}

ISR(TIMER2_OVF_vect) {
    uint8_t mask = 0b00001111;

    bit_count_1 = ((bit_count_1 << 1) & mask) | BIT_VALUE(PINB, 2);
    if (bit_count_1 == mask) {
        switch_state_1 = 1;
    } else if (bit_count_1 == 0) {
        switch_state_1 = 0;
    }

    bit_count_2 = ((bit_count_2 << 1) & mask) | BIT_VALUE(PINB, 3);
    if (bit_count_2 == mask) {
        switch_state_2 = 1;
    } else if (bit_count_2 == 0) {
        switch_state_2 = 0;
    }

    bit_count_3 = ((bit_count_3 << 1) & mask) | BIT_VALUE(PINB, 4);
    if (bit_count_3 == mask) {
        switch_state_3 = 1;
    } else if (bit_count_3 == 0) {
        switch_state_3 = 0;
    }
}




//===============================================================================
//  Part 7 ---- Button Setting
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================



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
//  Part 8 ---- LCD setup
//===============================================================================


void setup_lcd();
void set_menu();
void loop_lcd();

void setup_lcd() {
  lcd_init();

  lcd_write_string(0, 0, "n10599070");
  lcd_write_string(0, 1, "Zippo He");
  _delay_ms(1000);

  lcd_clear();
  
  lcd_blink();

}

void loop_lcd() {
  char year_buf[64];
  char month_buf[64];
  char day_buf[64];
  char hour_buf[64]; 
  char minute_buf[64];
  char second_buf[64];
  char temp_buff[64];

  intToStr(Oyear, year_buf, 4);
  intToStr(Omonth, month_buf, 2);
  intToStr(Oday, day_buf, 2);
  intToStr(Ohour, hour_buf, 2);
  intToStr(Ominute, minute_buf, 2);
  intToStr(Osecond, second_buf, 2);
  intToStr(temperature, temp_buff, 3);

  lcd_write_string(0, 0, year_buf);
  lcd_write_string(4, 0, "/");
  lcd_write_string(5, 0, month_buf);
  lcd_write_string(7, 0, "/");
  lcd_write_string(8, 0, day_buf);

  if (Oweekday == 0) {
    lcd_write_string(12, 0, "Sun");
  }
  if (Oweekday == 1) {
    lcd_write_string(12, 0, "Mon");
  }
  if (Oweekday == 2) {
    lcd_write_string(12, 0, "Tues");
  }
  if (Oweekday == 3) {
    lcd_write_string(12, 0, "Wed");
  }
  if (Oweekday == 4) {
    lcd_write_string(12, 0, "Thur");
  }
  if (Oweekday == 5) {
    lcd_write_string(12, 0, "Fri");
  }
  if (Oweekday == 6) {
    lcd_write_string(12, 0, "Sat");
  }

  lcd_write_string(0, 1, hour_buf);
  lcd_write_string(2, 1, "/");
  lcd_write_string(3, 1, minute_buf);
  lcd_write_string(5, 1, "/");
  lcd_write_string(6, 1, second_buf);

  lcd_write_string(12, 1, temp_buff);
}

void set_menu() {

}





//===============================================================================
//  Part 8 ---- LCD setup
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================




//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT        999999
// PP      PP       AA  AA        RR      RR          TT          99      99
// PP      PP     AA      AA      RR      RR          TT          99      99
// PPPPPPPPP      AA      AA      RRRRRRRR            TT            99999999
// PP             AAAAAAAAAA      RR  RR              TT                  99
// PP             AA      AA      RR    RR            TT                99
// PP             AA      AA      RR      RR          TT            9999
//
//===============================================================================
//  Part 9 ---- Timer
//===============================================================================


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



ISR(TIMER1_OVF_vect) {
	overflow_counter ++;

  add_time = (overflow_counter * 65536.0 + TCNT1 ) * PRESCALE  / FREQ;
  int time = (int) add_time;
  total_time = time + initi_time;
  
}


// how many seconds since Year 0
void time_calculator() {
  int pre_day = get_days(Oyear, Omonth, Oday) - 1;
  initi_time = pre_day * 24 * 60 * 60 + Ohour * 60 * 60 + Ominute * 60 + Osecond;
  total_time = initi_time;
}

void day_calculator() {
  Osecond = total_time % 60;
  Ominute = ((total_time - Osecond) / 60) % 60;
  Ohour = (((total_time - Osecond) / 60 - Ominute) / 60) % 24;
  int today = (((total_time - Osecond) / 60 - Ominute) / 60 -Ohour) / 24 + 1;

  int mon[2][13]={{0,31,28,31,30,31,30,31,31,30,31,30,31}, {0,31,29,31,30,31,30,31,31,30,31,30,31}};
  int year = 0;
  int month = 0;
  while (today > mon[is_leap_year(year)][month]) {
    today -= mon[is_leap_year(year)][month] ;
    month++;
    if (month == 13) {
      month -= 12;
      year++;
    }
  }
  Oyear = year;
  Omonth = month;
  Oday = today;
  Oweekday = day_of_week(Oyear, Omonth, Oday);
}

_Bool is_leap_year(int year) {
    return (year % 400 == 0 ) || (year % 4 == 0 && year % 100 != 0);
}

// how many days in that month
int days_of_month(int year, int month) { 
  const int month_days[MONTHS] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(2 == month && is_leap_year(year))
    return 29;
  else
    return month_days[month-1]; 
}

// how many days since the begin of this year
int days_of_year(int year, int month, int day) {
     int i;
     int days = 0;
     for(i = 1; i < month; i++)
     {
         days += days_of_month(year, i);
         
     }
     return days + day;
}

//how many days since Year 0
int get_days(int year, int month, int day) {
    int days = days_of_year(year, month, day);
    int temp = year-1;
    return temp * 365 + temp / 4 - temp / 100 + temp / 400 + days;
}

//which day of that week, the first day of Year 0 is Monday, Sunday is 0
int day_of_week(int year, int month, int day) {
    return get_days(year, month, day) % DAYS_PER_WEEK;
} 







//===============================================================================
//  Part 9 ---- Timer
//===============================================================================
//
// PPPPPPPPP          AA          RRRRRRRR        TTTTTTTTTT        999999
// PP      PP       AA  AA        RR      RR          TT          99      99
// PP      PP     AA      AA      RR      RR          TT          99      99
// PPPPPPPPP      AA      AA      RRRRRRRR            TT            99999999
// PP             AAAAAAAAAA      RR  RR              TT                  99
// PP             AA      AA      RR    RR            TT                99
// PP             AA      AA      RR      RR          TT            9999
//
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================









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
  uart_init(MYUBRR);
  time1_setup();
  temp_setup();
  temp_process();
  light_setup();
  light_process();
  setup_lcd();
  while(1){
    loop_lcd();
    _delay_ms(50);
  }
  return 0;
}

//===============================================================================
//  Main Part
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
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================



//===============================================================================
//
// EEEEEEEEEE      NNN     NN       DDDDDDDD
// EE              NNNN    NN       DD      DD
// EE              NN NN   NN       DD      DDD
// EEEEEEEEEE      NN  NN  NN       DD      DDD
// EE              NN   NN NN       DD      DDD
// EE              NN    NNNN       DD      DD
// EEEEEEEEEE      NN     NNN       DDDDDDDD
//
//===============================================================================

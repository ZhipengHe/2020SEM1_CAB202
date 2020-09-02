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


int temperature = 0;
long map(long x, long in_min, long in_max, long out_min, long out_max);
void temp_process();
void temp_setup();

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
    uart_putstring(temp_buf);
    uart_putstring("\r\n");
}

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

// light function
uint8_t light_process();
void light_setup();

void light_setup() {
    adc_setup(128);
}

uint8_t light_process() {
    char temp_buf[64];

    // choose channel 1
    uint16_t pot = adc_read(1);

    uint8_t duration = map(pot, 0, 1023, 0, 255);
    // convert uint16_t to string
    snprintf(temp_buf, sizeof(temp_buf), "%d", pot);

    //send serial data
    uart_putstring(temp_buf);
    uart_putstring("\r\n");
    return duration;
}

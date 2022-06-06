/**
 * Martin Egli
 * 2021-07-16
 * http://actrl.cz/blog/index.php/2016/arduino-nano-ch340-schematics-and-details/
 * https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial
 *
 * avr-gcc see: /usr/lib/avr
 */

/* - includes --------------------------------------------------------------- */
#include "project.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>

#include "uart.h"

/* - defines ---------------------------------------------------------------- */
// PORTB
#define BV_SCK  _BV(5)
#define BV_MOSI _BV(3)
#define BV_nCS  _BV(4)
#define BV_CD   _BV(2) // unused, keep low, (=0: command, =1: data)

// Timings, Timer1, 16-Bit

/* - public variables ------------------------------------------------------- */
volatile uint8_t global_events;

/* - private functions ------------------------------------------------------ */

#define SCK_low()    PORTB &= ~BV_SCK
#define SCK_high()   PORTB |=  BV_SCK
#define MOSI_low()   PORTB &= ~BV_MOSI
#define MOSI_high()  PORTB |=  BV_MOSI
#define nCS_low()    PORTB &= ~BV_nCS
#define nCS_high()   PORTB |=  BV_nCS

/**
 * data setup clk low, data valid clk rising edge
 */
static void spi_Send(uint8_t dc, uint8_t data) {
    uint8_t mask;
    // 1st D/C 
    SCK_low();
    if(dc == 0) {
        MOSI_low();
    }
    else {
        MOSI_high();
    }
    SCK_high();
    
    */
    // data D7 ... D0
    for(mask = 0x80; mask > 0; mask >>= 1) {
        SCK_low();
        if((data & mask) == 0) {
            MOSI_low();
        }
        else {
            MOSI_high();
        }
        SCK_high();
    }
}

/* - functions -------------------------------------------------------------- */

/**
 * main loop
 */
int main(void) {
    uint8_t local_events;
    // init
/*    DDRB = _BV(5);
    DDRD = _BV(1);
    PORTD = _BV(1);

    uart_Init(UBRR_VAL_57600);
    uart_puts("double slit lin camera\n");
*/
    DDRB  |=  (BV_SCK|BV_MOSI|BV_nCS|BV_CD);
    PORTB &= ~(BV_SCK|BV_MOSI|BV_nCS|BV_CD);
    nCS_high();
    _delay_ms(1000);

    // default oled init
    /*
    // 0xA8, 0x3F: Set MUX Ratio
    nCS_low();
    spi_Send(0, 0xA8);
    spi_Send(0, 0x3F);
    nCS_high();
    // 0xD3, 0x00: Set display offset
    nCS_low();
    spi_Send(0, 0xD3);
    spi_Send(0, 0x00);
    nCS_high();
    // 0x40: set display start line
    nCS_low();
    spi_Send(0, 0x40);
    nCS_high();
    // 0xA0/1: set segment remap
    nCS_low();
    spi_Send(0, 0xA0);
    nCS_high();
    // 0xC0/8: set COM output scan direction
    nCS_low();
    spi_Send(0, 0xC0);
    nCS_high();
    // 0xDA, 0x02: set COM pins hardware config
    nCS_low();
    spi_Send(0, 0xDA);
    spi_Send(0, 0x02);
    nCS_high();
    // 0x81, 0x7F: Set contrast control
    nCS_low();
    spi_Send(0, 0x81);
    spi_Send(0, 0x7F);
    nCS_high();
    // 0xA4: Disable entire display on
    nCS_low();
    spi_Send(0, 0xA4);
    nCS_high();
    // 0xA6: Set Normal Display
    nCS_low();
    spi_Send(0, 0xA6);
    nCS_high();
    // 0xD5, 0x80: Set osc freq
    nCS_low();
    spi_Send(0, 0xD5);
    spi_Send(0, 0x80);
    nCS_high();
    // 0x8D, 0x14: enable charge pump regulator
    nCS_low();
    spi_Send(0, 0x8D);
    spi_Send(0, 0x14);
    nCS_high();
    // 0xAF: display on
    nCS_low();
    spi_Send(0, 0xAF);
    nCS_high();
    */

    
    // event loop
    while(1) {

    	while(1) {
            cli();
            local_events = global_events;
            global_events = 0;
            if(local_events) {
                sei();
                break;
            }
            // wait here for new event
            //_EnterSleepMode(SLEEP_MODE_PWR_DOWN);
            sei();
            while(global_events == 0);
        }
    }
    return(0);
}

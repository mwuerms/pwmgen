/**
 * Martin Egli
 * 2022-06-06
 *
 * avr-gcc see: /usr/lib/avr
 * http://www.nongnu.org/avr-libc/user-manual/using_tools.html
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>

#include "project.h"
#include "mcusleep.h"
#include "buttons.h"
#include "uart.h"

/* - defines ---------------------------------------------------------------- */

/* - public variables ------------------------------------------------------- */
volatile uint8_t global_events;


/**
 * main loop
 */
int main(void) {
    uint8_t local_events;
    // init
    sleep_mode_init();
    use_sleep_mode(ACTIVE);
    
    // event loop
    while(1) {

    	while(1) {
            cli();
            // check if there is a new event
            local_events = global_events;
            global_events = 0;
            if(local_events) {
                sei();
                break;
            }
            // wait here for new event
            enter_sleep_mode();
        }
    }
    return(0);
}

/**
 * Martin Egli
 * 2022-06-06
 *
 * project on github: https://github.com/mwuerms/pwmgen
 * avr-gcc see: /usr/lib/avr
 * http://www.nongnu.org/avr-libc/user-manual/using_tools.html
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include <avr/interrupt.h>
#include <string.h>

#include "project.h"
#include "mcusleep.h"
#include "pwm.h"
#include "buttons.h"
#include "disp.h"
#include "wdt.h"

/* - defines ---------------------------------------------------------------- */

/* - public variables ------------------------------------------------------- */
volatile uint8_t global_events;
volatile uint8_t global_display_events;
volatile uint8_t global_button_events;

/**
 * main loop
 */
int main(void)
{
    uint8_t local_events = 0;
    uint8_t local_display_events = 0;
    uint8_t local_button_events = 0;
    // init
    sleep_mode_init();
    use_sleep_mode(ACTIVE);

    buttons_init();
    disp_init();
    disp_show_info();

    pwm_init();
    sei();

    // event loop
    while (1)
    {
        if (local_events & EV_DISPLAY)
        {
            disp_process_events(EV_DISPLAY, local_display_events);
        }
        if (local_events & EV_BUTTON)
        {
            disp_process_events(EV_BUTTON, local_button_events);
        }

        while (1)
        {
            cli();
            // check if there is a new event
            local_events = global_events;
            if (local_events & EV_DISPLAY)
            {
                local_display_events = global_display_events;
                global_display_events = 0;
            }
            if (local_events & EV_BUTTON)
            {
                local_button_events = global_button_events;
                global_button_events = 0;
            }
            global_events = 0;
            if (local_events)
            {
                sei();
                break;
            }
            // wait here for new event
            enter_sleep_mode();
        }
    }
    return (0);
}

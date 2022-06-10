/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include "disp.h"
#include "disp_draw.h"

void disp_init(void) {
    disp_draw_init();
}

void disp_show_info(void) {
    disp_draw_info();
}

void disp_process_events(uint8_t events, uint8_t btn_events) {
    return;
}

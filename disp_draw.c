/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "project.h"
#include "version.h"
#include "wdt.h"
#include "ssd1306.h"
#include "disp_draw.h"

void disp_draw_init(void) {
    // gpios
    // vars
    uint8_t addr = SSD1306_ADDRESS;
    // init ssd1306
    SSD1306_Init (SSD1306_ADDRESS);
    // clear screen
    SSD1306_ClearScreen ();
    /*
    // draw line
    
    // draw line
    SSD1306_DrawLine (0, MAX_X, 18, 18);
    // set position
    SSD1306_SetPosition (40, 3);
    // draw string
    SSD1306_DrawString ("Linus");
    // set position
    SSD1306_SetPosition (48, 3);
    // draw string
    SSD1306_DrawString ("Iris Egli :-)");
    // set position
    SSD1306_SetPosition (58, 5);
    // draw string
    SSD1306_DrawString ("2022");
    // update
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
    */
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}
void disp_draw_clear(void) {
    SSD1306_ClearScreen ();
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}

struct {
    int8_t  step;
    uint8_t pox_x;
    uint8_t repeate;
} info_ctrl;
void disp_draw_info(void) {
    SSD1306_DrawLine (0, MAX_X, 4, 4);
    SSD1306_SetPosition (7, 1);
    SSD1306_DrawString (PROGRAM_NAME);
    SSD1306_SetPosition (45, 1);
    SSD1306_DrawString ("-");
    SSD1306_SetPosition (52, 1);
    SSD1306_DrawString (AUTHOR_NAME);
    SSD1306_SetPosition (87, 1);
    SSD1306_DrawString ("-");
    SSD1306_SetPosition (94, 1);
    SSD1306_DrawString (PROGRAM_YEAR);

    info_ctrl.step = +2;
    info_ctrl.pox_x = 30;
    info_ctrl.repeate = 2-1;
    SSD1306_DrawLine (10, 20, ROW_TO_Y(3)+8, ROW_TO_Y(3)+8);
    SSD1306_DrawLine (20, 20, ROW_TO_Y(3)+8, ROW_TO_Y(3)+0);
    SSD1306_DrawLine (20, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+0);
    SSD1306_DrawLine (info_ctrl.pox_x, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+8);
    SSD1306_DrawLine (info_ctrl.pox_x, 60, ROW_TO_Y(3)+8, ROW_TO_Y(3)+8);
    SSD1306_DrawLine (60, 60, ROW_TO_Y(3)+8, ROW_TO_Y(3)+0);
    SSD1306_DrawLine (60, 70, ROW_TO_Y(3)+0, ROW_TO_Y(3)+0);

    SSD1306_SetPosition (80, 3);
    SSD1306_DrawString ("f=10kHz");

    SSD1306_DrawLine (0, MAX_X, ROW_TO_Y(5)+3, ROW_TO_Y(5)+3);
    SSD1306_SetPosition (0, 6);
    SSD1306_DrawString (GIT_REPO_URL);
    //SSD1306_SetPosition (0, 6+2);
    //SSD1306_DrawString (GIT_VERSION);

    SSD1306_UpdateScreen (SSD1306_ADDRESS);
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY, EV_DISPLAY_UPDATE_INFO);
}

uint8_t disp_draw_update_info(void) {
    SSD1306_ClearArea (30-1,60-1,ROW_TO_Y(3)+0, ROW_TO_Y(3)+8+1);
    if(info_ctrl.step >= +1) {
        info_ctrl.pox_x += info_ctrl.step;
        if(info_ctrl.pox_x >= 50) {
            // max
            info_ctrl.step = -2;
        }
    }
    else {
        info_ctrl.pox_x += info_ctrl.step;
        if(info_ctrl.pox_x <= 30) {
            // max
            info_ctrl.step = +2;
            if(info_ctrl.repeate == 0) {
                // done here
                wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_2S, EV_DISPLAY, EV_DISPLAY_NEXT);
                return RET_STOP;
            }
            info_ctrl.repeate--;
        }
    }

    SSD1306_DrawLine (20, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+0);
    SSD1306_DrawLine (info_ctrl.pox_x, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+8);
    SSD1306_DrawLine (info_ctrl.pox_x, 60, ROW_TO_Y(3)+8, ROW_TO_Y(3)+8);

    SSD1306_UpdateScreen (SSD1306_ADDRESS);
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY, EV_DISPLAY_UPDATE_INFO);
    return RET_AGAIN;
}

void disp_draw_pwm_setup(void) {
    return;
}
/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "project.h"
#include "version.h"
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
}

void disp_draw_info(void) {
    SSD1306_DrawLine (0, MAX_X, 4, 4);
    SSD1306_SetPosition (7, 1);
    SSD1306_DrawString (PROGRAM_NAME);
    SSD1306_SetPosition (60, 1);
    SSD1306_DrawString (PROGRAM_YEAR);
    SSD1306_DrawLine (0, MAX_X, ROW_TO_Y(2)+3, ROW_TO_Y(2)+3);
    SSD1306_SetPosition (0, 3);
    SSD1306_DrawString (GIT_REPO_URL);
    SSD1306_SetPosition (0, 5);
    SSD1306_DrawString (GIT_VERSION);

    //SSD1306_DrawLine (0, MAX_X, ROW_TO_Y(0)+0, MAX_Y);

    SSD1306_UpdateScreen (SSD1306_ADDRESS);

}

void disp_draw_pwm_setup(void) {
    return;
}
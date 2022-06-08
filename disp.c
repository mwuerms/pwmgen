/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "ssd1306.h"
#include "disp.h"

void disp_init(void) {
    // gpios
    // vars
    uint8_t addr = SSD1306_ADDRESS;

  // init ssd1306
  SSD1306_Init (addr);

  // clear screen
  SSD1306_ClearScreen ();
  // draw line
  SSD1306_DrawLine (0, MAX_X, 4, 4);
  // set position
  SSD1306_SetPosition (7, 1);
  // draw string
  SSD1306_DrawString ("WOOHOOOOO");
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
  SSD1306_UpdateScreen (addr);
}

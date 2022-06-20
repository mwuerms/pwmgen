/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include "disp.h"
#include "disp_draw.h"
#include "project.h"
#include <stdint.h>

void disp_init(void) { disp_draw_init(); }

void disp_show_info(void) { disp_draw_info(); }

uint8_t disp_update_info(void) { return disp_draw_update_info(); }

void disp_show_pwm_setup(void) { disp_draw_pwm_setup(); }

void disp_process_events(uint8_t events, uint8_t detail_events)
{
  if (events & EV_DISPLAY)
  {
    if (detail_events & EV_DISPLAY_NEXT)
    {
      disp_draw_clear();
      disp_draw_pwm_setup();
    }
    if (detail_events & EV_DISPLAY_UPDATE_INFO)
    {
      disp_draw_update_info();
    }
    if (detail_events & EV_DISPLAY_UPDATE_PWM)
    {
      disp_draw_pwm_setup();
    }
  }
  if (events & EV_BUTTON)
  {
    if (detail_events & EV_BUTTON_UPDATE)
    {
      disp_draw_button();
    }
  }
  return;
}

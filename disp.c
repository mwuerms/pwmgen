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

pwm_settings_t pwm_settings = {
    .freq = 10,
    .freq_pos = 0,
    .duty = 500,
    .duty_pos = 0,
    .status = 0,
    .item = 1,
};

void disp_init(void) { disp_draw_init(); }

void disp_show_info(void) { disp_draw_info(); }

uint8_t disp_update_info(void) { return disp_draw_update_info(); }

void disp_show_pwm_setup(void) { disp_draw_pwm_setup(&pwm_settings); }

void disp_process_events(uint8_t events, uint8_t detail_events)
{
  if (events & EV_DISPLAY)
  {
    if (detail_events & EV_DISPLAY_NEXT)
    {
      disp_draw_clear();
      disp_draw_pwm_setup(&pwm_settings);
    }
    if (detail_events & EV_DISPLAY_UPDATE_INFO)
    {
      disp_draw_update_info();
    }
    if (detail_events & EV_DISPLAY_UPDATE_PWM)
    {
      disp_draw_pwm_setup(&pwm_settings);
    }
  }
  if (events & EV_BUTTON)
  {
    // if (detail_events & (EV_BUTTON_BTN_ON_OFF | EV_BUTTON_BTN_WHEEL | EV_BUTTON_WHEEL)
    disp_draw_button();
  }
  return;
}
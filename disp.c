/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "project.h"
#include "disp.h"
#include "disp_draw.h"
#include "buttons.h"

pwm_settings_t pwm_settings = {
    .freq = 6543,
    .freq_pos = ITEM_POS,
    .duty = 789,
    .duty_pos = ITEM_POS,
    .status = 0,
    .menu = MENU_ITEM_FREQ,
};

// - private functions ---------------------------------------------------------
static uint8_t disp_flags;
#define DISP_FLAG_PWM_SETUP_ENABLE _BV(0)
void disp_init_pwm_setup(void)
{
  disp_flags |= DISP_FLAG_PWM_SETUP_ENABLE;
  disp_draw_pwm_setup(&pwm_settings);
}

static void switch_menu(uint8_t next_menu, uint8_t prev_menu)
{
  if (wheel_cnt > 0)
  {
    pwm_settings.menu = next_menu;
  }
  else
  {
    pwm_settings.menu = prev_menu;
  }
  wheel_cnt = 0;
}

void disp_update_pwm_setup(uint8_t button_events)
{
  if ((disp_flags & DISP_FLAG_PWM_SETUP_ENABLE) == 0)
  {
    // not enabled yet, leave
    return;
  }
  if (button_events & EV_BUTTON_BTN_ON)
  {
    if (pwm_settings.status & PWM_STATUS_ON)
      pwm_settings.status &= ~PWM_STATUS_ON;
    else
      pwm_settings.status |= PWM_STATUS_ON;
  }

  switch (pwm_settings.menu)
  {
  case MENU_ITEM_FREQ:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.freq_pos == ITEM_POS)
      {
        switch_menu(MENU_ITEM_DUTY, MENU_ITEM_SWEEP);
      }
    }
    break;
  case MENU_ITEM_DUTY:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.duty_pos == ITEM_POS)
      {
        switch_menu(MENU_ITEM_SWEEP, MENU_ITEM_FREQ);
      }
    }
    break;
  case MENU_ITEM_SWEEP:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.duty_pos == ITEM_POS)
      {
        switch_menu(MENU_ITEM_FREQ, MENU_ITEM_DUTY);
      }
    }
    break;
  }
  // EV_DISPLAY_UPDATE_PWM
  disp_draw_pwm_setup(&pwm_settings);
}

// - public functions ----------------------------------------------------------
void disp_init(void)
{
  disp_flags = 0;
  disp_draw_init();
}

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
      disp_init_pwm_setup();
    }
    if (detail_events & EV_DISPLAY_UPDATE_INFO)
    {
      disp_draw_update_info();
    }
  }
  if (events & EV_BUTTON)
  {
    // if (detail_events & (EV_BUTTON_BTN_ON_OFF | EV_BUTTON_BTN_WHEEL | EV_BUTTON_WHEEL)
    disp_update_pwm_setup(detail_events);
  }
  return;
}

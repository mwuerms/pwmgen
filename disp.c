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
#include "pwm.h"

pwm_settings_t pwm_settings = {
    .freq = 100,
    .freq_pos = MENU_POS,
    .duty = 50,
    .duty_pos = MENU_POS,
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

static const uint8_t freq_next_positions[] = {1, 2, 3, MENU_POS};
static const uint8_t duty_next_positions[] = {1, 2, MENU_POS};
static uint8_t next_position(uint8_t curr_pos, const uint8_t *positions)
{
  if (curr_pos == MENU_POS)
  {
    return 0;
  }
  return positions[curr_pos];
}

void calc_new_frequency(void)
{
  int16_t new_value;
  if (wheel_cnt > 0)
  {
    new_value = pwm_settings.freq + decimal_place[pwm_settings.freq_pos];
  }
  else
  {
    new_value = pwm_settings.freq - decimal_place[pwm_settings.freq_pos];
  }
  wheel_cnt = 0;
  if (new_value > PWM_MAX_FREQUENCY)
    new_value = PWM_MAX_FREQUENCY;
  if (new_value < 0)
    new_value = 0;
  pwm_settings.freq = new_value;
}

void calc_new_duty(void)
{
  int16_t new_value;
  if (wheel_cnt > 0)
  {
    new_value = pwm_settings.duty + decimal_place[pwm_settings.duty_pos];
  }
  else
  {
    new_value = pwm_settings.duty - decimal_place[pwm_settings.duty_pos];
  }
  wheel_cnt = 0;
  if (new_value > PWM_MAX_PULSE_WIDTH)
    new_value = PWM_MAX_PULSE_WIDTH;
  if (new_value < 0)
    new_value = 0;
  pwm_settings.duty = new_value;
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
    {
      pwm_settings.status &= ~PWM_STATUS_ON;
      pwm_stop();
    }
    else
    {
      pwm_settings.status |= PWM_STATUS_ON;
      pwm_start(pwm_settings.freq, pwm_settings.duty);
    }
  }

  switch (pwm_settings.menu)
  {
  case MENU_ITEM_FREQ:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.freq_pos == MENU_POS)
      {
        switch_menu(MENU_ITEM_DUTY, MENU_ITEM_SWEEP);
      }
      else
      {
        calc_new_frequency();
        pwm_start(pwm_settings.freq, pwm_settings.duty);
      }
    }
    if (button_events & EV_BUTTON_BTN_WHEEL)
    {
      pwm_settings.freq_pos = next_position(pwm_settings.freq_pos, freq_next_positions);
    }
    break;
  case MENU_ITEM_DUTY:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.duty_pos == MENU_POS)
      {
        switch_menu(MENU_ITEM_SWEEP, MENU_ITEM_FREQ);
      }
      else
      {
        calc_new_duty();
        pwm_start(pwm_settings.freq, pwm_settings.duty);
      }
    }
    if (button_events & EV_BUTTON_BTN_WHEEL)
    {
      pwm_settings.duty_pos = next_position(pwm_settings.duty_pos, duty_next_positions);
    }
    break;
  case MENU_ITEM_SWEEP:
    if (button_events & EV_BUTTON_WHEEL)
    {
      // any :-) if (pwm_settings.duty_pos == MENU_POS)
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

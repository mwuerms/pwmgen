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
    .duty = 500, // 500 = 50.0
    .status = 0,
    .menu = MENU_ITEM_FREQ,
    .menu_pos = MENU_POS,
    .sweep.mode = SWEEP_MODE_OFF,
    .sweep.duty_start = 450,
    .sweep.duty_stop = 550,
    .sweep.freq_start = 10,
    .sweep.freq_stop = 100,
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
// static const uint8_t sweep_mode_next_positions[] = {MENU_POS};
static uint8_t next_position(uint8_t curr_pos, const uint8_t *positions)
{
  if (curr_pos == MENU_POS)
  {
    return 0;
  }
  return positions[curr_pos];
}

void calc_new_frequency(uint8_t pos)
{
  int16_t new_value;
  if (wheel_cnt > 0)
  {
    new_value = pwm_settings.freq + decimal_place[pos];
  }
  else
  {
    new_value = pwm_settings.freq - decimal_place[pos];
  }
  wheel_cnt = 0;
  if (new_value > PWM_MAX_FREQUENCY)
    new_value = PWM_MAX_FREQUENCY;
  if (new_value < 0)
    new_value = 0;
  pwm_settings.freq = new_value;
}

static uint16_t calc_new_duty(uint16_t duty, uint8_t pos)
{
  int16_t new_value;
  if (wheel_cnt > 0)
  {
    new_value = duty + decimal_place[pos];
  }
  else
  {
    new_value = duty - decimal_place[pos];
  }
  wheel_cnt = 0;
  if (new_value > PWM_MAX_DUTY_CYCLE)
    new_value = PWM_MAX_DUTY_CYCLE;
  if (new_value < 0)
    new_value = 0;
  return new_value;
}

static pwm_sweep_modes_t next_sweep_mode_mode(pwm_sweep_modes_t mode)
{
#warning unimplemented at the moment SWEEP_MODE_FREQ
  if (wheel_cnt > 0)
  {
    // next
    wheel_cnt = 0;
    if (mode >= SWEEP_MODE_DUTY) // SWEEP_MODE_FREQ)
    {
      return SWEEP_MODE_OFF;
    }
    return mode + 1;
  }
  else
  {
    // prev
    wheel_cnt = 0;
    if (mode == SWEEP_MODE_OFF)
    {
      return SWEEP_MODE_DUTY; // SWEEP_MODE_FREQ;
    }
    return mode - 1;
  }
}

static void update_pwm_output(void)
{
  if (pwm_settings.status & PWM_STATUS_ON)
  {
    if (pwm_settings.sweep.mode == SWEEP_MODE_OFF)
    {
      pwm_start(pwm_settings.freq, pwm_settings.duty);
    }
    if (pwm_settings.sweep.mode == SWEEP_MODE_DUTY)
    {
      pwm_start_sweep_duty_cycle(pwm_settings.freq, pwm_settings.sweep.duty_start, pwm_settings.sweep.duty_stop);
    }
  }
  else
  {
    pwm_stop();
  }
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
      update_pwm_output();
    }
    else
    {
      pwm_settings.status |= PWM_STATUS_ON;
      update_pwm_output();
    }
  }

  switch (pwm_settings.menu)
  {
  case MENU_ITEM_FREQ:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.menu_pos == MENU_POS)
      {
        switch_menu(MENU_ITEM_DUTY, MENU_ITEM_SWEEP_DUTY_STOP);
      }
      else
      {
        calc_new_frequency(pwm_settings.menu_pos);
        update_pwm_output();
      }
    }
    if (button_events & EV_BUTTON_BTN_WHEEL)
    {
      pwm_settings.menu_pos = next_position(pwm_settings.menu_pos, freq_next_positions);
    }
    break;
  case MENU_ITEM_DUTY:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.menu_pos == MENU_POS)
      {
        switch_menu(MENU_ITEM_SWEEP_MODE, MENU_ITEM_FREQ);
      }
      else
      {
        pwm_settings.duty = calc_new_duty(pwm_settings.duty, pwm_settings.menu_pos);
        update_pwm_output();
      }
    }
    if (button_events & EV_BUTTON_BTN_WHEEL)
    {
      pwm_settings.menu_pos = next_position(pwm_settings.menu_pos, duty_next_positions);
    }
    break;
  case MENU_ITEM_SWEEP_MODE:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.menu_pos == MENU_POS)
      {
        switch_menu(MENU_ITEM_SWEEP_DUTY_START, MENU_ITEM_DUTY);
      }
      else
      {
        pwm_settings.sweep.mode = next_sweep_mode_mode(pwm_settings.sweep.mode);
        update_pwm_output();
      }
    }
    if (button_events & EV_BUTTON_BTN_WHEEL)
    {

      if (pwm_settings.menu_pos == 0)
      {
        pwm_settings.menu_pos = MENU_POS;
      }
      else
      {
        pwm_settings.menu_pos = 0;
      }
    }
    break;
  case MENU_ITEM_SWEEP_DUTY_START:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.menu_pos == MENU_POS)
      {
        switch_menu(MENU_ITEM_SWEEP_DUTY_STOP, MENU_ITEM_SWEEP_MODE);
      }
      else
      {
        pwm_settings.sweep.duty_start = calc_new_duty(pwm_settings.sweep.duty_start, pwm_settings.menu_pos);
        update_pwm_output();
      }
    }
    if (button_events & EV_BUTTON_BTN_WHEEL)
    {
      pwm_settings.menu_pos = next_position(pwm_settings.menu_pos, duty_next_positions);
    }
    break;
  case MENU_ITEM_SWEEP_DUTY_STOP:
    if (button_events & EV_BUTTON_WHEEL)
    {
      if (pwm_settings.menu_pos == MENU_POS)
      {
        switch_menu(MENU_ITEM_FREQ, MENU_ITEM_SWEEP_DUTY_START);
      }
      else
      {
        pwm_settings.sweep.duty_stop = calc_new_duty(pwm_settings.sweep.duty_stop, pwm_settings.menu_pos);
        update_pwm_output();
      }
    }
    if (button_events & EV_BUTTON_BTN_WHEEL)
    {
      pwm_settings.menu_pos = next_position(pwm_settings.menu_pos, duty_next_positions);
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

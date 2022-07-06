/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include "disp_draw.h"
#include "lcd.h"
#include "project.h"
#include "version.h"
#include "wdt.h"
#include <stdint.h>
#include "buttons.h"

void disp_draw_init(void)
{
  // gpios
  // vars
  lcd_init(LCD_DISP_ON); // init lcd and turn on
  lcd_charMode(DOUBLESIZE);
}
void disp_draw_clear(void)
{
  lcd_clrscr();
  lcd_display();
}
struct
{
  int8_t step;
  uint8_t pox_x;
  uint8_t repeate;
} info_ctrl;
void disp_draw_info(void)
{
  lcd_drawLine(0, ROW_TO_Y(0) + 4, MAX_X, ROW_TO_Y(0) + 4, WHITE);
  lcd_drawLine(0, ROW_TO_Y(0) + 5, MAX_X, ROW_TO_Y(0) + 5, WHITE);
  lcd_charMode(DOUBLESIZE);
  lcd_gotoxy(4, 1);
  lcd_puts(PROGRAM_NAME);
  lcd_drawLine(0, ROW_TO_Y(3) + 3, MAX_X, ROW_TO_Y(3) + 3, WHITE);

  info_ctrl.step = +2;
  info_ctrl.pox_x = 30;
  info_ctrl.repeate = 1;
  /*
    lcd_drawLine(10, ROW_TO_Y(5), 20, ROW_TO_Y(5), WHITE);
    lcd_drawLine(20, ROW_TO_Y(5), 20, ROW_TO_Y(5) - 10, WHITE);
    lcd_drawLine(20, ROW_TO_Y(5) - 10, info_ctrl.pox_x, ROW_TO_Y(5) - 10, WHITE);
    lcd_drawLine(info_ctrl.pox_x, ROW_TO_Y(5) - 10, info_ctrl.pox_x, ROW_TO_Y(5), WHITE);
    lcd_drawLine(info_ctrl.pox_x, ROW_TO_Y(5), 60, ROW_TO_Y(5), WHITE);
    lcd_drawLine(60, ROW_TO_Y(5), 60, ROW_TO_Y(5) - 10, WHITE);
    lcd_drawLine(60, ROW_TO_Y(5) - 10 + 0, 70, ROW_TO_Y(5) - 10, WHITE);
*/
  lcd_drawLine(0, ROW_TO_Y(5) + 4, MAX_X, ROW_TO_Y(5) + 4, WHITE);
  lcd_drawLine(0, ROW_TO_Y(5) + 5, MAX_X, ROW_TO_Y(5) + 5, WHITE);

  lcd_charMode(NORMALSIZE);
  lcd_gotoxy(2, 6);
  lcd_puts(GIT_REPO_URL);
  lcd_gotoxy(2, 7);
  lcd_puts(GIT_VERSION);

  lcd_charMode(NORMALSIZE);
  lcd_gotoxy(13, 4);
  lcd_puts("f=10kHz");

  lcd_display();
  wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY,
                        EV_DISPLAY_UPDATE_INFO);
}

uint8_t disp_draw_update_info(void)
{
  lcd_fillRect(10 - 2, ROW_TO_Y(5) + 2, 70 + 2, ROW_TO_Y(5) - 11, BLACK);
  if (info_ctrl.repeate == 0)
  {
    // do not draw, wait some time
    if (info_ctrl.step)
    {
      info_ctrl.step--;
      wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY,
                            EV_DISPLAY_UPDATE_INFO);
      return RET_AGAIN;
    }
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_2S, EV_DISPLAY,
                          EV_DISPLAY_NEXT);
    return RET_STOP;
  }
  if (info_ctrl.step >= +1)
  {
    info_ctrl.pox_x += info_ctrl.step;
    if (info_ctrl.pox_x >= 50)
    {
      // max
      info_ctrl.step = -2;
    }
  }
  else
  {
    info_ctrl.pox_x += info_ctrl.step;
    if (info_ctrl.pox_x <= 30)
    {
      // max
      info_ctrl.step = +2;
      if (info_ctrl.repeate == 0)
      {
        // done here
        info_ctrl.step = 10;
        wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY,
                              EV_DISPLAY_UPDATE_INFO);
        lcd_display();
        return RET_AGAIN;
      }
      else
      {
        info_ctrl.repeate--;
      }
    }
  }

  lcd_drawLine(10, ROW_TO_Y(5), 20, ROW_TO_Y(5), WHITE);
  lcd_drawLine(20, ROW_TO_Y(5), 20, ROW_TO_Y(5) - 10, WHITE);
  lcd_drawLine(20, ROW_TO_Y(5) - 10, info_ctrl.pox_x, ROW_TO_Y(5) - 10, WHITE);
  lcd_drawLine(info_ctrl.pox_x, ROW_TO_Y(5) - 10, info_ctrl.pox_x, ROW_TO_Y(5), WHITE);
  lcd_drawLine(info_ctrl.pox_x, ROW_TO_Y(5), 60, ROW_TO_Y(5), WHITE);
  lcd_drawLine(60, ROW_TO_Y(5), 60, ROW_TO_Y(5) - 10, WHITE);
  lcd_drawLine(60, ROW_TO_Y(5) - 10 + 0, 70, ROW_TO_Y(5) - 10, WHITE);

  lcd_display();

  wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY,
                        EV_DISPLAY_UPDATE_INFO);
  return RET_AGAIN;
}

// NORMALSIZE: x: 21 chars in 128 pixel, y: 8 lines in 64 pixel
// DOUBLESIZE: x: 10 chars in 128 pixel, y: 4 lines in 64 pixel
uint8_t font_size_width[] = {0, 128 / 21, 128 / 10};
uint8_t font_size_height[] = {0, 64 / 8, 64 / 4};
void disp_draw_mark_position(uint8_t cx, uint8_t cy, uint8_t size)
{
  if (size != DOUBLESIZE)
  {
    size = NORMALSIZE;
  }
  else
  {
    cy = cy / 2;
  }
  uint8_t x0, y0, x1, y1;
  x0 = cx * font_size_width[size];
  x1 = x0 + font_size_width[size];
  y0 = ((cy + 1) * font_size_height[size]) - 1;
  y1 = y0 - 1;
  lcd_drawRect(x0, y0, x1, y1, WHITE);
}

// defined and temporary strings to show values on display
static char pwm_status_text[][4] = {"OFF", "ON "};
static const char sweep_mode_text[][5] = {"OFF ", "DUTY", "FREQ"};

#define LINE_SIZE (23)
static char line[LINE_SIZE + 1]; // +1: '\0' at the end
static char freq_str[8];
static char duty_str[8];
static char sweep_duty_start_str[8];
static char sweep_duty_stop_str[8];

void stringcopyn(char *to, char *from, uint8_t max_size)
{
  uint8_t n;
  for (n = 0; n < max_size; n++)
  {
    if (from[n] == '\0')
    {
      // found end
      break;
    }
    to[n] = from[n];
  }
  to[n] = '\0'; // termination
}

uint8_t stringlen(char *str)
{
  uint8_t n;
  for (n = 0; n < 254; n++)
  {
    if (str[n] == '\0')
    {
      break;
    }
  }
  return n;
}

const uint16_t decimal_place[] = {1, 10, 100, 1000, 10000};
static char *uint16_t_to_str(char *dest, uint16_t u16, uint8_t nb_digits)
{
  uint8_t digit, digit_pos, str_pos, space;
  space = 1;
  str_pos = 0;
  for (digit_pos = nb_digits - 1; digit_pos != 0; digit_pos--)
  {
    digit = (uint8_t)(u16 / decimal_place[digit_pos]);
    if ((digit == 0) && (space == 1))
    {
      // skip
      dest[str_pos++] = ' ';
    }
    else
    {
      space = 0;
      dest[str_pos++] = digit + '0';
      u16 = u16 - (digit * decimal_place[digit_pos]);
    }
  }
  digit = (uint8_t)u16;
  dest[str_pos++] = digit + '0';
  dest[str_pos++] = '\0';
  return dest;
}

static char *int8_t_to_str(char *dest, int8_t i8, uint8_t nb_digits)
{
  uint8_t u8, digit, digit_pos, str_pos, space;
  space = 1;
  str_pos = 0;
  if (i8 < 0)
  {
    dest[str_pos++] = '-'; // neg sign
    u8 = (uint8_t)(i8 * -1);
  }
  else
  {
    dest[str_pos++] = ' '; // no sign
    u8 = (uint8_t)(i8);
  }
  for (digit_pos = nb_digits - 1; digit_pos != 0; digit_pos--)
  {
    digit = (uint8_t)(u8 / decimal_place[digit_pos]);
    if ((digit == 0) && (space == 1))
    {
      // skip
      dest[str_pos++] = ' ';
    }
    else
    {
      space = 0;
      dest[str_pos++] = digit + '0';
      u8 = u8 - (digit * decimal_place[digit_pos]);
    }
  }
  digit = (uint8_t)u8;
  dest[str_pos++] = digit + '0';
  dest[str_pos++] = '\0';
  return dest;
}

static void convert_freq_to_str(char *str, uint16_t freq)
{
  if (freq >= 10000)
  {
    // special case
    stringcopyn(str, "10000", 6);
    return;
  }
  uint16_t_to_str(str, freq, 4);
}

static void convert_duty_to_str(char *str, uint16_t duty)
{
  if (duty >= 1000)
  {
    // special case
    stringcopyn(str, "100.0", 6);
    return;
  }
  uint16_t_to_str(str, duty, 4);
  // add "." to get format "xxx.x", smallest = "  0.0"
  str[5] = '\0';
  str[4] = str[3];
  str[3] = '.';
  if (duty < 10)
  {
    str[2] = '0';
  }
}

static void frequency_to_line(char *num_str)
{
  //"  10000 Hz"
  uint8_t num_len = stringlen(num_str);
  stringcopyn(line, "      0 Hz", LINE_SIZE);
  stringcopyn(&line[2 + 5 - num_len], num_str, LINE_SIZE);
  stringcopyn(&line[2 + 5], " Hz", LINE_SIZE);
}

static uint8_t calc_freq_focus_pos(uint8_t pos)
{
  if (pos == MENU_POS)
  {
    return 0;
  }
  //"  10000 Hz"
  return FREQ_POS_0 - pos;
}

static void frequency_small_to_line(char *num_str)
{
  //"Freq:10000 Hz"
  uint8_t num_len = stringlen(num_str);
  stringcopyn(line, "Freq:    0Hz", LINE_SIZE);
  stringcopyn(&line[5 + 5 - num_len], num_str, LINE_SIZE);
  stringcopyn(&line[5 + 5], " Hz", LINE_SIZE);
}

static void duty_to_line(char *num_str)
{
  // "    0.0 %"
  stringcopyn(line, "    0.0 %", LINE_SIZE);
  stringcopyn(&line[2], num_str, LINE_SIZE);
  stringcopyn(&line[2 + 5], " %", LINE_SIZE);
}

static const uint8_t duty_focus_pos[] = {0, 2, 3};
static uint8_t calc_duty_focus_pos(uint8_t pos)
{
  if (pos == MENU_POS)
  {
    return 0;
  }
  // "    0.0 %", do put focus not on "."
  return DUTY_POS_0 - duty_focus_pos[pos];
}

static void duty_small_to_line(char *num_str)
{
  //"Duty:   0.0%"
  stringcopyn(line, "Duty:   0.0%", LINE_SIZE);
  stringcopyn(&line[6], num_str, LINE_SIZE);
  stringcopyn(&line[6 + 5], " %", LINE_SIZE);
}

static uint8_t calc_sweep_mode_focus_pos(uint8_t pos)
{
  if (pos == MENU_POS)
  {
    return 0;
  }
  // else
  return 2;
}

static void sweep_mode_duty_status_to_line(char *start_str, char *stop_str)
{ //                    "from:100.0%-to:100.0%"
  stringcopyn(&line[0], "from:   .0%-to:   .0%", LINE_SIZE);
  stringcopyn(&line[5], start_str, LINE_SIZE - 5);
  line[10] = '%';
  stringcopyn(&line[15], stop_str, LINE_SIZE - 15);
  line[20] = '%';
}

void disp_draw_pwm_setup(pwm_settings_t *ps)
{
  // preparation convert numbers into strings
  lcd_clear_buffer();
  convert_freq_to_str(freq_str, ps->freq);
  convert_duty_to_str(duty_str, ps->duty);
  convert_duty_to_str(sweep_duty_start_str, ps->sweep.duty_start);
  convert_duty_to_str(sweep_duty_stop_str, ps->sweep.duty_stop);

  // 1st: status line
  lcd_charMode(DOUBLESIZE);
  lcd_gotoxy(0, 0);
  lcd_puts(pwm_status_text[(ps->status & PWM_STATUS_ON)]);
  lcd_charMode(NORMALSIZE);
  lcd_gotoxy(8, 0);
  frequency_small_to_line(freq_str);
  lcd_puts(line);
  lcd_gotoxy(8, 1);
  duty_small_to_line(duty_str);
  lcd_puts(line);
  lcd_gotoxy(0, 2);
  lcd_puts("Sweep mode: ");
  lcd_gotoxy(12, 2);
  lcd_puts(sweep_mode_text[ps->sweep.mode]);
  // show if SWEEP_MODE_DUTY is active
  if (ps->sweep.mode == SWEEP_MODE_DUTY)
  {
    lcd_gotoxy(0, 3);
    sweep_mode_duty_status_to_line(sweep_duty_start_str, sweep_duty_stop_str);
    lcd_puts(line);
  }

  // 2nd: settings
  lcd_charMode(DOUBLESIZE);
  switch (ps->menu)
  {
  case MENU_ITEM_FREQ:
    lcd_gotoxy(0, 4);
    lcd_puts("Frequency");
    lcd_gotoxy(0, 6);
    frequency_to_line(freq_str);
    lcd_puts(line);
    lcd_gotoxy(0, 6);
    lcd_puts(">");
    disp_draw_mark_position(calc_freq_focus_pos(ps->menu_pos), 6, DOUBLESIZE);
    break;
  case MENU_ITEM_DUTY:
    lcd_gotoxy(0, 4);
    lcd_puts("Duty cycle");
    lcd_gotoxy(0, 6);
    duty_to_line(duty_str);
    lcd_puts(line);
    lcd_gotoxy(0, 6);
    lcd_puts(">");
    disp_draw_mark_position(calc_duty_focus_pos(ps->menu_pos), 6, DOUBLESIZE);
    break;
  case MENU_ITEM_SWEEP_MODE:
    lcd_gotoxy(0, 4);
    lcd_puts("Sweep Mode");
    lcd_gotoxy(0, 6);
    stringcopyn(&line[0], ">   ", 8);
    stringcopyn(&line[2], sweep_mode_text[ps->sweep.mode], 8);
    lcd_puts(line);
    disp_draw_mark_position(calc_sweep_mode_focus_pos(ps->menu_pos), 6, DOUBLESIZE);
    break;
  case MENU_ITEM_SWEEP_DUTY_START:
    lcd_gotoxy(0, 4);
    lcd_puts("Swp D Start");
    lcd_gotoxy(0, 6);
    duty_to_line(sweep_duty_start_str);
    lcd_puts(line);
    lcd_gotoxy(0, 6);
    lcd_puts(">");
    disp_draw_mark_position(calc_duty_focus_pos(ps->menu_pos), 6, DOUBLESIZE);
    break;
  case MENU_ITEM_SWEEP_DUTY_STOP:
    lcd_gotoxy(0, 4);
    lcd_puts("Swp D Stop");
    lcd_gotoxy(0, 6);
    duty_to_line(sweep_duty_stop_str);
    lcd_puts(line);
    lcd_gotoxy(0, 6);
    lcd_puts(">");
    disp_draw_mark_position(calc_duty_focus_pos(ps->menu_pos), 6, DOUBLESIZE);
    break;

  default:;
  }
  lcd_display();
}

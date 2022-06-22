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
  info_ctrl.repeate = 2;
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
#define PWM_STATUS_ON _BV(0) // =1: ON, =0: OFF
static char pwm_status_text[][4] = {"OFF", "ON"};

#define LINE_SIZE (23)
static char line[LINE_SIZE + 1]; // +1: '\0' at the end
static char freq_str[8];
static char duty_str[8];

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

static char *uint16_t_to_str(char *dest, uint16_t u16, uint8_t nb_digits)
{
  uint16_t divisor[] = {1, 10, 100, 1000, 10000};
  uint8_t digit, digit_pos, str_pos, space;
  space = 1;
  str_pos = 0;
  for (digit_pos = nb_digits - 1; digit_pos != 0; digit_pos--)
  {
    digit = (uint8_t)(u16 / divisor[digit_pos]);
    if ((digit == 0) && (space == 1))
    {
      // skip
      dest[str_pos++] = ' ';
      continue;
    }
    else
    {
      space = 0;
      dest[str_pos++] = digit + '0';
      u16 = u16 - (digit * divisor[digit_pos]);
    }
  }
  digit = (uint8_t)u16;
  dest[str_pos++] = digit + '0';
  dest[str_pos++] = '\0';
  return dest;
}

static char *int8_t_to_str(char *dest, int8_t i8, uint8_t nb_digits)
{
  uint16_t divisor[] = {1, 10, 100};
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
    digit = (uint8_t)(u8 / divisor[digit_pos]);
    if ((digit == 0) && (space == 1))
    {
      // skip
      dest[str_pos++] = ' ';
      continue;
    }
    else
    {
      space = 0;
      dest[str_pos++] = digit + '0';
      u8 = u8 - (digit * divisor[digit_pos]);
    }
  }
  digit = (uint8_t)u8;
  dest[str_pos++] = digit + '0';
  dest[str_pos++] = '\0';
  return dest;
}

convert_freq_to_str(uint16_t freq)
{
  if (freq == 10000)
  {
    // special case
    stringcopyn(freq_str, "10000", 6);
  }
  uint16_t_to_str(freq_str, freq, 4);
}

convert_duty_to_str(uint16_t duty)
{
  if (duty == 1000)
  {
    // special case
    stringcopyn(duty_str, "1000", 6);
  }
  uint16_t_to_str(duty_str, duty, 4);
}

static void frequency_to_line(char *num_str)
{
  stringcopyn(line, "      0 Hz", LINE_SIZE);
  stringcopyn(&line[3], num_str, LINE_SIZE);
  stringcopyn(&line[3 + 4], " Hz", LINE_SIZE);
}

static void frequency_small_to_line(char *num_str)
{
  stringcopyn(line, "Freq:    0Hz", LINE_SIZE);
  stringcopyn(&line[6], num_str, LINE_SIZE);
  stringcopyn(&line[6 + 4], " Hz", LINE_SIZE);
}

static void duty_to_line(char *num_str)
{
  stringcopyn(line, "    0.0 %", LINE_SIZE);
  stringcopyn(&line[3], num_str, LINE_SIZE);
  stringcopyn(&line[3 + 2], ".", LINE_SIZE);
  stringcopyn(&line[3 + 3], &num_str[2], LINE_SIZE);
  stringcopyn(&line[3 + 4], " %", LINE_SIZE);
}

static void duty_small_to_line(char *num_str)
{
  stringcopyn(line, "Duty:   0.0%", LINE_SIZE);
  stringcopyn(&line[6], num_str, LINE_SIZE);
  stringcopyn(&line[6 + 2], ".", LINE_SIZE);
  stringcopyn(&line[6 + 3], &num_str[2], LINE_SIZE);
  stringcopyn(&line[6 + 4], " %", LINE_SIZE);
}

void disp_draw_pwm_setup(pwm_settings_t *ps)
{
  // no need to clear display when only showing text because of "footprint" of the characters?
  // "footprint": eg. 8x8 pixel containing whole character AND background
  //              -> it will draw over existing pixels

  // preparation convert numbers into strings

  duty_str[8];

  convert_freq_to_str(ps->freq); // result in char freq_str[]
  convert_duty_to_str(ps->duty); // result in char duty_str[]

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

  // 2nd: settings
  lcd_charMode(DOUBLESIZE);
  switch (ps->item)
  {
  case ITEM_FREQ:
    lcd_gotoxy(0, 2);
    lcd_puts("Frequency");
    lcd_gotoxy(0, 4);
    frequency_to_line(freq_str);
    lcd_puts(line);
    lcd_gotoxy(0, 6);
    lcd_puts("Duty cycle");
    lcd_gotoxy(0, 4);
    lcd_puts(">");
    disp_draw_mark_position((FREQ_POS_0 - ps->freq_pos), 4, DOUBLESIZE);
    break;
  case ITEM_DUTY:
  default:
    ps->item = ITEM_DUTY;
    lcd_gotoxy(0, 2);
    lcd_puts("Duty cycle");
    lcd_gotoxy(0, 4);
    duty_to_line(duty_str);
    lcd_puts(line);
    lcd_gotoxy(0, 6);
    lcd_puts("Frequency");
    lcd_gotoxy(0, 4);
    lcd_puts(">");
    disp_draw_mark_position((DUTY_POS_0 - ps->duty_pos), 4, DOUBLESIZE);
    break;
  }
  lcd_display();
}

void disp_draw_button(void)
{
  uint8_t b0, b1;
  int8_t w0;
  char *res;
  b0 = button_on_cnt;
  b1 = button_wheel_cnt;
  w0 = wheel_cnt;

  disp_draw_clear();
  lcd_charMode(NORMALSIZE);
  lcd_gotoxy(0, 0);
  lcd_puts("Buttons");

  lcd_gotoxy(1, 1);
  lcd_puts("PIN_BUTTON_ON_OFF");
  lcd_gotoxy(2, 2);
  res = int8_t_to_str(line, b0, 3);
  lcd_puts(res);

  lcd_gotoxy(1, 3);
  lcd_puts("PIN_BUTTON_WHEEL");
  lcd_gotoxy(2, 4);
  res = int8_t_to_str(line, b1, 3);
  lcd_puts(res);

  lcd_gotoxy(1, 5);
  lcd_puts("PIN_WHEEL");
  lcd_gotoxy(2, 6);
  res = int8_t_to_str(line, w0, 3);
  lcd_puts(res);

  lcd_display();
}

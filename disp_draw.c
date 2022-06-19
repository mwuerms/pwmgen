/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include "disp_draw.h"
#include "disp_draw_elements.h"
#include "lcd.h"
#include "project.h"
#include "version.h"
#include "wdt.h"
#include <stdint.h>

void disp_draw_init(void) {
  // gpios
  // vars
  lcd_init(LCD_DISP_ON); // init lcd and turn on
  lcd_charMode(DOUBLESIZE);
}
void disp_draw_clear(void) {
  lcd_clrscr();
  lcd_display();
}
#define TEXT_INFO "INFO InFo"
struct {
  int8_t step;
  uint8_t pox_x;
  uint8_t repeate;
} info_ctrl;
void disp_draw_info(void) {
  // lcd_charMode(DOUBLESIZE);
  lcd_puts("Hello World");     // put string from RAM to display (TEXTMODE) or
                               // buffer (GRAPHICMODE)
  lcd_gotoxy(0, 2);            // set cursor to first column at line 3
  lcd_puts_p(PSTR(TEXT_INFO)); // puts string form flash to display (TEXTMODE)
                               // or buffer (GRAPHICMODE)
  lcd_drawLine(0, 4, MAX_X, 4, WHITE);
  /*SSD1306_SetPosition (7, 1);
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
  SSD1306_DrawLine (info_ctrl.pox_x, info_ctrl.pox_x, ROW_TO_Y(3)+0,
  ROW_TO_Y(3)+8); SSD1306_DrawLine (info_ctrl.pox_x, 60, ROW_TO_Y(3)+8,
  ROW_TO_Y(3)+8); SSD1306_DrawLine (60, 60, ROW_TO_Y(3)+8, ROW_TO_Y(3)+0);
  SSD1306_DrawLine (60, 70, ROW_TO_Y(3)+0, ROW_TO_Y(3)+0);

  SSD1306_SetPosition (80, 3);
  SSD1306_DrawString ("f=10kHz");

  SSD1306_DrawLine (0, MAX_X, ROW_TO_Y(5)+3, ROW_TO_Y(5)+3);
  SSD1306_SetPosition (0, 6);
  SSD1306_DrawString (GIT_REPO_URL);
  //SSD1306_SetPosition (0, 6+2);
  //SSD1306_DrawString (GIT_VERSION);*/

  lcd_display();
  wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY,
                        EV_DISPLAY_UPDATE_INFO);
}

uint8_t disp_draw_update_info(void) {
  /*SSD1306_ClearArea (30-1,60-1,ROW_TO_Y(3)+0, ROW_TO_Y(3)+8+1);
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
              wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_2S, EV_DISPLAY,
  EV_DISPLAY_NEXT); return RET_STOP;
          }
          info_ctrl.repeate--;
      }
  }

  SSD1306_DrawLine (20, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+0);
  SSD1306_DrawLine (info_ctrl.pox_x, info_ctrl.pox_x, ROW_TO_Y(3)+0,
  ROW_TO_Y(3)+8); SSD1306_DrawLine (info_ctrl.pox_x, 60, ROW_TO_Y(3)+8,
  ROW_TO_Y(3)+8);

  SSD1306_UpdateScreen (SSD1306_ADDRESS);
  wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY,
  EV_DISPLAY_UPDATE_INFO); return RET_AGAIN;
  */
}

// NORMALSIZE: x: 21 chars in 128 pixel, y: 8 lines in 64 pixel
// DOUBLESIZE: x: 10 chars in 128 pixel, y: 4 lines in 64 pixel
uint8_t font_size_width[] = {0, 128 / 21, 128 / 10};
uint8_t font_size_height[] = {0, 64 / 8, 64 / 4};
void disp_draw_mark_position(uint8_t cx, uint8_t cy, uint8_t size) {
  if (size != DOUBLESIZE) {
    size = NORMALSIZE;
  } else {
    cy = cy / 2;
  }
  uint8_t x0, y0, x1, y1;
  x0 = cx * font_size_width[size];
  x1 = x0 + font_size_width[size];
  y0 = ((cy + 1) * font_size_height[size]) - 1;
  y1 = y0 - 1;
  lcd_drawRect(x0, y0, x1, y1, WHITE);
  // lcd_drawRect(7*8-2, 4*8, 8*8+3, 4*8-1, WHITE);
}

struct {
  uint16_t freq;    // 0 ... 10000  (0 ... 10000 Hz)
  uint8_t freq_pos; // 0 ... 3 (1, 10, 100, 1000)
  uint16_t duty;    // 0 ... 1000 (0.0 ... 100.0 %)
  uint8_t duty_pos; // 0, 2, 3 (0.1, 1.0, 10.0)
} pwm_settings = {
    .freq = 10,
    .freq_pos = 0,
    .duty = 500,
    .duty_pos = 0,
};
#define FREQ_POS_0 (5)
#define DUTY_POS_0 (5)

#define LINE_SIZE (23)
static char line[LINE_SIZE + 1]; // +1: '\0' at the end

void stringcopyn(char *to, char *from, uint8_t max_size) {
  uint8_t n;
  for (n = 0; n < max_size; n++) {
    if (from[n] == '\0') {
      // found end
      break;
    }
    to[n] = from[n];
  }
  to[n] = '\0'; // termination
}

static freq_to_line(void) {
  uint16_t divisor[] = {1, 10, 100, 1000};
  uint16_t f;
  uint8_t digit, pos, space;
  if (pwm_settings.freq == 10000) {
    // special case
    stringcopyn(line, " 10000 Hz", LINE_SIZE);
    return;
  }
  stringcopyn(line, "     0 Hz", LINE_SIZE);
  f = pwm_settings.freq;
  space = 1;
  for (pos = 3; pos != 0; pos--) {
    digit = (uint8_t)(f / divisor[pos]);
    if ((digit == 0) && (space == 1)) {
      // skip
      continue;
    } else {
      space = 0;
      line[FREQ_POS_0 - pos] = digit + '0';
      f = f - (digit * divisor[pos]);
    }
  }
  digit = (uint8_t)f;
  line[FREQ_POS_0 - pos] = digit + '0';
}

static duty_to_line(void) {
  uint16_t divisor[] = {1, 10, 100};
  uint16_t d;
  uint8_t digit, pos, space;
  if (pwm_settings.duty == 1000) {
    // special case
    stringcopyn(line, " 100.0 %", LINE_SIZE);
    return;
  }
  stringcopyn(line, "   0.0 %", LINE_SIZE);
  d = pwm_settings.duty;
  space = 1;
  for (pos = 2; pos != 0; pos--) {
    digit = (uint8_t)(d / divisor[pos]);
    if ((digit == 0) && (space == 1)) {
      // skip
      continue;
    } else {
      space = 0;
      line[DUTY_POS_0 - pos - 1] = digit + '0'; // -1: '.' in 0.0
      d = d - (digit * divisor[pos]);
    }
  }
  digit = (uint8_t)d;
  line[DUTY_POS_0] = digit + '0';
}

void disp_draw_pwm_setup(void) {
  // text 1st
  lcd_charMode(DOUBLESIZE);
  lcd_gotoxy(0, 0);
  lcd_puts("Frequency");

  lcd_gotoxy(0, 2);
  // lcd_puts(" 10000 Hz");
  pwm_settings.freq = 9020;
  freq_to_line();
  lcd_puts(line);

  lcd_gotoxy(0, 4);
  lcd_puts("Duty cycle");

  lcd_gotoxy(0, 6);
  // lcd_puts(" 100.0 %");
  pwm_settings.duty = 902;
  duty_to_line();
  lcd_puts(line);

  lcd_gotoxy(0, 2);
  lcd_puts(">");

  // lcd_drawRect(7*8-2, 4*8, 8*8+3, 4*8-1, WHITE);
  // disp_draw_mark_position(5, 4, DOUBLESIZE);
  disp_draw_mark_position((FREQ_POS_0 - pwm_settings.freq_pos), 2, DOUBLESIZE);
  disp_draw_mark_position((DUTY_POS_0 - pwm_settings.duty_pos), 6, DOUBLESIZE);

  // testing
  /*lcd_gotoxy(0, 0);
  lcd_puts("0123456789ABCDEF");
  disp_draw_mark_position(0, 0, DOUBLESIZE);
  disp_draw_mark_position(2, 0, DOUBLESIZE);
  disp_draw_mark_position(4, 0, DOUBLESIZE);
  disp_draw_mark_position(6, 0, DOUBLESIZE);
  disp_draw_mark_position(8, 0, DOUBLESIZE);
*/
  lcd_display();
}
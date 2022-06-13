/**
 * Martin Egli
 * 2022-06-06
 *
 */

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "project.h"
#include "version.h"
#include "wdt.h"
#include "ssd1306.h"
#include "disp_draw.h"
#include "disp_draw_elements.h"

frame_t drw_frm;
void disp_draw_element_frame(frame_t *f) {
    uint8_t x1, x2, y1, y2;
    x1 = f->pos.x;
    x2 = x1 + f->size.x;
    y1 = f->pos.y;
    if(f->size.y >= y1) {
        y2 = 0;
    }
    else {
        y2 = y1 - f->size.y;
    }
    SSD1306_DrawLine(x1+1, x2-1, y1, y1);
    SSD1306_DrawLine(x1+1, x2-1, y2, y2);
    SSD1306_DrawLine(x1, x1, y1-1, y2+1);
    SSD1306_DrawLine(x2, x2, y1-1, y2+1);
}

inline void disp_draw_element_inverse_area(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2) {
    SSD1306_InverseArea(x1, x2, y1, y2);
}

void disp_draw_element_inverse_inside_frame(frame_t *f) {
    uint8_t x1, x2, y1, y2;
    x1 = f->pos.x;
    x2 = x1 + f->size.x;
    y1 = f->pos.y;
    if(f->size.y >= y1) {
        y2 = 0;
    }
    else {
        y2 = y1 - f->size.y;
    }
    SSD1306_InverseArea(x1+2, x2-1, y1-1, y2+2);
}

void disp_draw_element_inverse_text_position(uint8_t x, uint8_t y, uint8_t cursor_pos) {
    uint8_t x2, y2;
    // CHARS_COLS_LENGTH = 5
    x += cursor_pos*(5+1);
    x2 = x + (5+1);
    y2 = y - 9;
    disp_draw_element_inverse_area(x, x2, y, y2);
}

void disp_draw_element_text_label(text_label_t *el) {
    SSD1306_SetPosition (el->pos.x, el->pos.y);
    SSD1306_DrawString (el->text);
    
    if(el->options & TEXT_LABEL_OPT_FOCUS)
        disp_draw_element_frame(&(el->frame));
    if(el->options & TEXT_LABEL_OPT_ACTIVE)
        disp_draw_element_inverse_inside_frame(&(el->frame));

    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}

void disp_draw_element_input_2row(input_2row_t *el) {
    SSD1306_SetPosition (el->pos0.x, el->pos0.y);
    SSD1306_DrawString (el->text0);
    SSD1306_SetPosition (el->pos1.x, el->pos1.y);
    SSD1306_DrawString (el->text1);
    if(el->options & TEXT_LABEL_OPT_FOCUS) {
        disp_draw_element_frame(&(el->frame));
        disp_draw_element_inverse_text_position(el->pos0.x, ROW_TO_Y(el->pos0.y), el->cursor_pos0);
    }
    //if(el->options & TEXT_LABEL_OPT_ACTIVE)
    //    disp_draw_element_inverse_inside_frame(&(el->frame));
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}

void disp_draw_element_pwm_graph(pwm_graph_t *el) {
    uint8_t x1, x2, y1, y2, pwm_wid, pedge;
    uint16_t pwm_duty_edge;
    x1 = el->pos.x;
    x2 = x1 + el->size.x;
    y1 = el->pos.y;
    y2 = y1 - el->size.y;
    pwm_wid = el->size.x;// - 2*PWM_PIXEL_BOARDER);
    pwm_duty_edge = (pwm_wid * el->duty) / 100;
    pedge = (uint8_t)pwm_duty_edge;

    SSD1306_ClearArea(x1, x2, y1+1, y2);
    disp_draw_element_frame(&(el)->frame);

    SSD1306_DrawLine(x1, x1, y1, y2);
    // duty
    SSD1306_DrawLine(x1, x1+pedge, y2, y2);
    SSD1306_DrawLine(x1+pedge, x1+pedge, y1, y2);
    SSD1306_DrawLine(x1+pedge, x2, y1, y1);
    // 2nd edge
    SSD1306_DrawLine(x2, x2, y1, y2);  
    
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}

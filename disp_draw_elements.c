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

/*void disp_draw_element_frame_row(uint8_t pos_x0, uint8_t pos_yrow0, uint8_t offs_x, uint8_t offs_y, uint8_t size_x, uint8_t size_y) {
    uint8_t x1, x2, y1, y2;

    x1 = pos_x0 + offs_x;
    x2 = x1 + size_x;
    if(pos_yrow0 == 0) {
        // cannot drawSSD1306_DrawLine(x1+1, x2-1, y1, y1);
        y1 = 0;
        y2 = y1 + size_y-2;
        SSD1306_DrawLine(x1+1, x2-1, y2, y2);
        SSD1306_DrawLine(x1, x1, y1+1, y2-1);
        SSD1306_DrawLine(x2, x2, y1+1, y2-1);
    }
    else {
        y1 = ROW_TO_Y(pos_yrow0) + offs_y;
        y2 = y1 + size_y;
        SSD1306_DrawLine(x1+1, x2-1, y1, y1);
        SSD1306_DrawLine(x1+1, x2-1, y2, y2);
        SSD1306_DrawLine(x1, x1, y1+1, y2-1);
        SSD1306_DrawLine(x2, x2, y1+1, y2-1);
    }
}*/

void disp_draw_element_inverse(uint8_t pos_x0, uint8_t pos_yrow0, uint8_t offs_x, uint8_t offs_y, uint8_t size_x, uint8_t size_y) {
    return;
}

void disp_draw_element_text_label(text_label_t *tl) {
    SSD1306_SetPosition (tl->pos.x, tl->pos.y);
    SSD1306_DrawString (tl->text);
//    .options = TEXT_LABEL_OPT_FOCUS | TEXT_LABEL_OPT_ACTIVE,
    //if(tl->options & TEXT_LABEL_OPT_FOCUS)
    drw_frm.pos.x = tl->frame.pos.x;
    drw_frm.pos.y = tl->frame.pos.y;
    drw_frm.size.x = tl->frame.size.x;
    drw_frm.size.y = tl->frame.size.y;
    disp_draw_element_frame(&drw_frm);
    //disp_draw_element_frame(&(tl->frame));
    //if(tl->options & TEXT_LABEL_OPT_ACTIVE)
        //disp_draw_element_inverse(tl->pos.x, tl->pos.y, tl->frame_offset.x, tl->frame_offset.y, tl->frame_size.x, tl->frame_size.y);

    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}

void disp_draw_element_pwm_graph(pwm_graph_t *pg) {
    uint8_t x1, x2, y1, y2, pwm_wid, pedge;
    uint16_t pwm_duty_edge;
    x1 = pg->pos.x;
    x2 = x1 + pg->size.x;
    y1 = pg->pos.y;
    y2 = y1 - pg->size.y;
    pwm_wid = pg->size.x;// - 2*PWM_PIXEL_BOARDER);
    pwm_duty_edge = (pwm_wid * pg->duty) / 100;
    pedge = (uint8_t)pwm_duty_edge;

    SSD1306_ClearArea(x1, x2, y1+1, y2);
    disp_draw_element_frame(&(pg)->frame);
/*
    // 1st edge
    SSD1306_DrawLine(x1+PWM_PIXEL_BOARDER, x1+PWM_PIXEL_BOARDER, y1-PWM_PIXEL_BOARDER, y2+PWM_PIXEL_BOARDER);
    // duty
    SSD1306_DrawLine(x1+PWM_PIXEL_BOARDER, x1+PWM_PIXEL_BOARDER+pwm_duty_edge, y2+PWM_PIXEL_BOARDER, y2+PWM_PIXEL_BOARDER);
    SSD1306_DrawLine(x1+PWM_PIXEL_BOARDER+pwm_duty_edge, x1+PWM_PIXEL_BOARDER+pwm_duty_edge, y1-PWM_PIXEL_BOARDER, y2+PWM_PIXEL_BOARDER);
    SSD1306_DrawLine(x1+PWM_PIXEL_BOARDER+pwm_duty_edge, x2-PWM_PIXEL_BOARDER, y1-PWM_PIXEL_BOARDER, y1-PWM_PIXEL_BOARDER);
    // 2nd edge
    SSD1306_DrawLine(x2-PWM_PIXEL_BOARDER, x2-PWM_PIXEL_BOARDER, y1-PWM_PIXEL_BOARDER, y2+PWM_PIXEL_BOARDER);*/

    SSD1306_DrawLine(x1, x1, y1, y2);
    // duty
    SSD1306_DrawLine(x1, x1+pedge, y2, y2);
    SSD1306_DrawLine(x1+pedge, x1+pedge, y1, y2);
    SSD1306_DrawLine(x1+pedge, x2, y1, y1);
    // 2nd edge
    SSD1306_DrawLine(x2, x2, y1, y2);  
    
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}

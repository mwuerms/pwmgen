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

void disp_draw_init(void) {
    // gpios
    // vars
    uint8_t addr = SSD1306_ADDRESS;
    // init ssd1306
    SSD1306_Init (SSD1306_ADDRESS);
    // clear screen
    SSD1306_ClearScreen ();
    /*
    // draw line
    
    // draw line
    SSD1306_DrawLine (0, MAX_X, 18, 18);
    // set position
    SSD1306_SetPosition (40, 3);
    // draw string
    SSD1306_DrawString ("Linus");
    // set position
    SSD1306_SetPosition (48, 3);
    // draw string
    SSD1306_DrawString ("Iris Egli :-)");
    // set position
    SSD1306_SetPosition (58, 5);
    // draw string
    SSD1306_DrawString ("2022");
    // update
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
    */
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}
void disp_draw_clear(void) {
    SSD1306_ClearScreen ();
    SSD1306_UpdateScreen (SSD1306_ADDRESS);
}

struct {
    int8_t  step;
    uint8_t pox_x;
    uint8_t repeate;
} info_ctrl;
void disp_draw_info(void) {
    SSD1306_DrawLine (0, MAX_X, 4, 4);
    SSD1306_SetPosition (7, 1);
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
    SSD1306_DrawLine (info_ctrl.pox_x, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+8);
    SSD1306_DrawLine (info_ctrl.pox_x, 60, ROW_TO_Y(3)+8, ROW_TO_Y(3)+8);
    SSD1306_DrawLine (60, 60, ROW_TO_Y(3)+8, ROW_TO_Y(3)+0);
    SSD1306_DrawLine (60, 70, ROW_TO_Y(3)+0, ROW_TO_Y(3)+0);

    SSD1306_SetPosition (80, 3);
    SSD1306_DrawString ("f=10kHz");

    SSD1306_DrawLine (0, MAX_X, ROW_TO_Y(5)+3, ROW_TO_Y(5)+3);
    SSD1306_SetPosition (0, 6);
    SSD1306_DrawString (GIT_REPO_URL);
    //SSD1306_SetPosition (0, 6+2);
    //SSD1306_DrawString (GIT_VERSION);

    SSD1306_UpdateScreen (SSD1306_ADDRESS);
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY, EV_DISPLAY_UPDATE_INFO);
}

uint8_t disp_draw_update_info(void) {
    SSD1306_ClearArea (30-1,60-1,ROW_TO_Y(3)+0, ROW_TO_Y(3)+8+1);
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
                wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_2S, EV_DISPLAY, EV_DISPLAY_NEXT);
                return RET_STOP;
            }
            info_ctrl.repeate--;
        }
    }

    SSD1306_DrawLine (20, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+0);
    SSD1306_DrawLine (info_ctrl.pox_x, info_ctrl.pox_x, ROW_TO_Y(3)+0, ROW_TO_Y(3)+8);
    SSD1306_DrawLine (info_ctrl.pox_x, 60, ROW_TO_Y(3)+8, ROW_TO_Y(3)+8);

    SSD1306_UpdateScreen (SSD1306_ADDRESS);
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY, EV_DISPLAY_UPDATE_INFO);
    return RET_AGAIN;
}

void disp_draw_pwm_setup(void) {
    return;
}

static const char text_on[] = "ON";
static const char text_off[] = "OFF";
static const char text_3_3V[] = "3.3V";
static const char text_5_0V[] = "5.0V";
static const char text_sweep_off[] = "SWEEP_OFF";
static const char text_sweep_freq[] = "SWEEP_FREQ";
static const char text_sweep_duty[] = "SWEEP_DUTY";

text_label_t output_state = {
    .text = text_on,
    .pos.x = 2,
    .pos.y = 0,
    .frame.pos.x = 2-2,
    .frame.pos.y = 8+0,//ROW_TO_Y(0)-2,
    .frame.size.x = 20,
    .frame.size.y = 8+2,//ROW_TO_Y(1)+2,
    .options = TEXT_LABEL_OPT_ACTIVE,
};

text_label_t output_voltage = {
    .text = text_5_0V,
    .pos.x = 26,
    .pos.y = 0,
    .frame.pos.x = 26-2,
    .frame.pos.y = ROW_TO_Y(0),
    .frame.size.x = 30,
    .frame.size.y = ROW_TO_Y(1)+2,
    .options = TEXT_LABEL_OPT_FOCUS | TEXT_LABEL_OPT_ACTIVE,
};

text_label_t sweep_label = {
    .text = text_sweep_freq,
    .pos.x = 60,
    .pos.y = 0,
    .frame.pos.x = 60-2,
    .frame.pos.y = ROW_TO_Y(0),
    .frame.size.x = 64,
    .frame.size.y = ROW_TO_Y(1)+2,
    .options = 0,
};

frame_t disp_outer_frame = {
    .pos.x = 0,
    .pos.y = 63,
    .size.x = 127,
    .size.y = 63-10,
};

input_2row_t freq_input = {
    .text0 = "F:0000kHz",
    .pos0.x = 70,
    .pos0.y = 2,
    .text1 = "T:0.000ms",
    .pos1.x = 70,
    .pos1.y = 3,
    .frame.pos.x = 70-2,
    .frame.pos.y = ROW_TO_Y(3),
    .frame.size.x = 127-70-2,
    .frame.size.y = ROW_TO_Y(2)-5,
    .options = TEXT_LABEL_OPT_FOCUS,
    .cursor_pos0 = 5,
};

input_2row_t duty_input = {
    .text0 = "Duty:99.9%",
    .pos0.x = 70,
    .pos0.y = 5,
    .text1 = "th:0.00ms",
    .pos1.x = 70,
    .pos1.y = 6,
    .frame.pos.x = 70-2,
    .frame.pos.y = ROW_TO_Y(6),
    .frame.size.x = 127-70-2,
    .frame.size.y = ROW_TO_Y(2)-5,
    .cursor_pos0 = 7,
};

void disp_draw_test_label(void) {
    disp_draw_element_frame(&disp_outer_frame);
    //disp_draw_element_frame(&(output_state.frame));
    disp_draw_element_text_label(&output_state);
    disp_draw_element_text_label(&output_voltage);
    disp_draw_element_text_label(&sweep_label);

    disp_draw_element_input_2row(&freq_input);
    disp_draw_element_input_2row(&duty_input);
}

pwm_graph_t pwm_disp = {
    .pos.x = 6,
    .pos.y = 42,
    .size.x = 54,
    .size.y = 28,
    .frame.pos.x = 3,
    .frame.pos.y = 44,
    .frame.size.x = 60,
    .frame.size.y = 32,
    .duty = 20,
};

void disp_draw_test_pwm(void) {
    pwm_disp.duty++;
    if(pwm_disp.duty > 100)
        pwm_disp.duty = 0;
    disp_draw_element_pwm_graph(&pwm_disp);
    wdtTimer_StartTimeout(1, cEV_TIMER_INTERVAL_125MS, EV_DISPLAY, EV_DISPLAY_UPDATE_PWM);
}
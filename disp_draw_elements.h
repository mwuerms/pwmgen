/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _DISP_DRAW_ELEMENTS_H_
#define _DISP_DRAW_ELEMENTS_H_

/* - includes --------------------------------------------------------------- */
#include <stdint.h>
#include "macrolib.h"

// - definitions ---------------------------------------------------------------

typedef struct {
    int8_t x, y;
} coord_t;

typedef struct {
    coord_t pos;
    coord_t size;
} frame_t;

#define TEXT_LABEL_OPT_FOCUS      _BV(0)
#define TEXT_LABEL_OPT_ACTIVE     _BV(1)
typedef struct {
    char *text;
    coord_t pos;
    frame_t frame;
    uint8_t options;
} text_label_t;

typedef struct {
    char *text0;
    coord_t pos0;
    char *text1;
    coord_t pos1;
    frame_t frame;
    uint8_t options;
    uint8_t cursor_pos0;
} input_2row_t;

typedef struct {
    coord_t pos;
    coord_t size;
    frame_t frame;
    uint8_t duty;
} pwm_graph_t;

// - public functions ----------------------------------------------------------

void disp_draw_element_frame(frame_t *f);
void disp_draw_element_text_label(text_label_t *tl);
void disp_draw_element_pwm_graph(pwm_graph_t *pg);

#endif /* _DISP_DRAW_ELEMENTS_H_ */

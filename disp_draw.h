/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _DISP_DRAW_H_
#define _DISP_DRAW_H_
/* - includes --------------------------------------------------------------- */

// - public functions ----------------------------------------------------------
void disp_draw_init(void);
void disp_draw_clear(void);

void disp_draw_info(void);
uint8_t disp_draw_update_info(void);
void disp_draw_pwm_setup(void);

#endif /* _DISP_DRAW_H_ */

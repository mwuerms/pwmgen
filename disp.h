/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _DISP_H_
#define _DISP_H_
/* - includes --------------------------------------------------------------- */
#include <stdint.h>

// - public functions ----------------------------------------------------------
void disp_init(void);
void disp_show_info(void);
uint8_t disp_update_info(void);
void disp_show_pwm_setup(void);
void disp_process_events(uint8_t events, uint8_t btn_events);

#endif /* _DISP_H_ */

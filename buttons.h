/**
 * Martin Egli
 * 2022-06-06
 *
 */

#ifndef _BUTTONS_H_
#define _BUTTONS_H_
/* - includes --------------------------------------------------------------- */

extern volatile uint8_t button_on_cnt;
extern volatile uint8_t button_wheel_cnt;
extern volatile int8_t wheel_cnt;

void buttons_init(void);

#endif /* _BUTTONS_H_ */
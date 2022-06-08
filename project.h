/**
 * Martin Egli
 * 2021-07-16
 *
 * basic project definitions
 */

#ifndef _PROJECT_H_
#define _PROJECT_H_
/* - includes --------------------------------------------------------------- */
#include <stdint.h>

// - public definitions --------------------------------------------------------
#define RET_OK  (0)
#define RET_ERR (1)

/* - public variables ------------------------------------------------------- */
extern volatile uint8_t global_events;
#define EV_WAKEUP  _BV(0)

#define SEND_EVENT(ev)   global_events |= ev

/* - functions -------------------------------------------------------------- */

#endif /* _PROJECT_H_ */

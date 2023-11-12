#ifndef STATEMACHINE_H
#define STATEMACHINE_H
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif /*__AVR_ATmega162__*/
#include <stdint.h>

/**
 * @brief Enum holding the states for the finite state machine.
*/
typedef enum{
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_IDLE,
} FSM_STATE;

/**
 * @brief The finite state machine loop implementing the game logic.
 * @param[in] calib_array The calibration data of the joystick.
*/
void FSM_loop(uint8_t* calib_array);

#endif /*STATEMACHINE_H*/
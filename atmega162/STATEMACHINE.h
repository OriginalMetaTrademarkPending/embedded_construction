#ifndef FSM_H
#define FSM_H
#include <stdint.h>

typedef enum{
    STATE_INIT,
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_IDLE,
} FSM_STATE;


/**
 * @brief Transitions the state to one of the possible ones. MENU, START_GAME and GAME_END. 
 *
 * @param[in] state The state to transition to
 * @param[in] calib_array A pointer to the calibration array
 */
void fsm_transition_to(FSM_STATE state, uint8_t* calib_array);

#endif /*FSM_H*/
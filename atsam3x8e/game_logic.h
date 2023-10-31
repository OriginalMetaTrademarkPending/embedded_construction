#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "sam.h"
#include "sam3x8e.h"
#include "adc.h"

/**
 * @brief Struct containing the game state.
*/
typedef struct
{
    bool has_scored = false;
    uint16_t score = 0;

}game_state;

static game_state game_info;

/**
 * @brief Counts the goals scored.
 * 
 * @return Number of goals scored.
*/
uint16_t goals_scored();
#endif /*GAME_LOGIC_H*/
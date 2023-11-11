#include "STATEMACHINE.h"
#include "CAN.h"
#include "ADC.h"
#include <avr/io.h>
#include "MENU.h"
#include "CAN.h"

static FSM_STATE current_state = STATE_INIT;

static void fsm_transmit_state(FSM_STATE state){
    CAN_frame m;
    m.data[0] = state;
    CAN_write(&m);
}

void fsm_transition_to(FSM_STATE state, uint8_t* calib_array){
    switch (state)
    {
    case STATE_MENU:
    {
        menu_master(calib_array);

        current_state = STATE_MENU;
        break;
    }
    case STATE_PLAYING:
    {
        // Start playing->Display that the game is running
        OLED_clear();
        OLED_game_running();
        // Send data to node 2. Stop after the game is finished.
        ADC_game(calib_array);

        current_state = STATE_PLAYING;
        break;
    }
    case STATE_GAME_OVER:
    {
        // Game over-> Display the results
        printf("Game over");
        OLED_clear();
        CAN_frame result;
        current_state = STATE_GAME_OVER;
        result = CAN_read();
        OLED_highscores_screen(ca);
        while((PINB & (1 << PB2)))
        {
            printf("Score: %d", result.data[0]);  
        }
        fsm_transition_to(STATE_MENU, calib_array);
        break;
    }
    case STATE_IDLE:
    {
        current_state = STATE_IDLE;
        break;
    }
    default:
        break;
    }

    _delay_ms(50);
    fsm_transmit_state(state);
}














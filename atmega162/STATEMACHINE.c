#include "STATEMACHINE.h"
#include "CAN.h"
#include "ADC.h"
#include <avr/io.h>
#include "OLED.h"

CAN_frame result;
FSM_STATE current_state = STATE_MENU;

void FSM_loop(uint8_t* calib_array){
    while (1)
    {
        switch(current_state){
            case STATE_MENU:
            {
                OLED_clear();
                int selector = 1;
                OLED_home_screen(selector);
                int running = 1;
                while (running) 
                {
                    _delay_ms(300);
                    pos_t joy_pos = pos_read(calib_array);
                    joy_dir dirJoy = dir_read(joy_pos);
                    uint8_t button_press = 0;

                    if((PINB & (1<<PB3)))
                    {
                        button_press = 1;
                    }
                    if(dirJoy==UP){
                        selector -= 1;
                        if(selector < 1){
                            selector=2;
                        }
                        OLED_clear_columns(0, 10);
                        OLED_goto_pos(selector+2, 0);
                        OLED_printf_char('>');
                    }
                    else if (dirJoy==DOWN) {
                        selector += 1;
                        if (selector>2) {
                            selector = 1;
                        }
                        OLED_clear_columns(0, 10);
                        OLED_goto_pos(selector+2, 0);
                        OLED_printf_char('>');
                    }
                    if (button_press){
                        switch (selector)
                        {
                        case 1 :
                            /* Start New Game */
                            current_state = STATE_PLAYING;
                            running = 0;
                            break;
                        case 2:
                            /* Calibrate */
                            OLED_clear();
                            OLED_calibrate_screen();
                            ADC_calibrate(calib_array);
                            break;
                        default:
                            OLED_clear();
                            OLED_home_screen(selector);
                            break;
                        }
                    }
                    else {
                        OLED_home_screen(selector);
                        current_state = STATE_MENU;
                    }
                }
                break;   
            }
            case STATE_PLAYING:
            {
                OLED_clear();
                // Start playing->Display that the game is running
                OLED_game_screen();
                // Send data to node 2. Stop after the game is finished.
                ADC_game(calib_array);
                current_state = STATE_GAME_OVER;
                break;
            }
            case STATE_GAME_OVER:
            {
                OLED_clear();
                OLED_highscores_screen();
                //result = CAN_read();
                while(!(PINB & (1 << PB3)))
                {
                    printf("Score: %d", 2);  
                }
                current_state = STATE_MENU;
            }
            default:
            {
                while((PINB & (1 << PB3)));
                current_state = STATE_MENU;
            }
        }
    }
}
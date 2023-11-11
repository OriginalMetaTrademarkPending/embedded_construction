#include "MENU.h"
#include "OLED.h"
#include "ADC.h"
#include "STATEMACHINE.h"


void menu_master(uint8_t* calib_array)
{
	// uint8_t calib_array[4];
	// ADC_calibrate(calib_array);
	printf("Menu master reached \n\r");
	int selector = 1;
	OLED_clear();
	OLED_home_screen(selector);
	int running = 1;
	while (running) 
	{
		_delay_ms(300);
		pos_t joy_pos = pos_read(calib_array);
		joy_dir dirJoy = dir_read(joy_pos);
        uint8_t button_press = 0;

        if(!(PINB & (1<<PB2)))
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
                fsm_transition_to(STATE_PLAYING, calib_array);
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
        OLED_home_screen(selector);
		
	}
}


void menu_init(){

}
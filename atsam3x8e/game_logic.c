#include "game_logic.h"

uint16_t goals_scored()
{
    uint16_t adc_val = ma_read();
        printf("ADC value: %u\n\r", adc_val);
        if(adc_val < 3500){
            if(game_info.has_scored == false)
            {
                game_info.score++;
                game_info.has_scored = true;
            }
        }
        else
        {
            game_info.has_scored = false;
        }
    return game_info.score;
}
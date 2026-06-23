/*********************************************************************************************************************
Copyright (c) 2026, Saab Araoz Melina <melinasaabaraoz@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*********************************************************************************************************************/

/** @file screen.c
 ** @brief Abstracción de pantalla multiplexada de siete segmentos
 **/

/* === Headers files inclusions ==================================================================================== */

#include "screen.h"
#include <string.h>

/* === Macros definitions ========================================================================================== */

#ifndef DISPLAY_MAX_DIGITS
#define DISPLAY_MAX_DIGITS 8
#endif

/* === Private data type declarations ============================================================================== */

/**
 * @brief Estructura de la pantalla multiplexada
 * 
 */
struct display_s{
    uint8_t digits;
    uint8_t active_digit;
    uint8_t flashing_fron;
    uint8_t flashing_to;
    uint16_t flashing_frecuency;
    uint16_t flashing_count;
    uint8_t display_memory[DISPLAY_MAX_DIGITS];
    struct display_driver_s driver[1];
};

/* === Private function declarations =============================================================================== */

static display_t DisplayAllocate(void);

/* === Private variable definitions ================================================================================ */

static const uint8_t IMAGES[] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,             // 0
    SEGMENT_B | SEGMENT_C,                                                             // 1
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         // 2
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                         // 3
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                     // 4
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         // 5
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             // 6
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                 // 7
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, // 8
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G              // 9
};

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

static display_t DisplayAllocate(void){
    static struct display_s instances[1] = {0};
    
    return &instances[0];
}

/* === Public function implementation ============================================================================== */

/**
 * @brief Crea una pantalla multiplexada de siete segmentos
 * @param  digits  Cantidad de dígitos de la pantalla
 * @param  driver  Puntero a la estructura con los callbacks de hardware
 * @return         Descriptor de la pantalla, o @c NULL si no hay instancia disponible
 */
display_t DisplayCreate(uint8_t digits, display_driver_t driver){
    display_t display = DisplayAllocate();
    if(display){
        display->digits = digits;
        display->active_digit = digits - 1;
        display->flashing_count = 0;
        display->flashing_fron = 0;
        display->flashing_to = 0;
        display->flashing_frecuency = 0;
        memcpy(display->driver, driver, sizeof(display -> driver));
        memset(display->display_memory, 0, sizeof(display->display_memory));
        display -> driver -> UpdateSegments(0x00);
    }
    return display;
}

/**
 * @brief Escribe un número BCD en la memoria de la pantalla
 * @param  display  Descriptor creado con @ref DisplayCreate
 * @param  number   Puntero al primer dígito BCD (cada elemento en @c 0..9)
 * @param  size     Cantidad de dígitos en @p number
 */
void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size){
    memset(display->display_memory, 0, sizeof(display->display_memory));
    for(int i = 0; i < size; i++){
        if(i >= display->digits){
            break;
        }
        display->display_memory[i] = IMAGES[number[i]];
    }
}

void DisplayRefresh(display_t display){
    uint8_t segments;

    display -> driver -> UpdateSegments(0x00);
    display -> active_digit = (display -> active_digit + 1) % display -> digits;
   
    if (display->active_digit == 0){
        display->flashing_count++;
        if (display->flashing_count >= display->flashing_frecuency){
            display->flashing_count = 0; 
        }
    } 

    segments = display -> display_memory[display -> active_digit];
    
    if (display->flashing_frecuency > 0){
        if (display->flashing_count >= display->flashing_frecuency / 2){
            if ((display->active_digit >= display->flashing_fron) && (display->active_digit <= display->flashing_to)){
                segments=0;
            }
        }
    }

    display -> driver -> UpdateDigits(display -> active_digit);
    display -> driver -> UpdateSegments(segments);
}

/**
 * @brief Configura los parámetros de parapadeo 
 * @param display Puntero al objeto del display
 * @param from índice del primer dígito donde comienza el efecto de parpadeo
 * @param to índice del último dígito donde termina el efecto de parpadeo 
 * @param frecuency Cantidad de ciclos para conmutar el estado 
 */
void DisplayFlashDigits(display_t display, uint8_t from, uint8_t to, uint16_t frecuency){
    display->flashing_count = 0;
    display->flashing_fron = from;
    display->flashing_to = to;
    display->flashing_frecuency = frecuency;
}

/**
 * @brief Conmuta el punto decimal de un rango de dígitos
 * @param  display  Descriptor creado con @ref DisplayCreate
 * @param  from     Índice del primer dígito
 * @param  to       Índice del último dígito
 */
void DisplayToggleDots(display_t display, uint8_t from, uint8_t to){
    for(int i = from; i <= to; i++){
        display -> display_memory[i] ^= SEGMENT_P;
    }
}
/* === End of documentation ======================================================================================== */

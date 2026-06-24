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

/** @file clock.c
 ** @brief Archivo para implementación del reloj
 **/

/* === Headers files inclusions ==================================================================================== */

#include "clock.h"
#include <stdio.h>
#include <string.h>

/* === Macros definitions ========================================================================================== */

#define CLOCK_SIZE_BCD 6
#define SECONDS_POSITION 5
#define MINUTE_POSITION 3
#define HOURS_POSITION 1

/* === Private data type declarations ============================================================================== */

struct clock_s{
    hour_t time;
    bool time_is_valid;
    unsigned int ticks_per_second;
    uint16_t ticks_counter;
};

/* === Private function declarations =============================================================================== */

/**
 * @brief Función auxiliar para manejar los límites de cada posición del vector
 */
static void IncreaceTimeBCD(clock_t clock, uint8_t position);

/**
 * @brief Función auxiliar para comprobar si una hora es valida
 */
static bool ValidHour(hour_t hour);

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/**
 * @brief Función auxiliar para manejar los límites de cada posición del vector
 */
static void IncreaceTimeBCD(clock_t clock, uint8_t position){
    clock -> time[position]++;

    if (clock -> time[position] >= 10) {
        clock -> time[position] = 0;
        IncreaceTimeBCD(clock, position-1);

        if(position == SECONDS_POSITION || position == MINUTE_POSITION){
            if(clock -> time[position-1] == 6){
                clock -> time[position-1] = 0;
                IncreaceTimeBCD(clock, position-2);
            }
        }
    }

    if(position == HOURS_POSITION){
        if(clock -> time[position-1] >= 2 && clock -> time[position] == 4){
            memset(clock -> time, 0, sizeof(hour_t));
        }
    }
}

/**
 * @brief Función auxiliar para comprobar si una hora es valida
 */
static bool ValidHour(hour_t hour){
    for(int indice = 0; indice < CLOCK_SIZE_BCD; indice++){
        if(hour[indice] >= 10 || hour[indice] < 0){
            return false;
        }
        if((indice == SECONDS_POSITION-1 || indice == MINUTE_POSITION-1) && hour[indice]>= 6){
            return false;
        }
        if(indice == HOURS_POSITION-1 && hour[indice] >= 2){
            if(hour[indice] == 2 && hour[indice+1] < 4){
                return true;
            }
            return false;
        }
    }

    return true;
}

/* === Public function implementation ============================================================================== */

/**
 * @brief Funcion para crear una variable de tipo reloj
 * @param ticks_per_second variables que indica la cantidad de ciclos por segundo ?
 * @param alarm_handler puntero a una función de alarma
 */
clock_t ClockCreate(unsigned int ticks_per_second, void * alarm_handler){
    static struct clock_s instance;
    clock_t self = &instance;
    self -> ticks_per_second = ticks_per_second;
    self -> ticks_counter = 0;
    self -> time_is_valid = false;
    memset(self -> time, 0, sizeof(hour_t));

    return self;
}

/**
 * @brief Función para obtener la hora.
 * @param clock variable de reloj
 * @param current_hour variable con la hora actual
 * @return retorna verdadero si la hora es valida y falso si es invalida
 */
bool ClockGetCurrentTime(clock_t clock, hour_t current_hour){
    memcpy(current_hour, clock -> time, sizeof(hour_t));
    return clock -> time_is_valid;
}

/**
 * @brief Función para establecer una hora.
 * @param clock variable de reloj
 * @param new_hour variable con la hora que se quiere establecer
 * @return retorna verdadero si la hora es valioda y falso si es invalida
 */
bool ClockSetUpCurrentTime(clock_t clock, const hour_t new_hour){
    if(ValidHour(new_hour)){
        memcpy(clock -> time, new_hour, sizeof(hour_t));
        clock -> time_is_valid = true;
    }
    return clock -> time_is_valid;
}

/**
 * @brief Función que hace avanzar el tiempo.
 * @param clock variable de reloj
 */
void ClockNewTick(clock_t clock){
    clock -> ticks_counter++;

    if(clock -> ticks_counter >= clock -> ticks_per_second){
        clock -> ticks_counter = 0;
        IncreaceTimeBCD(clock, SECONDS_POSITION);
    }
}

/* === End of documentation ======================================================================================== */

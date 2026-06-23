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

/* === Private data type declarations ============================================================================== */

struct clock_s{
    hour_t time;
    bool time_is_valid;
};

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

/**
 * @brief Funcion para crear una variable de tipo reloj
 * @param ticks_per_second variables que indica la cantidad de ciclos por segundo ?
 * @param alarm_handler puntero a una función de alarma
 */
clock_t ClockCreate(unsigned int ticks_per_second, void * alarm_handler){
    static struct clock_s instance;
    clock_t self = &instance;
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
    memcpy(clock -> time, new_hour, sizeof(hour_t));
    clock -> time_is_valid = true;

    return clock -> time_is_valid;
}

/**
 * @brief Función que hace avanzar el tiempo.
 * @param clock variable de reloj
 */
void ClockNewTick(clock_t clock){
    clock -> time[5] = 7;
}

/* === End of documentation ======================================================================================== */

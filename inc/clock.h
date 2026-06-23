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

#ifndef CLOCK_H_
#define CLOCK_H_

/** @file clock.h
 ** @brief Archivo para el control del reloj
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdbool.h>
#include <stdint.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @brief Tipo de dato para el reloj
 */
typedef struct clock_s * clock_t;

/**
 * @brief Tipo de dato para la hora
 */
typedef uint8_t hour_t[6];

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Funcion para crear una variable de tipo reloj
 * @param ticks_per_second variable que indica la cantidad de ciclos por segundo ?
 * @param alarm_handler puntero a una función de alarma
 */
clock_t ClockCreate(unsigned int ticks_per_second, void * alarm_handler);

/**
 * @brief Función para obtener la hora.
 * @param clock variable de reloj
 * @param current_hour variable con la hora actual o que se quiere usar.
 * @return retorna verdadero si la hora es valioda y falso si es invalida
 */
bool ClockGetCurrentTime(clock_t clock, hour_t current_hour);

/**
 * @brief Función para establecer una hora.
 * @param clock variable de reloj
 * @param new_hour variable con la hora que se quiere establecer
 * @return retorna verdadero si la hora es valioda y falso si es invalida
 */
bool ClockSetUpCurrentTime(clock_t clock, const hour_t new_hour);

/**
 * @brief Función que hace avanzar el tiempo.
 * @param clock variable de reloj
 */
void ClockNewTick(clock_t clock);



/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* CLOCK_H_ */

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

/**
 * @brief Estructura del reloj
 * @param time Guarda la hora del reloj
 * @param time_is_valid Especifica si la hora que muestra el reloj es válida
 * @param tick_per_second Guarda los ciclos necesarios que deben pasar para 1 segundo
 * @param tick_countewr Guarda la cuenta de cuantos ticks ocurrieron para saber si paso o no 1 segundo
 * @param alarm Guarda la alarma seteada
 * @param alarm_enabled Esppecifica si la alarma está activada
 * @param AlarmHandler Función que maneja el evento de la alarma
 */
struct clock_s{
    hour_t time;
    bool time_is_valid;
    unsigned int ticks_per_second;
    uint16_t ticks_counter;

    hour_t alarm;
    bool alarm_enabled;
    clock_event_t AlarmHandler;
};

static const uint8_t SECOND_MINUTE_LIMIT[2] = {6,0};
static const uint8_t HOUR_LIMIT[2]= {2,4};  // Reloj 24 horas
// static const uint8_t HOUR_LIMIT[2]= {1,2};  // Reloj 12 horas

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
            if(clock -> time[position-1] == SECOND_MINUTE_LIMIT[0]){
                clock -> time[position-1] = 0;
                IncreaceTimeBCD(clock, position-2);
            }
        }
    }

    if(position == HOURS_POSITION){
        if(clock -> time[position-1] >= HOUR_LIMIT[0] && clock -> time[position] == HOUR_LIMIT[1]){
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
clock_t ClockCreate(unsigned int ticks_per_second, clock_event_t alarm_handler){
    static struct clock_s instance;
    clock_t self = &instance;
    self -> ticks_per_second = ticks_per_second;
    self -> ticks_counter = 0;
    self -> time_is_valid = false;
    memset(self -> time, 0, sizeof(hour_t));

    self -> alarm_enabled = false;
    self -> AlarmHandler = alarm_handler;
    memset(self -> alarm, 0, sizeof(hour_t));

    return self;
}

/**
 * @brief Función para obtener la hora.
 * @param clock variable de reloj
 * @param current_hour variable con la hora actual
 * @return retorna verdadero si la hora es valida y falso si es invalida
 */
bool ClockGetCurrentTime(clock_t clock, hour_t current_hour){
    if(current_hour != NULL){
        memcpy(current_hour, clock -> time, sizeof(hour_t));
    }
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

        // --- CONTROL DE ALARMA ---
        if (clock -> alarm_enabled && clock -> AlarmHandler != NULL) {
            if (memcmp(clock -> time, clock -> alarm, sizeof(hour_t)) == 0) {
                clock -> AlarmHandler(clock);
            }
        }
    }
}

/**
 * @brief Función para obtener la alarma seteada.
 * @param clock variable de reloj
 * @param consulted_alarm variable a la cual se le da la alrma seteada
 * @return retorna verdadero si la alarma es valida y falso si es invalida
 */
bool ClockGetAlarm(clock_t clock, hour_t consulted_alarm){
    if(consulted_alarm != NULL){
        memcpy(consulted_alarm, clock -> alarm, sizeof(hour_t));
    }
    return clock -> alarm_enabled;
}

/**
 * @brief Función para establecer una alarma.
 * @param clock variable de reloj
 * @param new_hour variable con la hora que se quiere establecer
 * @return retorna verdadero si la hora es valioda y falso si es invalida
 */
bool ClockSetUpAlarm(clock_t clock, const hour_t new_alarm){
    if(ValidHour(new_alarm)){
        memcpy(clock -> alarm, new_alarm, sizeof(hour_t));
        clock -> alarm_enabled = true;
    }
    return clock -> alarm_enabled;
}

/* === End of documentation ======================================================================================== */

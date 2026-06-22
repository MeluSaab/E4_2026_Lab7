/*********************************************************************************************************************
Copyright (c) 2026, Saab Araoz Melina <saabaraozmelina@gmail.com>

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

/** @file inoutput.c
 ** @brief Implementación para las entradas y salidas digitales.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "inoutput.h"
#include "chip.h"
#include <stdlib.h>
#include <stdbool.h>

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/**
 * @brief Estructura para representar una salida digital
 * @param gpio Puerto
 * @param bit Pin del Puerto
 * @param state Estado de la salida
 *
 */
struct inoutput_output_s {
    uint32_t port;
    uint8_t pin;
    bool state;
};

/**
 * @brief Estructura para representar una entrada digital
 * @param port Puerto
 * @param pin Pin del Puerto
 * @param logic true si la tecla trabaja con lógica invertida (activo en bajo / PULL-UP),
 *              false si trabaja con lógica directa (activo en alto / PULL-DOWN).
 * @param last_state variable para almacenar el ultimo estado de la entrada digital
 *
 */
struct inoutput_input_s {
    uint32_t port;
    uint8_t pin;
    bool logic;
    bool last_state;
};

/* === Private function declarations =============================================================================== */

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

/* === Public function implementation ============================================================================== */

/* === OutPut ================================================================================ */
/**
 * @brief Funcion para crear una salida digital
 * @param state true si la carga trabaja con lógica invertida (activa en bajo),
 *                 false si trabaja con lógica directa (activa en alto).
 * @return inoutput_output_t Puntero al objeto de la salida digital creada
 */
inoutput_output_t InoutputOutputCreate(uint32_t port, uint8_t pin, bool state){
    inoutput_output_t self = malloc(sizeof(struct inoutput_output_s));
    if(self){
        self->port = port;
        self->pin = pin;
        self->state = state;

        InoutputOutputDeactivate(self);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self -> port, self -> pin, true);
    }
    return self;
}

/**
 * @brief Funcion para activar la salida digital
 * @param self Puntero a la salida digital
 */
void InoutputOutputActivate(inoutput_output_t self){
    if(self != NULL){
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, self -> port, self -> pin, !self -> state);
    }
}

/**
 * @brief Funcion para desactivar la salida digital
 * @param self Puntero a la salida digital
 */
void InoutputOutputDeactivate(inoutput_output_t self){
    if(self != NULL){
       Chip_GPIO_SetPinState(LPC_GPIO_PORT, self -> port, self -> pin, self -> state);
    }
}

/**
 * @brief Invierte el estado actual de la salida digital
 * @param self Puntero a la salida digital
 */
void InoutputOutputToggle(inoutput_output_t self){
    if (self != NULL){
        Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self -> port, self -> pin);
    }
}

/**
 * @brief Funcion para crear entradas digitales
 * @param port Puerto
 * @param pin Pin del puerto
 * @param logic true si la tecla trabaja con lógica invertida (activo en bajo / PULL-UP),
 *              false si trabaja con lógica directa (activo en alto / PULL-DOWN).
 * @return inoutput_input_t Puntero al objeto de la entrada digital creada
 */
inoutput_input_t InoutputInputCreate(uint32_t port, uint8_t pin, bool logic) {
    inoutput_input_t self = malloc(sizeof(struct inoutput_input_s));
   if(self){
    self->port = port;
    self->pin = pin;
    self->logic = logic;
    self -> last_state = InoutputInputGetState(self);
   }

    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self -> port, self -> pin, false);
    return self;
}

/**
 * @brief Funcion para obtener el estado actual de la entrada digital
 * @param self Puntero a la entrada
 */
bool InoutputInputGetState(inoutput_input_t self) {
    if (self == NULL) {
        return false;
    }
    bool state_pin = Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self -> port, self -> pin);
    if (self -> logic) {
        return !state_pin;
    } else {
        return state_pin;
    }
}

/**
 * @brief Funcion para conocer si la entrada digital cambio de estado
 * @param self Puntero a la entrada digital
 * @return 1 si se debe activar
 *         2 si se debe desactivar
 *         0 si no hubo cambios por lo que no debe ocurrir un evento
 */
int InoutputInputHasChanged(inoutput_input_t self) {
    int resultado = INOUTPUT_INPUT_NO_EVENT;
    if (self != NULL) {
        bool actual = InoutputInputGetState(self);
        if (actual && !self->last_state) {
            resultado = INOUTPUT_INPUT_ACTIVATE_EVENT;
        } else if (!actual && self->last_state) {
            resultado = INOUTPUT_INPUT_DEACTIVATE_EVENT;
        }
        self->last_state = actual;
    }
    return resultado;
}

/**
 * @brief Funcion para conocer si la entrada digital fue activada
 * @param self Puntero a la entrada digital
 */
bool InoutputInputHasActivated(inoutput_input_t self) {
    return InoutputInputHasChanged(self) == INOUTPUT_INPUT_ACTIVATE_EVENT;
}

/**
 * @brief Funcion para conocer si la entrada digital fue desactivada
 * @param self Puntero a la entrada digital
 */
bool InoutputInputHasDeactivated(inoutput_input_t self) {
    return InoutputInputHasChanged(self) == INOUTPUT_INPUT_DEACTIVATE_EVENT;
}

/* === End of documentation ======================================================================================== */

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

#ifndef INOUTPUT_H_
#define INOUTPUT_H_

/** @file inoutput.h
 ** @brief Declaraciones para las entradas y salidas digitales.
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdint.h>
#include <stdbool.h>

#define INOUTPUT_INPUT_NO_EVENT         0
#define INOUTPUT_INPUT_ACTIVATE_EVENT   1
#define INOUTPUT_INPUT_DEACTIVATE_EVENT 2

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @brief Estructura para representar a las salidas digitales.
 */
typedef struct inoutput_output_s * inoutput_output_t;

/**
 * @brief Estructura para representar las entradas digitales.
 */
typedef struct inoutput_input_s * inoutput_input_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/* === OutPut ================================================================================ */
/**
 * @brief Funcion para crear una salida digital
 * @param state true si la carga trabaja con lógica invertida (activa en bajo),
 *                 false si trabaja con lógica directa (activa en alto).
 * @return inoutput_output_t Puntero al objeto de la salida digital creada
 */
inoutput_output_t InoutputOutputCreate(uint32_t port, uint8_t pin, bool state);

/**
 * @brief Funcion para activar la salida digital
 * @param self Puntero a la salida digital
 */
void InoutputOutputActivate(inoutput_output_t output);

/**
 * @brief Funcion para desactivar la salida digital
 * @param self Puntero a la salida digital
 */
void InoutputOutputDeactivate(inoutput_output_t output);

/**
 * @brief Invierte el estado actual de la salida digital
 * @param self Puntero a la salida digital
 */
void InoutputOutputToggle(inoutput_output_t output);

/* === InPut ================================================================================ */
/**
 * @brief Funcion para crear entradas digitales
 * @param port Puerto
 * @param pin Pin del puerto
 * @param logic true si la tecla trabaja con lógica invertida (activo en bajo / PULL-UP),
 *              false si trabaja con lógica directa (activo en alto / PULL-DOWN).
 * @return inoutput_input_t Puntero al objeto de la entrada digital creada
 */
inoutput_input_t InoutputInputCreate(uint32_t port, uint8_t pin, bool logic);

/**
 * @brief Funcion para obtener el estado actual de la entrada digital
 * @param self Puntero a la entrada
 */
bool InoutputInputGetState(inoutput_input_t entrada);

/**
 * @brief Funcion para conocer si la entrada digital cambio de estado
 * @param self Puntero a la entrada digital
 * @return 1 si se debe activar
 *         2 si se debe desactivar
 *         0 si no hubo cambios por lo que no debe ocurrir un evento
 */
int InoutputInputHasChanged(inoutput_input_t entrada);

/**
 * @brief Funcion para conocer si la entrada digital fue activada
 * @param self Puntero a la entrada digital
 */
bool InoutputInputHasActivated(inoutput_input_t entrada);

/**
 * @brief Funcion para conocer si la entrada digital fue desactivada
 * @param self Puntero a la entrada digital
 */
bool DigitalInputHasDeactivated(inoutput_input_t entrada);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* INOUTPUT_H_ */
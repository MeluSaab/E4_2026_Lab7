/**
 * === PRUEBAS PARA LA ALARMA ===
 * - Probar que el create no devuelve algo nulo.
 * - Al ajustar la hora del reloj queda en hora y es válida.
 * - Probar que no se pone en hora (SetUpCurrentTime) si esta es invalida.
 * - Después de n ciclos de reloj la hora avanza 1 segundo.
 * - Después de n ciclos de reloj la hora avanza 10 segundo.
 * - Después de n ciclos de reloj la hora avanza 1 minuto.
 * - Después de n ciclos de reloj la hora avanza 1 día.
*/


#include "unity.h"
#include "clock.h"
static const hour_t DEFOULT_TIME = {0,0,0,0,0,0}; // 00:00:00
static const hour_t INITIAL_TIME = {1,2,3,4,5,6}; // 12:34:56
static const hour_t INVALID_TIME = {9,9,9,9,9,9}; // 99:99:99

static const hour_t ALARMA = {1, 2, 3, 4, 5, 7};
static bool alarma_sono_en_el_sistema = false;


#define TICKS_PER_SECOND 3
#define ONE_SECOND TICKS_PER_SECOND
#define TEN_SECONDS (10 * ONE_SECOND)


void SimulateClockTicks(clock_t reloj, unsigned int ticks){
    for(int indice = 0; indice <= ticks; indice++){
        ClockNewTick(reloj);
    } 
}

// Esta es la función ficticia que le pasamos al reloj
void MockAlarmHandler(clock_t clock) {
    alarma_sono_en_el_sistema = true;
}

// Probar que el create no devuelve algo nulo.
void test_prueba_Create_GetCurrentTime(void){
    clock_t reloj;
    hour_t hora_actual = {1,2,3,4,5,6}; // 12:34:56

    // 1 es ticks_per_second
    // NULL es AlarmHandler
    reloj = ClockCreate(1, NULL);
    TEST_ASSERT_FALSE(ClockGetCurrentTime(reloj, hora_actual));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(DEFOULT_TIME, hora_actual, 6);
}

// Al ajustar la hora del reloj queda en hora y es válida
void test_prueba_Create_GetCurrentTime_HoraValida(void){
    clock_t reloj;
    hour_t hora_actual;

    reloj = ClockCreate(1, NULL);

    // RelojSetUpCurrentTime devuelve TRUE porque la hora es válida.
    TEST_ASSERT_TRUE(ClockSetUpCurrentTime(reloj, INITIAL_TIME));
    TEST_ASSERT_TRUE(ClockGetCurrentTime(reloj, hora_actual));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(INITIAL_TIME, hora_actual, 6);

}

// Probar que no se pone en hora (SetUpCurrentTime) si esta es invalida 
void test_prueba_Create_GetCurrentTime_HoraInvalida(void){
    clock_t reloj;
    hour_t hora_actual;

    reloj = ClockCreate(3, NULL);

    // RelojSetUpCurrentTime devuelve FALSE porque la hora es inválida.
    TEST_ASSERT_FALSE(ClockSetUpCurrentTime(reloj, INVALID_TIME));
    ClockGetCurrentTime(reloj, hora_actual);
    //La hora al no ser válida el reloj debe seguir teniendo la hora por defecto
    TEST_ASSERT_EQUAL_UINT8_ARRAY(DEFOULT_TIME, hora_actual, 6);
}

// Después de n ciclos de reloj la hora avanza 1 segundo
void test_prueba_hora_avanza_n_ciclos(void){
    clock_t reloj;
    hour_t hora_actual;
    static const hour_t EXPECTED_TIME = {1,2,3,4,5,7}; // 12:34:57

    reloj = ClockCreate(TICKS_PER_SECOND, NULL);
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    
    SimulateClockTicks(reloj, ONE_SECOND);

    ClockGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED_TIME, hora_actual, 6);
}

// Después de n ciclos de reloj la hora avanza 10 segundo
void test_prueba_hora_avanza_10_segundos(void){
    clock_t reloj;
    hour_t hora_actual;
    static const hour_t EXPECTED_TIME = {1,2,3,5,0,6}; // 12:35:06

    reloj = ClockCreate(TICKS_PER_SECOND, NULL);
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    
    SimulateClockTicks(reloj, TEN_SECONDS);

    ClockGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED_TIME, hora_actual, 6);
}

// Después de n ciclos de reloj la hora avanza 1 hora
void test_prueba_hora_avanza_1_minuto(void){
    clock_t reloj;
    hour_t hora_actual;
    static const hour_t EXPECTED_TIME = {1,3,3,4,5,6}; // 13:34:57

    reloj = ClockCreate(TICKS_PER_SECOND, NULL);
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    
    SimulateClockTicks(reloj, 3600*ONE_SECOND);

    ClockGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED_TIME, hora_actual, 6);
}

// Después de n ciclos de reloj la hora avanza 1 dia
void test_prueba_hora_avanza_1_dia(void){
    clock_t reloj;
    hour_t hora_actual;
    static const hour_t EXPECTED_TIME = {1,2,3,4,5,6}; // 12:34:56

    reloj = ClockCreate(TICKS_PER_SECOND, NULL);
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    
    SimulateClockTicks(reloj, 24*3600*ONE_SECOND);

    ClockGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED_TIME, hora_actual, 6);
}

/**
 * === PRUEBAS PARA LA ALARMA ===
 * - Fijar la hora de la alarma y consultarla.
 * - Probar que con una alarma ivalida no se setea la alarma.
 * - Fijar la alarma y avanzar el reloj para que suene.
 * - Fijar la alarma, deshabilitarla y avanzar el reloj para que no suene.
 * - Hacer sonar la alarma y posponerla.
 * - Hacer sonar la alarma y cancelarla hasta el otro día.
 * - La alarma no suena hasta que se configure el reloj.
 * - Decidir qué sucede con el reloj desconfigurado y el avance de la hora.
 */

// Fijar la hora de la alarma y consultarla.
void test_fijar_y_consultar_alarma(void) {
    clock_t reloj;
    hour_t alarma_consultada = {0,0,0,0,0,0};

    reloj = ClockCreate(TICKS_PER_SECOND, NULL);

    // Seteamos la alarma
    TEST_ASSERT_TRUE(ClockSetUpAlarm(reloj, ALARMA));

    // Consultamos la alarma y verificamos que devuelva la hora correcta
    TEST_ASSERT_TRUE(ClockGetAlarm(reloj, alarma_consultada));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ALARMA, alarma_consultada, 6);
}

// Probar que con una alarma ivalida no se setea la alarma.
void test_fijar_y_consultar_alarma_invalido(void) {
    clock_t reloj;
    hour_t alarma_consultada = {0,0,0,0,0,0};

    reloj = ClockCreate(TICKS_PER_SECOND, NULL);

    // Seteamos la alarma con una hora inválida
    TEST_ASSERT_FALSE(ClockSetUpAlarm(reloj, INVALID_TIME));

    // Consultamos la alarma y verificamos que devuelva la hora por defecto pues no se seteó la alarma
    (void)(ClockGetAlarm(reloj, alarma_consultada));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(DEFOULT_TIME, alarma_consultada, 6);
}


// Fijar la alarma y avanzar el reloj para que suene.
void test_fijar_alarma_y_hacerla_sonar(void) {
    clock_t reloj;
    hour_t hora_actual;
    
    // Inicializamos nuestra bandera en falso antes del test
    alarma_sono_en_el_sistema = false;

    // Pasamos MockAlarmHandler
    reloj = ClockCreate(TICKS_PER_SECOND, MockAlarmHandler);
    
    // Ponemos en hora el reloj (12:34:56) y seteamos la alarma 1 segundo después (12:34:57)
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    (void)ClockSetUpAlarm(reloj, ALARMA);
    
    // La alarma todavía NO debería haber sonado
    TEST_ASSERT_FALSE(alarma_sono_en_el_sistema);
    
    // Simulamos el paso de 1 segundo (3 ticks) para llegar a la hora exacta
    SimulateClockTicks(reloj, ONE_SECOND);
    
    // Verificamos que el reloj haya invocado la función de la alarma
    TEST_ASSERT_TRUE(alarma_sono_en_el_sistema);
}

// Fijar la alarma, deshabilitarla y avanzar el reloj para que no suene.
void test_fijar_alarma_desabilitarla_y_que_no_suene(void) {
    clock_t reloj;
    hour_t hora_actual;
    
    // Inicializamos nuestra bandera en falso antes del test
    alarma_sono_en_el_sistema = false;

    // Pasamos MockAlarmHandler
    reloj = ClockCreate(TICKS_PER_SECOND, MockAlarmHandler);
    
    // Ponemos en hora el reloj (12:34:56) y seteamos la alarma 1 segundo después (12:34:57)
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    (void)ClockSetUpAlarm(reloj, ALARMA);
    
    // Desactivamos la alarma
    ClockToggleAlarm(reloj);

    // Simulamos el paso de 1 segundo (3 ticks) para llegar a la hora exacta
    SimulateClockTicks(reloj, ONE_SECOND);
    
    // Verificamos que el reloj NO haya invocado la función de la alarma
    TEST_ASSERT_FALSE(alarma_sono_en_el_sistema);
}

// Hacer sonar la alarma y posponerla.
void test_fijar_alarma_y_posponerla(void) {
    clock_t reloj;
    hour_t hora_actual;
    
    // Inicializamos nuestra bandera en falso antes del test
    alarma_sono_en_el_sistema = false;

    // Pasamos MockAlarmHandler
    reloj = ClockCreate(TICKS_PER_SECOND, MockAlarmHandler);
    
    // Ponemos en hora el reloj (12:34:56) y seteamos la alarma 1 segundo después (12:34:57)
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    (void)ClockSetUpAlarm(reloj, ALARMA);
    
    // Simulamos el paso de 1 segundo (3 ticks) para llegar a la hora exacta
    SimulateClockTicks(reloj, ONE_SECOND);
    
    ClockSnoozeAlarm(reloj);

    alarma_sono_en_el_sistema = false;
    SimulateClockTicks(reloj, 60*5*ONE_SECOND);

    // Verificamos que el reloj haya invocado de verdad la función de la alarma
    TEST_ASSERT_TRUE(alarma_sono_en_el_sistema);
}

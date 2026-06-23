// Probar que el create no devuelve algo nulo.
// Al ajustar la hora del reloj queda en hora y es válida
// Después de n ciclos de reloj la hora avanza 1 segundo
// Probar que no se pone en hora (SetUpCurrentTime) si esta es invalida 



#include "unity.h"
#include "clock.h"
static const hour_t DEFOULT_TIME = {0,0,0,0,0,0};
static const hour_t INITIAL_TIME = {1,2,3,4,5,6};

#define TICKS_PER_SECOND 3
#define ONE_SECOND TICKS_PER_SECOND
#define TEN_SECONDS (10 * ONE_SECOND)

void SimulateClockTicks(clock_t reloj, unsigned int ticks){
    for(int indice = 0; indice <= 3; indice++){
        ClockNewTick(reloj);
    } 
}

// Probar que el create no devuelve algo nulo.
void test_prueba_Create_GetCurrentTime_invalido(void){
    clock_t reloj;
    hour_t hora_actual = {1,2,3,4,5,6};

    // 1 es ticks_per_second
    // NULL es AlarmHandler
    reloj = ClockCreate(1, NULL);
    TEST_ASSERT_FALSE(ClockGetCurrentTime(reloj, hora_actual));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(DEFOULT_TIME, hora_actual, 6);
}

// Al ajustar la hora del reloj queda en hora y es válida
void test_prueba_Create_GetCurrentTime_valido(void){
    clock_t reloj;
    hour_t hora_actual;

    // 1 es ticks_per_second
    // NULL es AlarmHandler
    reloj = ClockCreate(1, NULL);

    // RelojSetUpCurrentTime devuelve TRUE porque la hora es válida.
    TEST_ASSERT_TRUE(ClockSetUpCurrentTime(reloj, INITIAL_TIME));
    TEST_ASSERT_TRUE(ClockGetCurrentTime(reloj, hora_actual));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(INITIAL_TIME, hora_actual, 6);

}

// Después de n ciclos de reloj la hora avanza 1 segundo
void test_prueba_hora_avanza_n_ciclos(void){
    clock_t reloj;
    hour_t hora_actual;
    static const hour_t EXPECTED_TIME = {1,2,3,4,5,7};

    // 1 es ticks_per_second
    // NULL es AlarmHandler
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
    static const hour_t EXPECTED_TIME = {1,2,3,4,6,6};

    // 1 es ticks_per_second
    // NULL es AlarmHandler
    reloj = ClockCreate(TICKS_PER_SECOND, NULL);
    (void)ClockSetUpCurrentTime(reloj, INITIAL_TIME);
    
    SimulateClockTicks(reloj, TEN_SECONDS);

    ClockGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED_TIME, hora_actual, 6);
}

// Probar que no se pone en hora (SetUpCurrentTime) si esta es invalida 
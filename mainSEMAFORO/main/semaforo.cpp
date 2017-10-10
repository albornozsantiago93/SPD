
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "semaforo.h"
#include "timer.h"

#define FALSE 0
#define TRUE 1

#define SEMAFORO_ROJO_1 0
#define SEMAFORO_AMARILLO_1 1
#define SEMAFORO_VERDE_1 2

static int state_semaforo;

/** \brief  Se invoca una sola vez cuando el programa empieza. 
 *          Se utiliza para inicializar los modos de trabajo 
 *          de los pines, el puerto serie, etc... 
 * \param void
 * \return void
 */
void semaforo_setup(void)
{
    Serial.begin(9600);
    state_semaforo = SEMAFORO_ROJO_1;
}


/** \brief  Contiene el programa del semaforo que se ejecutará cíclicamente
 *          
 * \param void
 * \return void
 */
void semaforo_loop(void)
{

    if( state_semaforo == SEMAFORO_ROJO_1 && timer_waitMs(RELOJ_TIMER_4,2000))
    {
        state_semaforo = SEMAFORO_AMARILLO_1;
    }
    else if( state_semaforo == SEMAFORO_AMARILLO_1 && timer_waitMs(RELOJ_TIMER_4,1000))
    {
        state_semaforo = SEMAFORO_VERDE_1;
    }      
    else if( state_semaforo == SEMAFORO_VERDE_1 && timer_waitMs(RELOJ_TIMER_4,3000))
    {
        state_semaforo = SEMAFORO_ROJO_1;
    }    
    
    
    if(timer_waitMs(RELOJ_TIMER_5,1000))
    {
        Serial.println(state_semaforo);
    }     
   
}

int semaforo(int state_semaforo)
{
  int flag=0;

  if(flag==0)
  {
    flag= 1;
    if(state_semaforo== SEMAFORO_ROJO_1)
    {
      flag= 0;
      return 0;
    }
    if(state_semaforo== SEMAFORO_AMARILLO_1)
    {
      flag= 0;
      return 1;
    }
    if(state_semaforo== SEMAFORO_VERDE_1)
    {
      flag=0;
      return 2;
    }      
  }
}


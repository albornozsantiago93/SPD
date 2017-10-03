/******************************************************************
* Programa: Ejemplo de uso del LCD Keypad Shield
*
* Objetivo:
*   Mostrar los aspectos básicos de funcionamiento del LCD Keypad Shield
*   Manejar una entrada analógica
*
* Aspectos a destacar:
*   -Lectura de la entrada analogica 
*
* Versión: 0.6 del 19 de Junio de 2017
* Autor: Mauricio Dávila
* Revisión: -
*
* *******************************************************************/
#include "reloj.h"
#include "timer.h"

  
/** \brief  Se invoca una sola vez cuando el programa empieza. 
 *          Se utiliza para inicializar los modos de trabajo 
 *          de los pines, el puerto serie, etc... 
 * \param void
 * \return void
 */
void setup(void)
{
  
  timer_setup();
  reloj_setup(); // inicializo el reloj

}


/** \brief  Contiene el programa que se ejecutará cíclicamente
 *          
 * \param void
 * \return void
 */
void loop(void)
{
    timer_loop();
    reloj_loop();

}

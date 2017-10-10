
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "reloj.h"
#include "timer.h"
#include "semaforo.h"

#define FALSE 0
#define TRUE 1

#define RELOJ_STATE_NONE 0
#define RELOJ_STATE_SHOW 1
#define RELOJ_STATE_SETUP_MINUTOS 2
#define RELOJ_STATE_SETUP_SEGUNDOS 3
#define RELOJ_STATE_STANDBY 4

#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5


static int minutos;
static int segundos;
static int minutosSetup;
static int segundosSetup;
static int state_reloj = RELOJ_STATE_NONE;
static int lastKey = btnNONE;

static LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 


/** \brief  Se invoca una sola vez cuando el programa empieza. 
 *          Se utiliza para inicializar los modos de trabajo 
 *          de los pines, el puerto serie, etc... 
 * \param void
 * \return void
 */
void reloj_setup(void)
{
    minutos = 0;
    segundos = 0;
    state_reloj = RELOJ_STATE_SHOW;

    lcd.begin(16, 2); // inicializo la biblioteca indicando 16 caracteres por 2 lineas
}


/** \brief  Contiene el programa del reloj que se ejecutará cíclicamente
 *          
 * \param void
 * \return void
 */
void reloj_loop(void)
{
    int tecla = btnNONE;
    char pantalla[21];
    if(timer_waitMs(RELOJ_TIMER_0,150))
    {
        tecla = read_LCD_buttons();
        if(tecla != btnNONE)
          timer_resetWait(RELOJ_TIMER_0);
    }    
        
    if(state_reloj == RELOJ_STATE_SHOW)
    {
        if(tecla == btnSELECT)
        {
            state_reloj = RELOJ_STATE_SETUP_MINUTOS;
            minutosSetup = minutos;
            segundosSetup = segundos;
        }
        if(tecla == btnDOWN)
        {
          state_reloj = RELOJ_STATE_STANDBY;
        }
        lcd.setCursor(0,0);
        sprintf(pantalla,"Rel - %02d:%02d      ",minutos,segundos);
        lcd.print(pantalla);
        
    }
    else if(state_reloj == RELOJ_STATE_SETUP_SEGUNDOS)
    {
        if(tecla == btnUP)
        {
            if( segundosSetup < 59)
              segundosSetup++;
            else
              segundosSetup =0;
        }
        if(tecla == btnDOWN)
        {
            if( segundosSetup > 0)
              segundosSetup--;
            else
              segundosSetup = 59;
        }
        
        if(tecla == btnLEFT)
        {
            state_reloj = RELOJ_STATE_SETUP_MINUTOS;
        }
        if(tecla == btnSELECT)
        {
            state_reloj = RELOJ_STATE_SHOW;
            minutos = minutosSetup;
            segundos = segundosSetup;
        }
        lcd.setCursor(0,0);
        sprintf(pantalla,"Set (S) - %02d:%02d",minutosSetup,segundosSetup);
        lcd.print(pantalla);
    }
    else if(state_reloj == RELOJ_STATE_SETUP_MINUTOS)
    {
        if(tecla == btnUP)
        {
            if( minutosSetup < 59)
              minutosSetup++;
            else
              minutosSetup =0;
        }
        if(tecla == btnDOWN)
        {
            if( minutosSetup > 0)
              minutosSetup--;
            else
              minutosSetup = 59;
        }
        if(tecla == btnRIGHT)
        {
            state_reloj = RELOJ_STATE_SETUP_SEGUNDOS;
        }
        if(tecla == btnSELECT)
        {
            state_reloj = RELOJ_STATE_SHOW;
            minutos = minutosSetup;
            segundos = segundosSetup;
            
        }
        lcd.setCursor(0,0);
        sprintf(pantalla,"Set (M) - %02d:%02d",minutosSetup,segundosSetup);
        lcd.print(pantalla);        
    }
  else if(state_reloj == RELOJ_STATE_STANDBY)
  {        
        if(tecla == btnLEFT)
        {
           state_reloj = RELOJ_STATE_SETUP_MINUTOS;
        }
        if(tecla == btnRIGHT)
        {
           state_reloj = RELOJ_STATE_SETUP_SEGUNDOS;
        }
        if(tecla == btnSELECT)
        {
          state_reloj = RELOJ_STATE_SHOW;
        }
        if(tecla == btnUP)
        {
          if(semaforo(state_reloj)==0)
          {
            lcd.setCursor(0,0);
            sprintf(pantalla,"VERDE");
            lcd.print(pantalla); 
          }
          if(semaforo(state_reloj)==1)
          {
            lcd.setCursor(0,0);
            sprintf(pantalla,"AMARILLO");
            lcd.print(pantalla); 
          }
          if(semaforo(state_reloj)==2)
          {
            lcd.setCursor(0,0);
            sprintf(pantalla,"ROJO");
            lcd.print(pantalla); 
          }
       
        }

  }

    if(timer_waitMs(RELOJ_TIMER_1,1000))
    {
        incrementarSegundos();
        timer_resetWait(RELOJ_TIMER_1);
    }

}

void incrementarSegundos()
{
        if(segundos<59)
        {
            segundos++;
        }
        else
        {
            segundos=0;
            if(minutos<59)
            {
                minutos++;
            }
            else
            {
                minutos = 0;
            }
        }

}

/** \brief  Realiza la lectura de la entrada analogica y determina según su valor
 *          a que botón hace referencia.
 * \param void
 * \return int: Que representa el botón pulsado
 */
int read_LCD_buttons(void)
{
  int lcd_key = btnNONE; // el valor inicial es NONE
  int adc_key_in = 0;
  adc_key_in = analogRead(0); // lectura de la entrada analogica

  if (adc_key_in > 1000)
    lcd_key = btnNONE; 
  else if (adc_key_in < 50) 
    lcd_key = btnRIGHT;
  else if (adc_key_in < 195) 
    lcd_key = btnUP;
  else if (adc_key_in < 380) 
    lcd_key = btnDOWN;
  else if (adc_key_in < 555)  
    lcd_key = btnLEFT;
  else if (adc_key_in < 790) 
    lcd_key = btnSELECT;

  return lcd_key; 
}

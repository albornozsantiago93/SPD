
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "reloj.h"
#include "timer.h"

#define FALSE 0
#define TRUE 1

#define RELOJ_STATE_NONE 0
#define RELOJ_STATE_SHOW 1
#define RELOJ_STATE_SETUP 2
#define RELOJ_STATE_ALARM 3

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
static int horas;
static int minutosAlarma;
static int horasAlarma;
static int horasSetup;
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
    Serial.begin(9600);
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
    int ultimaTecla;
    char pantalla[21];
    if(timer_waitMs(RELOJ_TIMER_0,150))
    {
        tecla = read_LCD_buttons();
        ultimaTecla= tecla;
        if(tecla != btnNONE && tecla != ultimaTecla)
          timer_resetWait(RELOJ_TIMER_0);
    }    
        
    if(state_reloj == RELOJ_STATE_SHOW)
    {
        if(tecla == btnSELECT)
        {
            state_reloj = RELOJ_STATE_SETUP;
            minutosSetup = minutos;
            segundosSetup = segundos;
            horasSetup= horas;
        }
        
        lcd.setCursor(0,0);
        sprintf(pantalla,"Rel - %02d:%02d:%02d",horas,minutos,segundos);
        lcd.print(pantalla);
        
    }
    if(state_reloj== RELOJ_STATE_SHOW)
    {
      if(tecla == btnDOWN)
      {
        state_reloj= RELOJ_STATE_ALARM;
      }
      if(state_reloj== RELOJ_STATE_ALARM)
      {
        if(tecla == btnRIGHT)
        {
          horasAlarma++;
          lcd.setCursor(0,1);
          sprintf(pantalla,"Set alarm , HORAS");
          lcd.print(pantalla);
        }
        if(tecla == btnUP)
        {
          minutosAlarma++;
          lcd.setCursor(0,1);
          sprintf(pantalla,"Set alarm, MINUTOS");
          lcd.print(pantalla);
        }
        if(tecla == btnSELECT)
        {
          state_reloj= RELOJ_STATE_SHOW;
          lcd.clear();
        }

        
      }
    }

    
    else if(state_reloj == RELOJ_STATE_SETUP)
    {
        if(tecla == btnRIGHT)
        {
           segundosSetup++;
           lcd.setCursor(0,1);
           sprintf(pantalla,"Set - SEGUNDOS");
           lcd.print(pantalla);            
        }
        if(tecla == btnUP)
        {
          minutosSetup++;
          lcd.setCursor(0,1);
          sprintf(pantalla,"Set - MINUTOS ");
          lcd.print(pantalla); 
        }
        if(tecla == btnLEFT)
        {
          horasSetup++;
          lcd.clear();
          lcd.setCursor(0,1);
          sprintf(pantalla,"Set - HORAS ");
          lcd.print(pantalla); 
        }
        if(tecla == btnSELECT)
        {
           segundos= segundosSetup;
           minutos= minutosSetup;
           horas= horasSetup;
           state_reloj = RELOJ_STATE_SHOW;
           lcd.clear();            
        }
        lcd.setCursor(0,0);
        sprintf(pantalla,"Set - %02d:%02d:%02d",horasSetup,minutosSetup,segundosSetup);
        lcd.print(pantalla);
    }



    //////////////////////////////////////////////////////
 
    

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
            if(horas<23)
            {
              horas++;
            }
            else
            {
              horas = 0;
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

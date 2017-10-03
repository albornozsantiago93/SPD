#ifndef TMR
#define TMR
    #define RELOJ_TIMER_0 0
    #define RELOJ_TIMER_1 1

    typedef struct
    {
        unsigned long delayValue;
        char running;
    }S_timerData;
#endif

int timer_waitMs(int delayNumber,int delayMs);
void timer_resetWait(int delayNumber);
void timer_setup();
void timer_tick(uint32_t interval);
int timer_loop();


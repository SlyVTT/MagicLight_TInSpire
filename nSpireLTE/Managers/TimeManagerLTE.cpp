#include "TimeManagerLTE.hpp"

#include "../Globals/GUIToolkitNFGlobals.hpp"
#include "../Globals/GlobalFunctions.hpp"

#include <libndls.h>


static volatile unsigned *value;
static volatile unsigned *control;


TimeManager::TimeManager()
{
    //ctor
}


TimeManager& TimeManager::Get( void )
{
    static TimeManager m_timer;
    return m_timer;
}


void TimeManager::InternalInitialize( void )
{
    start = 0;
    tick_sum = 0;

    InternalStartTicks();
}


void TimeManager::InternalClose( void )
{

}


int TimeManager::InternalGetCurrentHour( void )
{
    int HH = 0;
    int MM = 0;
    int SS = 0;

    InternalUpdateTimer();

    uint32_t DD=RTC_seconds/86400;

    SS = RTC_seconds%86400;
    HH = SS/3600;
    MM = (SS-3600*HH)/60;
    SS %= 60;

return HH;

}


int TimeManager::InternalGetCurrentMinute( void )
{
    int HH = 0;
    int MM = 0;
    int SS = 0;

    InternalUpdateTimer();

    uint32_t DD=RTC_seconds/86400;

    SS = RTC_seconds%86400;
    HH = SS/3600;
    MM = (SS-3600*HH)/60;
    SS %= 60;


return MM;
}


int TimeManager::InternalGetCurrentSecond( void )
{
    int HH = 0;
    int MM = 0;
    int SS = 0;

    InternalUpdateTimer();

    uint32_t DD=RTC_seconds/86400;

    SS = RTC_seconds%86400;
    HH = SS/3600;
    MM = (SS-3600*HH)/60;
    SS %= 60;


return SS;
}


void TimeManager::InternalGetCurrentTime( int* HH, int* MM, int*SS )
{
    InternalUpdateTimer();

    uint32_t DD=RTC_seconds/86400;

    *SS = RTC_seconds%86400;
    *HH = *SS/3600;
    *MM = (*SS-3600* *HH)/60;
    *SS %= 60;
}


void TimeManager::InternalUpdateTimer( void )
{
    RTC_seconds = * (volatile unsigned *) 0x90090000;
}


void TimeManager::InternalStartTicks(void)
{
    value = (unsigned *)0x900C0004;
    control = (unsigned *)0x900C0008;
    *(volatile unsigned *)0x900C0080 = 0xA; /* "[...] on the CX to specify the 32768Hz clock as the source for the First Timer" */
    *control = 0b10000010; /* Wrapping; 32-bit; divider to 1; interrupt disabled; free-running; start */
    start = *value;
}


uint32_t TimeManager::InternalGetTicks(void)
{
    if ( has_colors )
        return((start - *value) / 33);
    else
    {
        tick_sum += *value;
        *value = 0;
        return(tick_sum);
    }
}


void TimeManager::InternalDelay(uint32_t ms)
{
    msleep(ms);
}


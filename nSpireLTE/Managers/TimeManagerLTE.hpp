#ifndef TIMEMANAGER_HPP
#define TIMEMANAGER_HPP

#include <os.h>


class TimeManager
{
    public:

    TimeManager(TimeManager&) = delete;
	TimeManager& operator= (const TimeManager&) = delete;

	static TimeManager& Get( void );

    static void Delay( uint32_t ms ) {Get().InternalDelay( ms ); };
    static void StartTicks( void ) { Get().InternalStartTicks( ); };
    static uint32_t GetTicks( void ) { return Get().InternalGetTicks( ); };

    static void Initialize( void ) { Get().InternalInitialize(); };
    static void Close( void ) { Get().InternalClose(); };

    static int GetCurrentHour( void ) { return Get().InternalGetCurrentHour(); };
    static int GetCurrentMinute( void ) { return Get().InternalGetCurrentMinute(); };
    static int GetCurrentSecond( void ) { return Get().InternalGetCurrentSecond(); };
    static void GetCurrentTime( int* HH, int* MM, int*SS ) { Get().InternalGetCurrentTime( HH, MM, SS); };

    protected:

    private:

        TimeManager();
        static TimeManager m_timer;

        void InternalDelay( uint32_t ms );
        void InternalStartTicks( void );
        uint32_t InternalGetTicks( void );

        void InternalInitialize( void );
        void InternalClose( void );

        int InternalGetCurrentHour( void );
        int InternalGetCurrentMinute( void );
        int InternalGetCurrentSecond( void );
        void InternalGetCurrentTime( int* HH, int* MM, int*SS );
        void InternalUpdateTimer( void );

//        static volatile unsigned *value;
//        static volatile unsigned *control;

        uint32_t tick_sum;
        uint32_t start;

        uint32_t RTC_seconds;
};

#endif // TIMEMANAGER_HPP

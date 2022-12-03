#ifndef TIMER_HEADER_GUARD
#define TIMER_HEADER_GUARD

#include "common.h"
#include <stdbool.h>


typedef struct
{
    int m_ID;
    unsigned int m_intervalMs;
    bool m_fIsInitialized;
    bool m_fHasElapsed;
    bool m_fIsRunning;
} Timer_t;

typedef enum
{
    TIMER_ERROR_OK = 0,
    TIMER_ERROR_NOK = 1,
    TIMER_ERROR_ALREADY_STARTED = 2,
    TIMER_ERROR_ALREADY_STOPPED = 3,
    TIMER_ERROR_INVALID_INTERVAL = 4,
    TIMER_ERROR_NOT_INITIALIZED = 5
} Timer_Error_E;

Timer_Error_E Timer_Create(IN OUT Timer_t* pTimer, unsigned int interval);
Timer_Error_E Timer_Destroy(Timer_t* pTimer);
Timer_Error_E Timer_Start(Timer_t* pTimer);
Timer_Error_E Timer_Stop(Timer_t* pTimer);
Timer_Error_E Timer_ChangeInterval(Timer_t* pTimer, unsigned int interval);



#endif
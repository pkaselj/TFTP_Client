
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// WARNING: TESTING DROPIN FOR TIMER, NOT A REAL TIMER
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/* -------------------------------------------------------------------------- */
/*                                  INCLUDES                                  */
/* -------------------------------------------------------------------------- */

#include "timer.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

/* -------------------------------------------------------------------------- */
/*                                   MACROS                                   */
/* -------------------------------------------------------------------------- */

// Use macro instead of function to preserve __LINE__ value used by _DEBUG_MSG
#define _Timer_AssertValidHandle(HANDLE)                             \
    if (NULL == HANDLE)                                                   \
    {                                                                   \
        Log_Error(_DEBUG_MSG("Invalid handle to " _STR(Timer_t))); \
    }

#define _Timer_LogCurrentPosition(HANDLE, FUNCTION) _Timer_LogMessage(HANDLE, _DEBUG_MSG(_STR(FUNCTION)"()"))

#define MAX_TIMER_ID 10000

#define DEBUG_MSG_SIZE 512

/* -------------------------------------------------------------------------- */
/*                                  TYPEDEFS                                  */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/*                            FUNCTION DEFINITIONS                            */
/* -------------------------------------------------------------------------- */

/* --------------------------------- PRIVATE -------------------------------- */

PRIVATE void _Timer_LogMessage(Timer_t* pTimer, const char* szMessage);

/* -------------------------------------------------------------------------- */
/*                                   GLOBALS                                  */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/*                           FUNCTION IMPLEMENTATION                          */
/* -------------------------------------------------------------------------- */

/* -------------------------------- EXPORTED -------------------------------- */

Timer_Error_E Timer_Create(IN OUT Timer_t* pTimer, unsigned int interval)
{
    _Timer_AssertValidHandle(pTimer)

    IF_DEBUG(_Timer_LogCurrentPosition(pTimer, Timer_Create));

    if (interval <= 0)
    {
        return TIMER_ERROR_INVALID_INTERVAL;
    }

    pTimer->m_ID = 0;
    pTimer->m_intervalMs = interval;
    pTimer->m_fHasElapsed = false;
    pTimer->m_fIsRunning = false;

    pTimer->m_fIsInitialized = true;

    return TIMER_ERROR_OK;

}

Timer_Error_E Timer_Destroy(Timer_t* pTimer)
{
    _Timer_AssertValidHandle(pTimer)

    IF_DEBUG(_Timer_LogCurrentPosition(pTimer, Timer_Destroy));

    if (false == pTimer->m_fIsInitialized)
    {
        return TIMER_ERROR_NOT_INITIALIZED;
    }

    if (true == pTimer->m_fIsRunning)
    {
        Timer_Stop(pTimer);
    }
    
    // TODO
    // Do nothing currently

    pTimer->m_fIsInitialized = false;

    return TIMER_ERROR_OK;
}

Timer_Error_E Timer_Start(Timer_t* pTimer)
{
    _Timer_AssertValidHandle(pTimer)

    IF_DEBUG(_Timer_LogCurrentPosition(pTimer, Timer_Start));

    if (false == pTimer->m_fIsInitialized)
    {
        return TIMER_ERROR_NOT_INITIALIZED;
    }

    if (true == pTimer->m_fIsRunning)
    {
        return TIMER_ERROR_ALREADY_STARTED;
    }


    // TODO: TESTING
    pTimer->m_fIsRunning = true;
    Sleep(pTimer->m_intervalMs); 
    pTimer->m_fIsRunning = false;

    return TIMER_ERROR_OK;
    
}

Timer_Error_E Timer_Stop(Timer_t* pTimer)
{
    _Timer_AssertValidHandle(pTimer)

    IF_DEBUG(_Timer_LogCurrentPosition(pTimer, Timer_Stop));

    if (false == pTimer->m_fIsInitialized)
    {
        return TIMER_ERROR_NOT_INITIALIZED;
    }

    if (false == pTimer->m_fIsRunning)
    {
        return TIMER_ERROR_ALREADY_STOPPED;
    }

    pTimer->m_fIsRunning = false;
    // TODO

    return TIMER_ERROR_OK;
    
}

Timer_Error_E Timer_ChangeInterval(Timer_t* pTimer, unsigned int interval)
{
    _Timer_AssertValidHandle(pTimer)

    IF_DEBUG(_Timer_LogCurrentPosition(pTimer, Timer_ChangeInterval));

    if (false == pTimer->m_fIsInitialized)
    {
        return TIMER_ERROR_NOT_INITIALIZED;
    }

    if (false == pTimer->m_fIsRunning)
    {
        return TIMER_ERROR_NOK;
    }

    if (interval < 0)
    {
        return TIMER_ERROR_INVALID_INTERVAL;
    }

    pTimer->m_intervalMs = interval;
    return TIMER_ERROR_OK;
    
}



/* --------------------------------- PRIVATE -------------------------------- */

PRIVATE void _Timer_LogMessage(Timer_t* pTimer, const char* szMessage)
{
    if (NULL == pTimer)
    {
        return;
    }

    if (NULL == szMessage)
    {
        return;
    }

    char szFullDebugMessage[DEBUG_MSG_SIZE] = {0};

    snprintf(szFullDebugMessage, DEBUG_MSG_SIZE, "Timer ID [%lu] :: %s", pTimer->m_ID, szMessage);
    Log_Debug(szFullDebugMessage);
}

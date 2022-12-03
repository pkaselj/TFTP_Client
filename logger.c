/* -------------------------------------------------------------------------- */
/*                                  INCLUDES                                  */
/* -------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "logger.h"

/* -------------------------------------------------------------------------- */
/*                                   MACROS                                   */
/* -------------------------------------------------------------------------- */

#define DEFAULT_LOG_LEVEL LOG_VERBOSITY_DEBUG

#define ISSET(FLAGS, LOG_FLAG) ((FLAGS & ~LOG_FLAG) != 0)

/* -------------------------------------------------------------------------- */
/*                                  TYPEDEFS                                  */
/* -------------------------------------------------------------------------- */

typedef struct _HLOGGER HLOGGER;
typedef LOG_ERROR_E (*FNPPRINT)(const char *);

typedef struct _HLOGGER
{
    FNPPRINT m_fnWarning;
    FNPPRINT m_fnInfo;
    FNPPRINT m_fnSuccess;
    FNPPRINT m_fnDebug;
    FNPPRINT m_fnError;
} HLOGGER;

/* -------------------------------------------------------------------------- */
/*                            FUNCTION DEFINITIONS                            */
/* -------------------------------------------------------------------------- */

/* --------------------------------- PRIVATE -------------------------------- */

PRIVATE LOG_ERROR_E _Log_Initialize_wLevel(LOG_LEVEL_FLAGS fLogLevel);
PRIVATE void _Log_Success(const char *message);
PRIVATE void _Log_Print(const char *prefix, const char *message);
PRIVATE HLOGGER *Log_CreateInstance(LOG_LEVEL_FLAGS fLevel);
PRIVATE LOG_ERROR_E Log_DestroyInstance(HLOGGER *pHandle);
PRIVATE void _Log_Warning(const char *message);
PRIVATE void _Log_Error(const char *message);
PRIVATE void _Log_Info(const char *message);
PRIVATE void _Log_Debug(const char *message);

/* -------------------------------------------------------------------------- */
/*                                   GLOBALS                                  */
/* -------------------------------------------------------------------------- */

GLOBAL PRIVATE HLOGGER *g_pLogger;

/* -------------------------------------------------------------------------- */
/*                           FUNCTION IMPLEMENTATION                          */
/* -------------------------------------------------------------------------- */

/* -------------------------------- EXPORTED -------------------------------- */

LOG_ERROR_E Log_Initialize(void)
{
    return _Log_Initialize_wLevel(DEFAULT_LOG_LEVEL);
}

LOG_ERROR_E Log_Deinitialize(void)
{
    return Log_DestroyInstance(g_pLogger);
}

LOG_ERROR_E Log_SetLevel(LOG_LEVEL_FLAGS fLogLevel)
{
    LOG_ERROR_E err = Log_Deinitialize();
    if (err != LOG_ERROR_OK)
    {
        return err;
    }

    return _Log_Initialize_wLevel(fLogLevel);
}

LOG_ERROR_E Log_Success(const char *message)
{
    if (g_pLogger == NULL || g_pLogger->m_fnSuccess == NULL)
    {
        return LOG_ERROR_NOK;
    }

    _Log_Success(message);

    return LOG_ERROR_OK;
}

LOG_ERROR_E Log_Debug(const char *message)
{
    if (g_pLogger == NULL || g_pLogger->m_fnDebug == NULL)
    {
        return LOG_ERROR_NOK;
    }

    _Log_Debug(message);

    return LOG_ERROR_OK;
}

LOG_ERROR_E Log_Info(const char *message)
{
    if (g_pLogger == NULL || g_pLogger->m_fnInfo == NULL)
    {
        return LOG_ERROR_NOK;
    }

    _Log_Info(message);

    return LOG_ERROR_OK;
}

LOG_ERROR_E Log_Warning(const char *message)
{
    if (g_pLogger == NULL || g_pLogger->m_fnWarning == NULL)
    {
        return LOG_ERROR_NOK;
    }

    _Log_Warning(message);

    return LOG_ERROR_OK;
}

LOG_ERROR_E Log_Error(const char *message)
{
    if (g_pLogger == NULL || g_pLogger->m_fnError == NULL)
    {
        return LOG_ERROR_NOK;
    }

    _Log_Error(message);
    exit(-1);

    return LOG_ERROR_OK;
}

/* --------------------------------- PRIVATE -------------------------------- */

PRIVATE LOG_ERROR_E _Log_Initialize_wLevel(LOG_LEVEL_FLAGS fLogLevel)
{
    g_pLogger = Log_CreateInstance(fLogLevel);
    if (g_pLogger == NULL)
    {
        return LOG_ERROR_NOK;
    }
    return LOG_ERROR_OK;
}

PRIVATE HLOGGER *Log_CreateInstance(LOG_LEVEL_FLAGS fLevel)
{
    HLOGGER *pHandle = (HLOGGER *)malloc(sizeof(HLOGGER));

    if (pHandle == NULL)
    {
        return NULL;
    }

    pHandle->m_fnWarning = (ISSET((int)fLevel, LOG_LEVEL_WARNING) ? &Log_Warning : NULL);
    pHandle->m_fnError = &Log_Error; // Non maskable
    pHandle->m_fnInfo = (ISSET((int)fLevel, LOG_LEVEL_INFO) ? &Log_Info : NULL);
    pHandle->m_fnSuccess = (ISSET((int)fLevel, LOG_LEVEL_SUCCESS) ? &Log_Success : NULL);
    pHandle->m_fnDebug = (ISSET((int)fLevel, LOG_LEVEL_DEBUG) ? &Log_Debug : NULL);

    return pHandle;
}

PRIVATE LOG_ERROR_E Log_DestroyInstance(HLOGGER *pHandle)
{
    if (pHandle == NULL)
    {
        return LOG_ERROR_NOK;
    }

    free(pHandle);

    return LOG_ERROR_OK;
}

PRIVATE void _Log_Warning(const char *message)
{
    _Log_Print("WARNING", message);
}

PRIVATE void _Log_Error(const char *message)
{
    _Log_Print("ERROR", message);
}

PRIVATE void _Log_Info(const char *message)
{
    _Log_Print("INFO", message);
}

PRIVATE void _Log_Debug(const char *message)
{
    _Log_Print("DEBUG", message);
}

PRIVATE void _Log_Success(const char *message)
{
    _Log_Print("SUCCESS", message);
}

PRIVATE void _Log_Print(const char *prefix, const char *message)
{
    printf("[%s] :: %s\n", prefix, message);
}
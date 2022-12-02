#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"

typedef enum
{
    LOG_LEVEL_INFO = BIT(0),
    LOG_LEVEL_WARNING = BIT(1),
    LOG_LEVEL_SUCCESS = BIT(2),
    LOG_LEVEL_DEBUG = BIT(3)
    // ERROR -- Non maskable
} LOG_LEVEL_FLAGS;

#define LOG_VERBOSITY_LOW (LOG_LEVEL_WARNING | LOG_LEVEL_SUCCESS)
#define LOG_VERBOSITY_NORMAL (LOG_LEVEL_WARNING | LOG_LEVEL_SUCCESS | LOG_LEVEL_INFO)
#define LOG_VERBOSITY_DEBUG (LOG_LEVEL_WARNING | LOG_LEVEL_SUCCESS | LOG_LEVEL_INFO | LOG_LEVEL_DEBUG)

typedef enum
{
    LOG_ERROR_OK = 0,
    LOG_ERROR_NOK = 1
} LOG_ERROR_E;

LOG_ERROR_E Log_Initialize(void);
LOG_ERROR_E Log_Deinitialize(void);
LOG_ERROR_E Log_SetLevel(LOG_LEVEL_FLAGS fLogLevel);

LOG_ERROR_E Log_Warning(const char *message);
LOG_ERROR_E Log_Error(const char *message);
LOG_ERROR_E Log_Info(const char *message);
LOG_ERROR_E Log_Debug(const char *message);
LOG_ERROR_E Log_Success(const char *message);

#endif // !LOGGER_H

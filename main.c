#include <stdio.h>
#include "logger.h"

int main(void)
{
    LOG_ERROR_E error;

    error = Log_Initialize();
    if (error != LOG_ERROR_OK)
    {
        printf("Error while initializing logger: " _STR(error));
        return -1;
    }

    error = Log_SetLevel(LOG_VERBOSITY_DEBUG);
    if (error != LOG_ERROR_OK)
    {
        printf("Error while initializing logger: " _STR(error));
        return -1;
    }

    (void)Log_Info("Test message");

    printf("Hello, World!\n");
    return 0;
}
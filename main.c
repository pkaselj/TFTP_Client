#include <stdio.h>
#include "logger.h"
#include "TFTP_WRQ_FSM.h"

int main(void)
{
    LOG_ERROR_E error;

    error = Log_Initialize();
    if (error != LOG_ERROR_OK)
    {
        printf("Error while initializing logger.\n");
        return -1;
    }

    error = Log_SetLevel(LOG_VERBOSITY_DEBUG);
    if (error != LOG_ERROR_OK)
    {
        printf("Error while initializing logger.\n");
        return -1;
    }

    (void)Log_Info("Test message");

    TFTP_WRQ_FSM fsm;
    TFTP_WRQ_Initialize(&fsm);

    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_EVENT_ERROR, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_EVENT_ERROR, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_STATE_ATTEMPT_INIT, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_EVENT_ERROR, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_STATE_ATTEMPT_INIT, NULL);

    printf("Hello, World!\n");
    return 0;
}
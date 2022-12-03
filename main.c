#include <stdio.h>
#include "logger.h"
#include "TFTP_WRQ_FSM.h"
#include "timer.h"

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

    IF_DEBUG(Log_Info("DEBUG"));
    (void)Log_Info("Test message");

    TFTP_WRQ_FSM fsm;
    TFTP_WRQ_Initialize(&fsm);

    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_EVENT_ERROR, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_EVENT_ERROR, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_STATE_ATTEMPT_INIT, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_EVENT_ERROR, NULL);
    TFTP_WRQ_ProcessEvent(&fsm, TFTP_WRQ_STATE_ATTEMPT_INIT, NULL);

    Timer_t timer;
    const unsigned int intervalMs = 10000;
    Timer_Error_E timer_error;


    timer_error = Timer_Create(&timer, intervalMs);
    if (timer_error != TIMER_ERROR_OK)
    {
        printf("Timer create error #%d\n", timer_error);
        return -1;
    }

    Log_Info("Timer start!");
    timer_error = Timer_Start(&timer);
    if (timer_error != TIMER_ERROR_OK)
    {
        printf("Timer start error #%d\n", timer_error);
        return -1;
    }
    Log_Info("Timer stop!");
    
    
    timer_error = Timer_Destroy(&timer);
    if (timer_error != TIMER_ERROR_OK)
    {
        printf("Timer destroy error #%d\n", timer_error);
        return -1;
    }

    printf("Hello, World!\n");
    return 0;
}
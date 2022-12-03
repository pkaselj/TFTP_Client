#ifndef TFTP_WRQ_FSM_HEADER
#define TFTP_WRQ_FSM_HEADER

#include "logger.h"
#include "timer.h"
#include <stdbool.h>

typedef enum
{
    TFTP_WRQ_STATE_IDLE,
    TFTP_WRQ_STATE_ERROR,
    TFTP_WRQ_STATE_ATTEMPT_INIT,
    TFTP_WRQ_STATE_ATTEMPT_DATA,
    TFTP_WRQ_STATE_ATTEMPT_DATA_LAST_BLOCK
} TFTP_WRQ_STATE_E;

typedef enum
{
    TFTP_WRQ_EVENT_RESET,
    TFTP_WRQ_EVENT_ERROR,
    TFTP_WRQ_EVENT_TIMEOUT,
    TFTP_WRQ_EVENT_RECV_ACK_EQ_0,
    TFTP_WRQ_EVENT_RECV_ACK_NOT_0
} TFTP_WRQ_EVENT_E;

typedef struct
{
    TFTP_WRQ_STATE_E m_currentState;
    Timer_t m_timer;
    bool m_fIsInitialized;
} TFTP_WRQ_FSM;

void TFTP_WRQ_Initialize(TFTP_WRQ_FSM *pFSM);
void TFTP_WRQ_Deinitialize(TFTP_WRQ_FSM *pFSM);
void TFTP_WRQ_Reset(TFTP_WRQ_FSM *pFSM);
void TFTP_WRQ_ProcessEvent(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_EVENT_E event, void *pData);

#endif
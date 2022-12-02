/* -------------------------------------------------------------------------- */
/*                                  INCLUDES                                  */
/* -------------------------------------------------------------------------- */

#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include "TFTP_WRQ_FSM.h"

/* -------------------------------------------------------------------------- */
/*                                   MACROS                                   */
/* -------------------------------------------------------------------------- */

#define DEBUG_MSG_SIZE 512

/* -------------------------------------------------------------------------- */
/*                                  TYPEDEFS                                  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                            FUNCTION DEFINITIONS                            */
/* -------------------------------------------------------------------------- */

/* --------------------------------- PRIVATE -------------------------------- */

PRIVATE void _TFTP_WRQ_AssertValidHandle(TFTP_WRQ_FSM *pFSM);
PRIVATE void _TFTP_WRQ_WriteProcessEventLog(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_EVENT_E event);

/* -------------------------------------------------------------------------- */
/*                                   GLOBALS                                  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                           FUNCTION IMPLEMENTATION                          */
/* -------------------------------------------------------------------------- */

/* -------------------------------- EXPORTED -------------------------------- */

void TFTP_WRQ_Initialize(TFTP_WRQ_FSM *pFSM)
{
    _TFTP_WRQ_AssertValidHandle(pFSM);

    pFSM->m_currentState = TFTP_WRQ_STATE_IDLE;
    pFSM->m_fIsInitialized = true;
}

void TFTP_WRQ_ProcessEvent(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_EVENT_E event, void *pData)
{
    UNUSED(pData);

    _TFTP_WRQ_AssertValidHandle(pFSM);

    _TFTP_WRQ_WriteProcessEventLog(pFSM, event);
}

/* --------------------------------- PRIVATE -------------------------------- */

PRIVATE void _TFTP_WRQ_WriteProcessEventLog(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_EVENT_E event)
{
    char szDebugMessage[DEBUG_MSG_SIZE] = {0};

    snprintf(
        szDebugMessage,
        DEBUG_MSG_SIZE,
        "%s :: Current State [%d] :: Processing Event [%d]",
        _STR(TFTP_WRQ_FSM),
        pFSM->m_currentState,
        event);

    Log_Debug(szDebugMessage);
}

PRIVATE void _TFTP_WRQ_AssertValidHandle(TFTP_WRQ_FSM *pFSM)
{
    if (NULL == pFSM)
    {
        Log_Error(_DEBUG_MSG("Invalid handle to " _STR(TFTP_WRQ_FSM)));
    }
}

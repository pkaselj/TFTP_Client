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

// Use macro instead of function to preserve __LINE__ value used by _DEBUG_MSG
#define _TFTP_WRQ_AssertValidHandle(HANDLE)                             \
    if (NULL == HANDLE)                                                   \
    {                                                                   \
        Log_Error(_DEBUG_MSG("Invalid handle to " _STR(TFTP_WRQ_FSM))); \
    }

/* -------------------------------------------------------------------------- */
/*                                  TYPEDEFS                                  */
/* -------------------------------------------------------------------------- */

typedef void (*TFTP_WRQ_Action)(TFTP_WRQ_FSM *pFSM, void *pData);

typedef struct
{
    TFTP_WRQ_STATE_E m_newState;
    TFTP_WRQ_Action m_actionCallback;
} TFTP_WRQ_Transition;

/* -------------------------------------------------------------------------- */
/*                            FUNCTION DEFINITIONS                            */
/* -------------------------------------------------------------------------- */

/* --------------------------------- PRIVATE -------------------------------- */

PRIVATE inline void _TFTP_WRQ_ExecuteAction(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_Action pAction, void *pData);
PRIVATE void _TFTP_WRQ_WriteProcessEventLog(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_EVENT_E event);

PRIVATE TFTP_WRQ_Transition _TFTP_WRQ_GetTransition(TFTP_WRQ_STATE_E currentState, TFTP_WRQ_EVENT_E triggerEvent);
PRIVATE inline void _TFTP_WRQ_Advance(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_STATE_E newState);

PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_IDLE(TFTP_WRQ_EVENT_E triggerEvent);
PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ERROR(TFTP_WRQ_EVENT_E triggerEvent);
PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ATTEMPT_INIT(TFTP_WRQ_EVENT_E triggerEvent);
PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ATTEMPT_DATA(TFTP_WRQ_EVENT_E triggerEvent);
PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ATTEMPT_DATA_LAST_BLOCK(TFTP_WRQ_EVENT_E triggerEvent);

PRIVATE void _TFTP_WRQ_Action_DoVoid(TFTP_WRQ_FSM *pFSM, void *pData);
PRIVATE void _TFTP_WRQ_Action_DoHandleError(TFTP_WRQ_FSM *pFSM, void *pData);

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

    TFTP_WRQ_Transition transition = _TFTP_WRQ_GetTransition(pFSM->m_currentState, event);
    _TFTP_WRQ_Advance(pFSM, transition.m_newState);

    _TFTP_WRQ_WriteProcessEventLog(pFSM, event);
}

void TFTP_WRQ_Reset(TFTP_WRQ_FSM *pFSM)
{
    _TFTP_WRQ_AssertValidHandle(pFSM);
    pFSM->m_currentState = TFTP_WRQ_STATE_IDLE;
    pFSM->m_fIsInitialized = true;
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

PRIVATE inline void _TFTP_WRQ_Advance(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_STATE_E newState)
{
    pFSM->m_currentState = newState;
}

PRIVATE inline void _TFTP_WRQ_ExecuteAction(TFTP_WRQ_FSM *pFSM, TFTP_WRQ_Action pAction, void *pData)
{
    if (NULL == pAction)
    {
        // TODO Somehow identify callback action
        Log_Warning("Invalid action passed!");
        return;
    }

    pAction(pFSM, pData);
}

/* ---------------------------- TRANSITION TABLE ---------------------------- */

// MAIN EVENT DISPATCHER
PRIVATE TFTP_WRQ_Transition _TFTP_WRQ_GetTransition(TFTP_WRQ_STATE_E currentState, TFTP_WRQ_EVENT_E triggerEvent)
{
    switch (currentState)
    {
    case TFTP_WRQ_STATE_IDLE:
        return _TFTP_WRQ_GetTransition_STATE_IDLE(triggerEvent);
    case TFTP_WRQ_STATE_ERROR:
        return _TFTP_WRQ_GetTransition_STATE_ERROR(triggerEvent);
    case TFTP_WRQ_STATE_ATTEMPT_INIT:
        return _TFTP_WRQ_GetTransition_STATE_ATTEMPT_INIT(triggerEvent);
    case TFTP_WRQ_STATE_ATTEMPT_DATA:
        return _TFTP_WRQ_GetTransition_STATE_ATTEMPT_DATA(triggerEvent);
    case TFTP_WRQ_STATE_ATTEMPT_DATA_LAST_BLOCK:
        return _TFTP_WRQ_GetTransition_STATE_ATTEMPT_DATA_LAST_BLOCK(triggerEvent);
    default:
        return (TFTP_WRQ_Transition){
            .m_actionCallback = _TFTP_WRQ_Action_DoVoid,
            .m_newState = currentState};
    }
}

PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_IDLE(TFTP_WRQ_EVENT_E triggerEvent)
{
    switch (triggerEvent)
    {
    case TFTP_WRQ_EVENT_ERROR:
        return (TFTP_WRQ_Transition){
            .m_actionCallback = _TFTP_WRQ_Action_DoHandleError,
            .m_newState = TFTP_WRQ_STATE_ERROR};
    case TFTP_WRQ_EVENT_TIMEOUT:
    case TFTP_WRQ_EVENT_RECV_ACK_EQ_0:
    case TFTP_WRQ_EVENT_RECV_ACK_NOT_0:
    default:
        return (TFTP_WRQ_Transition){
            .m_actionCallback = _TFTP_WRQ_Action_DoVoid,
            .m_newState = TFTP_WRQ_STATE_IDLE};
    }
}

PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ERROR(TFTP_WRQ_EVENT_E triggerEvent)
{
    switch (triggerEvent)
    {
    case TFTP_WRQ_EVENT_ERROR:
        return (TFTP_WRQ_Transition){
            .m_actionCallback = _TFTP_WRQ_Action_DoHandleError,
            .m_newState = TFTP_WRQ_STATE_ERROR};
    case TFTP_WRQ_STATE_ATTEMPT_INIT:
        return (TFTP_WRQ_Transition){
            .m_actionCallback = _TFTP_WRQ_Action_DoVoid,
            .m_newState = TFTP_WRQ_STATE_IDLE};
    case TFTP_WRQ_EVENT_TIMEOUT:
    case TFTP_WRQ_EVENT_RECV_ACK_NOT_0:
    default:
        return (TFTP_WRQ_Transition){
            .m_actionCallback = _TFTP_WRQ_Action_DoVoid,
            .m_newState = TFTP_WRQ_STATE_IDLE};
    }
}

PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ATTEMPT_INIT(TFTP_WRQ_EVENT_E triggerEvent)
{
    UNUSED(triggerEvent);
    return (TFTP_WRQ_Transition){
        .m_actionCallback = _TFTP_WRQ_Action_DoVoid,
        .m_newState = TFTP_WRQ_STATE_ERROR};
}

PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ATTEMPT_DATA(TFTP_WRQ_EVENT_E triggerEvent)
{
    UNUSED(triggerEvent);
    return (TFTP_WRQ_Transition){
        .m_actionCallback = _TFTP_WRQ_Action_DoVoid,
        .m_newState = TFTP_WRQ_STATE_ERROR};
}

PRIVATE inline TFTP_WRQ_Transition _TFTP_WRQ_GetTransition_STATE_ATTEMPT_DATA_LAST_BLOCK(TFTP_WRQ_EVENT_E triggerEvent)
{
    UNUSED(triggerEvent);
    return (TFTP_WRQ_Transition){
        .m_actionCallback = _TFTP_WRQ_Action_DoVoid,
        .m_newState = TFTP_WRQ_STATE_ERROR};
}

/* --------------------------- TRANSITION ACTIONS --------------------------- */

PRIVATE void _TFTP_WRQ_Action_DoVoid(TFTP_WRQ_FSM *pFSM, void *pData)
{
    UNUSED(pData);
    UNUSED(pFSM);
    Log_Debug("Executed: " _STR(_TFTP_WRQ_Action_DoVoid));
}

PRIVATE void _TFTP_WRQ_Action_DoHandleError(TFTP_WRQ_FSM *pFSM, void *pData)
{
    UNUSED(pData);
    UNUSED(pFSM);
    Log_Debug("Executed: " _STR(_TFTP_WRQ_Action_DoHandleError));
}

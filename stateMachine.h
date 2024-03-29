#ifndef STATEMACHINE_H
#define STATEMACHINE_H

typedef enum{
  SM_ENTER_EVENT,
  SM_EXIT_EVENT,
  SM_TICK_EVENT,
  SM_TIMEOUT_EVENT,
} StateMachineEventType;

typedef void (*SMFunction)(StateMachineEventType Event);

typedef struct{
  SMFunction p2fCurrentState;
  SMFunction p2fNextState;
  u32 u32Timer;
}StateMachineType;


void InitStateMachine(StateMachineType *psMachine, SMFunction p2fInitState);
void RunStateMachine(StateMachineType *psMachine);

void ChangeState(StateMachineType *psMachine, SMFunction p2fNextState);
//Changes the next state of SM.

void safeSMCall(StateMachineType *psMachine, StateMachineEventType Event);


void SetTimeout(StateMachineType *psMachine, u32 u32Timeout);

#endif

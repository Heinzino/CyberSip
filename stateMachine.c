#include "configuration.h"


void safeSMCall(StateMachineType *psMachine, StateMachineEventType Event){
  if(psMachine->p2fCurrentState != NULL){
    psMachine->p2fCurrentState(Event);
  }
}


void InitStateMachine(StateMachineType *psMachine, SMFunction p2fInitState){
  psMachine->p2fCurrentState = NULL;
  psMachine->p2fNextState = p2fInitState; 
  psMachine->u32Timer = 0;
}

void RunStateMachine(StateMachineType *psMachine){

  safeSMCall(psMachine,SM_TICK_EVENT);

  if(psMachine->u32Timer > 0){ //There was a time set and is counting down
    if(--(psMachine->u32Timer) == 0 && psMachine->p2fNextState == psMachine->p2fCurrentState){
      //If I count down and timer expires
      safeSMCall(psMachine,SM_TIMEOUT_EVENT);
    }
  }

  if(psMachine -> p2fNextState != psMachine -> p2fCurrentState){
    safeSMCall(psMachine, SM_EXIT_EVENT);
    psMachine->p2fCurrentState = psMachine->p2fNextState; 
    //Next State itself doesn't change unless altered or by event in p2f
    psMachine->u32Timer = 0;
    safeSMCall(psMachine, SM_ENTER_EVENT);    
  }

}

void ChangeState(StateMachineType *psMachine, SMFunction p2fNextState){
    psMachine->p2fNextState = p2fNextState;
}

void SetTimeout(StateMachineType *psMachine, u32 u32Timeout){
  psMachine->u32Timer = u32Timeout;
}

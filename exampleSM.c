static StateMachineType UserApp1SM;

static void HomeState(StateMachineEventType Event);
static void SetTimeState(StateMachineEventType Event);
static void SetAlarmState(StateMachineEventType Event);
static void AlarmState(StateMachineEventType Event);

void UserApp1Initialize(void)
{
  InitStateMachine(&UserApp1SM, HomeState);
}

void UserApp1RunActiveState(void)
{
  RunStateMachine(&UserApp1SM);
}

void HomeState(StateMachineEventType Event){
  
  switch(Event){
    
  case SM_ENTER_EVENT:
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOn(LCD_BLUE);
    AckAllBtns();
    displayTime(timeDisplayed);
    LedOff(BLUE);
    LedOff(YELLOW);
    LedOff(RED);
    LedOff(WHITE);
    PWMAudioOff(BUZZER1);
    PWMAudioOff(BUZZER2);
    break;
    
  case SM_TICK_EVENT:
    updateTime();
    if(alarmTime == timeDisplayed && alarmSet){
      alarmSet = FALSE;
      ChangeState(&UserApp1SM,AlarmState);
    }
    
    if(WasButtonPressed(BUTTON0)){
      ButtonAcknowledge(BUTTON0);
      ChangeState(&UserApp1SM, SetTimeState);
    }
    if(WasButtonPressed(BUTTON3)){
      ButtonAcknowledge(BUTTON3);
      ChangeState(&UserApp1SM,SetAlarmState);
    }
    break;
  case SM_TIMEOUT_EVENT:
    ChangeState(&UserApp1SM, HomeState);
    break;
    
  case SM_EXIT_EVENT:
    break;


  }  
}

//SUPER_LOOP    
while(1){
    UserApp1RunActiveState();
}

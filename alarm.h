/**
 *  Project:    Zeitschaltuhr
 *  Author:     Franz Lorenz
 *  Copyright:  Franz Lorenz, Kelheim
 */

//----------------------------------------------------------
//  SYSTEM

//----------------------------------------------------------
//  DEFINES
#define   ALARM_DAY_SUNDAY            0
#define   ALARM_DAY_MONDAY            1
#define   ALARM_DAY_TUESDAY           2
#define   ALARM_DAY_WEDNESDAY         3
#define   ALARM_DAY_THURSDAY          4
#define   ALARM_DAY_FRIDAY            5
#define   ALARM_DAY_SATURDAY          6
//
#define   ALARM_ON                    1
#define   ALARM_OFF                   0

//----------------------------------------------------------
//  TYPEDEFs
typedef struct { int iOnHour; int iOnMin; int iOffHour; int iOffMin; } tAlarmDay;
typedef struct { String sName; tAlarmDay *pAlarm; } tAlarm;
typedef struct { boolean bActive; int iOffHour; int iOffMin; } tTimer;

//----------------------------------------------------------
//  GLOBALS
tAlarmDay AlarmNormal[] = 
{ 
  { 7, 0, 17, 0 },  //So
  { 7, 0, 17, 0 },  //Mo
  { 7, 0, 17, 0 },  //Di
  { 7, 0, 17, 0 },  //Mi
  { 7, 0, 17, 0 },  //Do
  { 7, 0, 17, 0 },  //Fr
  { 7, 0, 17, 0 },  //Sa
};

tAlarmDay AlarmWeekend[] = 
{ 
  { 9, 0, 16, 0 },  //So
  { 7, 0, 17, 0 },  //Mo
  { 7, 0, 17, 0 },  //Di
  { 7, 0, 17, 0 },  //Mi
  { 7, 0, 17, 0 },  //Do
  { 9, 0, 16, 0 },  //Fr
  { 9, 0, 16, 0 },  //Sa
};

tAlarmDay AlarmVacation[] = 
{ 
  { 9, 0, 16, 0 },  //So
  { 9, 0, 16, 0 },  //Mo
  { 9, 0, 16, 0 },  //Di
  { 9, 0, 16, 0 },  //Mi
  { 9, 0, 16, 0 },  //Do
  { 9, 0, 16, 0 },  //Fr
  { 9, 0, 16, 0 },  //Sa
};

tAlarm  alarmArray[]      = { { "Normal",AlarmNormal }, { "Wochenende",AlarmWeekend }, { "Urlaub",AlarmVacation }, { "",NULL } };
int     alarmArrayNr      = 0;
tTimer  alarmTimer        = { false, 0, 0 };
boolean alarmAlarmActive  = false; 

//----------------------------------------------------------
//  LOCAL FUNCTIONS

//----------------------------------------------------------
//  API FUNCTIONS

/**
 *  This function initialize the NTP service.
 */
void alarmInit( void )
{
  alarmTimer.bActive = false;                               //deactivate "running" timer
  alarmAlarmActive = false;                                 //set no-alarm active
}

/**
 *  This function returns the state of alarm.
 *  @return boolean true => alarm is active, false => alarm deactive
 */
boolean alarmIsAlarmActive( void )
{
  return alarmAlarmActive;
}

/**
 *  This function returns the state of timer.
 *  @return boolean true => timer is active, false => timer stopped
 */
boolean alarmIsTimerActive( void )
{
  return alarmTimer.bActive;
}

/**
 *  This function checks the state of alarm.
 *  @return   int     see ALARM_xxx defines
 */
int alarmHandle( int iDay, int iHour, int iMin )
{
  int iRet = ALARM_OFF;
  int iCtr = 0;
  int iVal = iHour*60+iMin;
  int iOn = 0;
  int iOff = 0;
  //
  // check timer
  if( alarmTimer.bActive )
  {
    iRet = ALARM_ON;
    if( iHour >= alarmTimer.iOffHour )
    {
      if( iMin >= alarmTimer.iOffMin )
      {
        alarmTimer.bActive = false;
        iRet = ALARM_OFF;
      }
    }
  }
  //
  // check pre-set alarms
  if( alarmArrayNr >= 0 )
  {
    tAlarmDay Active = alarmArray[alarmArrayNr].pAlarm[iCtr];
    iOn  = Active.iOnHour*60 + Active.iOnMin;
    iOff = Active.iOffHour*60 + Active.iOffMin;
    if( ( iVal >= iOn ) && ( iVal < iOff ) )
    {
      alarmAlarmActive = true;
      iRet |= ALARM_ON;
    }
    else
    {
      alarmAlarmActive = false;
    }
  } //if()
  //
  return iRet;
}

/**
 *  This function sets a timer.
 *  @param  iMinutes    timer from now to now+iMinutes     
 */
void alarmSetTimer( int iMinutes )
{
  tDateTime  now;
  memcpy( (void*)&now, (void*)&DateTime, sizeof( tDateTime ) );
  if( iMinutes > 0 )
  {
    alarmTimer.iOffHour = now.iHour;
    alarmTimer.iOffMin  = now.iMin + iMinutes;
    while( alarmTimer.iOffMin > 59 )
    {
      alarmTimer.iOffMin -= 60;
      alarmTimer.iOffHour++;
      if( alarmTimer.iOffHour > 23 )
        alarmTimer.iOffHour = 0;
    }
    alarmTimer.bActive = true;
  }
  else
  {
    alarmTimer.bActive = false;
  }
}

/**
 *  This function returns the current state of the 
 *  timer.
 *  @return   String    gets the current state
 */
String alarmGetTimer( void )
{
  String sRet = "AUS";
  if( alarmTimer.bActive )
  {
    char cBuf[20];
    sprintf( cBuf, "%02d:%02d", alarmTimer.iOffHour, alarmTimer.iOffMin );
    sRet = String( cBuf );
  }
  return sRet;  
}

/**
 *  This function returns 
 *  @param  iAlarm    number of alarm [0...n[
 *  @return tAlarm    alarm
 */
tAlarm alarmGetAlarmArray( int iAlarm )
{
  return alarmArray[iAlarm];
}

int alarmGetActiveAlarmArrayNr( void )
{
  return alarmArrayNr;
}

void alarmSetActiveAlarmArrayNr( int iNr )
{
  int iNrMax=0;
  while( alarmArray[iNrMax].sName.length() > 0 )
    iNrMax++;
  if( ( iNr >= 0 ) && ( iNr < iNrMax ) )
    alarmArrayNr = iNr;
}

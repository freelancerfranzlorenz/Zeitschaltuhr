/**
 *  Project:    Zeitschaltuhr
 *  Author:     Franz Lorenz
 *  Copyright:  Franz Lorenz, Kelheim
 */

//----------------------------------------------------------
//  SYSTEM
#if !defined( ESP8266 )
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif
// These define's must be placed at the beginning before #include "ESP8266TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
// Select a Timer Clock
#define USING_TIM_DIV1                false                 //for shortest and most accurate timer
#define USING_TIM_DIV16               true                  //for medium time and medium accurate timer
#define USING_TIM_DIV256              false                 //for longest timer but least accurate. Default
#include "ESP8266TimerInterrupt.h"

//----------------------------------------------------------
//  DEFINES
#define   DAYOFWEEK_SUNDAY            0
#define   DAYOFWEEK_MONDAY            1
#define   DAYOFWEEK_TUESDAY           2
#define   DAYOFWEEK_WEDNESDAY         3
#define   DAYOFWEEK_THURSDAY          4
#define   DAYOFWEEK_FRIDAY            5
#define   DAYOFWEEK_SATURDAY          6
#define   DAYOFWEEK_MAX               7

//----------------------------------------------------------
//  TYPEDEFs
typedef struct
{
  int   iHour;  int   iMin;   int   iSec;
  int   iDay;   int   iMonth; int   iYear;
  int   iDayWeek;
} tDateTime;

//----------------------------------------------------------
//  GLOBALS
ESP8266Timer        ITimer;
volatile tDateTime  DateTime;

//----------------------------------------------------------
//  LOCAL FUNCTIONS
/**
 *  Interupt functions
 */
void IRAM_ATTR datetimeTimerHandler()
{
  if( DateTime.iSec >= 59 )
  {
    DateTime.iSec = 0;
    if( DateTime.iMin >= 59 )
    {
      DateTime.iMin = 0;
      if( DateTime.iHour >= 23 )
      {
        DateTime.iHour = 0;
        DateTime.iDayWeek++;
        if( DateTime.iDayWeek >= DAYOFWEEK_MAX )
          DateTime.iDayWeek = DAYOFWEEK_MONDAY;
      }
      else
        DateTime.iHour++;
    }
    else
      DateTime.iMin++;
  }
  else
    DateTime.iSec++;
}

//----------------------------------------------------------
//  API FUNCTIONS

/**
 *  This function initialize the clock, calendar
 *  and the timer 1 second interrupt function.
 */
void datetimeInit( void )
{
  DateTime.iHour    = 12;
  DateTime.iMin     = 0;
  DateTime.iSec     = 0;
  DateTime.iDay     = 1;
  DateTime.iMonth   = 1;
  DateTime.iYear    = 2024;
  DateTime.iDayWeek = DAYOFWEEK_MONDAY;
  // Interval in microsecs
  if( ITimer.attachInterruptInterval( 1000*1000, datetimeTimerHandler ) )
    Serial.println( "datetimeInit() runs" );
  else
    Serial.println( "datetimeInit() - timer problem" );
}

/**
 *  This function returns the current day_of_week as string.
 *  @return   string    day_of_week
 */
String datetimeGetDayOfWeek( void )
{
  String sRet = "";
  switch( DateTime.iDayWeek ) 
  {
    case 0: sRet = "SON";   break;
    case 1: sRet = "MON";   break;
    case 2: sRet = "DIE";   break;
    case 3: sRet = "MIT";   break;
    case 4: sRet = "DON";   break;
    case 5: sRet = "FRE";   break;
    case 6: sRet = "SAM";   break;
    default:                break;
  }
  return sRet;
}

/**
 *  This function sets the time
 *  @param  iHour     hour
 *  @param  iMin      minutes
 *  @param  iSec      seconds
 */
void datetimeSetTime( int iHour, int iMin, int iSec )
{
  DateTime.iHour    = iHour;
  DateTime.iMin     = iMin;
  DateTime.iSec     = iSec;
}

/**
 *  This function sets the day-of-week number.
 *  @param  iDayOfWeek    day of week (see DAYOFWEEK defines)
 */
void datetimeSetDayOfWeek( int iDayOfWeek )
{
  DateTime.iDayWeek = iDayOfWeek;
}

/**
 *  This function returns a string with the current time.
 *  @return String
 */
String datetimeGet( void )
{
  char cBuf[20];
  sprintf( cBuf, "%s %02d:%02d:%02d", datetimeGetDayOfWeek(), DateTime.iHour, DateTime.iMin, DateTime.iSec );
  String sRet( cBuf );
  return sRet;
}

/**
 *  This function update the current date.
 *  @param  unsigned long   epoch time
 */
void datetimeSetEpochTime( unsigned long ulEpochTime )
{

}


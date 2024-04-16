/**
 *  Project:    Zeitschaltuhr
 *  Author:     Franz Lorenz
 *  Copyright:  Franz Lorenz, Kelheim
 */


#define   VERSION         "1.0"                  

//----------------------------------------------------------
//  INCLUDES
#include "config.h"                     //configuration for all following modules
#include "datetime.h"
#include "relais.h"
#include "wifi.h"
#include "ntp.h"
#include "alarm.h"
#include "websrv.h"
#include "led.h" 

//----------------------------------------------------------
//  DEFINES

//----------------------------------------------------------
//  LOCALS
int               iSec = -1;

//----------------------------------------------------------
//  ARDUINO FRAMEWORK

/**
 *  This function is called, when the system starts.
 */
void setup()
{
  ledInit();
  relaisInit();
  datetimeInit();
  //
  Serial.begin( 115200 );
  while( !Serial );
  delay( 300 );
  //
  wifiInit();
  ntpInit();
  websrvInit();
  alarmInit();
} //void setup()

/**
 *  This function is called, when the system starts.
 */
void loop()
{
  if( Serial.available() > 0 ) 
  {
    String sDebug = Serial.readString();
    sDebug.trim();
    if( sDebug == "ntpstop" )
      ntpSetUpdate( false );
    else if( sDebug == "ntpstart" )
      ntpSetUpdate( true );
    else if( sDebug.startsWith( "time" ) )
    {
      ntpSetUpdate( false );
      sscanf( sDebug.c_str(), "time%d:%d", &DateTime.iHour, &DateTime.iMin ) ;
    }
  }  
  //
  ledHandle();
  //
  if( DateTime.iSec != iSec )
  {
    if( ( DateTime.iHour >= WIFI_AP_SWITCHON_HOUR ) && ( DateTime.iHour < WIFI_AP_SWITCHOFF_HOUR ) )
    {
      if( wifiIsEnabled() )
      {
        wifiHandle();
        ntpHandle();
        websrvHandle();
      }
      else
      {
        wifiStartup();
        ntpInit();
      }
    }
    else
    {
      if( wifiIsEnabled() )
      {
        wifiShutdown();
      }
    }
    //
    Serial.printf( "%s %02d:%02d:%02d - ", datetimeGetDayOfWeek(), DateTime.iHour, DateTime.iMin, DateTime.iSec );
    relaisSet( ALARM_ON == alarmHandle( DateTime.iDay, DateTime.iHour, DateTime.iMin ) );
    Serial.printf( "Relais:%d - ", ( int )relaisGet() );
    Serial.printf( "WifiEnable:%d - ", ( int )wifiIsEnabled() );
    Serial.println( "" );
    //
    iSec = DateTime.iSec;
  }
} //void loop()

/*
void wifiShutdown( void )
void wifiStartup( void )
*/


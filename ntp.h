/**
 *  Project:    Zeitschaltuhr
 *  Author:     Franz Lorenz
 *  Copyright:  Franz Lorenz, Kelheim
 *
 *  Uses arduino library   by Fabrice Weinberg
 *    "https://github.com/arduino-libraries/NTPClient" 
 *    version 3.2.1
 */

//----------------------------------------------------------
//  SYSTEM
#include  <NTPClient.h>
#include  <WiFiUdp.h>

//----------------------------------------------------------
//  DEFINES

//----------------------------------------------------------
//  TYPEDEFs

//----------------------------------------------------------
//  GLOBALS
WiFiUDP           ntpUDP;
const long        ntpUtcOffsetInSec = 2*3600;
NTPClient         ntpTimeClient( ntpUDP, NTP_SERVER_URL, ntpUtcOffsetInSec );
int               ntpState;
boolean           ntpUpdate = true;

//----------------------------------------------------------
//  LOCAL FUNCTIONS

//----------------------------------------------------------
//  API FUNCTIONS

/**
 *  This function initialize the NTP service.
 */
void ntpInit( void )
{
  ntpState = 0;
}

/**
 *  This function calls the NTP server.
 */
void ntpHandle( void )
{
  switch( ntpState )  
  {
    case 0 :
      if( wifiIsConnected() )
        ntpState++;
      break;
    case 1 :
      ntpTimeClient.begin();
      ntpState++;
      break;
    case 2 :
      if( wifiIsConnected() )
      {
        ntpTimeClient.update();
        if( ntpUpdate )
        {
          datetimeSetTime( ntpTimeClient.getHours(), ntpTimeClient.getMinutes(), ntpTimeClient.getSeconds() );
          datetimeSetEpochTime( ntpTimeClient.getEpochTime() );
          datetimeSetDayOfWeek( ntpTimeClient.getDay() );
        }
      }
      else
        ntpState = 0;
      break;
    default:
      ntpState = 0;
      break;
  }
}

/**
 *  This function enables/disables the internal time/date update
 *  of the ntp client.
 *  @param  bUpdate     true => enables ntp update, false => disables
 */
void ntpSetUpdate( boolean bUpdate )
{
  ntpUpdate = bUpdate;
}

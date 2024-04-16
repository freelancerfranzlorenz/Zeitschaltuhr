/**
 *  Project:    Zeitschaltuhr
 *  Author:     Franz Lorenz
 *  Copyright:  Franz Lorenz, Kelheim
 *
 */

//----------------------------------------------------------
//  SYSTEM
#include  <WebSocketsServer.h>
#include  <ESP8266WebServer.h>

//----------------------------------------------------------
//  DEFINES

//----------------------------------------------------------
//  TYPEDEFs

//----------------------------------------------------------
//  GLOBALS
int               websrvState = 0;
ESP8266WebServer  websrvServer( 80 );
WebSocketsServer  websrvSocket( 81 );

//----------------------------------------------------------
//  LOCAL FUNCTIONS

void websrvHandleRoot( void )
{
  String sState = "AUS";
  if( relaisGet() )   sState = "EIN - ";
  if( alarmIsAlarmActive() )  sState += "Alarm";
  if( alarmIsTimerActive() )  sState += "Timer";
  //
  String sPage = "<html>\n";
  sPage += "<head>\n";
  sPage += "<meta name='viewport' content='width=device-width,initial-scale=1'>\n";
  sPage += "<link rel='manifest' href='data:application/manifest+json,{\"name\":\"L&uuml;ftung\",\"short_name\":\"L&uuml;ftung\",\"orientation\":\"portrait\",\"display\":\"standalone\"}' />\n";
  sPage += "<style>\n";
  sPage += ".area{border-radius:1vw;background-color:#eee;margin:1vw 1vw 1vw 1vw;padding:0.5em 1em 2vw 1em;}\n";
  sPage += "button{border-radius:0.5em;background-color:#ddd;width:100%;font-size:1.1em;padding:0.5em;cursor:pointer;margin-bottom:0.1em;}\n";
  sPage += ".ctr{text-align:center;}\n";
  sPage += ".AUS{background-color:green;}\n";
  sPage += ".EIN{background-color:red;}\n";
  sPage += ".AKT{background-color:#cfc;}\n";
  sPage += "</style>\n";
  sPage += "</head>\n";
  sPage += "<body>\n";
  sPage += "<h2 class='ctr "+sState+"' onclick='location.reload();' style='color:white;padding:0.5em;'>Zeitschaltuhr - "+sState+"</h2>\n";
  sPage += "<div class='area'>\n";
  sPage += "<p class='ctr'>Timer "+alarmGetTimer()+"</p>\n";
  sPage += "<button onclick='timer(15);'>Timer auf 15 Minuten setzen</button>\n";
  sPage += "<button onclick='timer(30);'>Timer auf 30 Minuten setzen</button>\n";
  sPage += "<button onclick='timer(60);'>Timer auf 1 Stunde setzen</button>\n";
  sPage += "<button onclick='timer(0);'>Timer ausschalten</button>\n";
  sPage += "</div>\n";
  sPage += "<br/>\n";
  //
  sPage += "<div class='area'>\n";
  sPage += "<p class='ctr'>Alarm setzen</p>\n";
  if( 0 == alarmGetActiveAlarmArrayNr() ) sState = "AKT"; else sState = "";
  sPage += "<button onclick='alarm(0);' class='"+sState+"'>Alarm auf 'Normal' setzen</button>\n";
  if( 1 == alarmGetActiveAlarmArrayNr() ) sState = "AKT"; else sState = "";
  sPage += "<button onclick='alarm(1);' class='"+sState+"'>Alarm auf 'Wochenende' setzen</button>\n";
  if( 2 == alarmGetActiveAlarmArrayNr() ) sState = "AKT"; else sState = "";
  sPage += "<button onclick='alarm(2);' class='"+sState+"'>Alarm auf 'Urlaub' setzen</button>\n";
  sPage += "</div>\n";
  sPage += "<br/>\n";
  //
  int     iAlarm = 0;
  tAlarm  Alarm = alarmGetAlarmArray( iAlarm );
  String  sDays = "MONDIEMITDONFRESAMSON";
  char    cLine[80];
  while( Alarm.sName.length() > 0 )
  {
    sPage += "<div class='area'>\n";
    sPage += "<h4>"+Alarm.sName;
    if( alarmGetActiveAlarmArrayNr() == iAlarm )
      sPage += " - AKTIV";
    sPage += "</h4>\n";
    sPage += "<table width='100%'>\n";
    sPage += "<tr><th>Tag</th><th>Einschaltzeit</th><th>Ausschaltzeit</th></tr>\n";
    for( int iDay=0; iDay < 7; iDay++ )
    {
      sprintf( cLine, "<tr><td>%s</td><td>%02d:%02d</td><td>%02d:%02d</td></tr>\n", 
                sDays.substring( iDay*3, iDay*3+3 ).c_str(), 
                Alarm.pAlarm->iOnHour, Alarm.pAlarm->iOnMin, 
                Alarm.pAlarm->iOffHour, Alarm.pAlarm->iOffMin );
      sPage += String( cLine );
    }
    sPage += "</table>\n";
    sPage += "</div>\n";
    iAlarm++;
    Alarm = alarmGetAlarmArray( iAlarm );
  }
  //
  sPage += "<br/>\n";
  sPage += "<div class='area'>\n";
  sPage += "<table width='100%'>\n";
  sPage += "<tr><td>IP Addresse</td><td>"+wifiGetIP()+"</td></tr>\n";
  sPage += "<tr><td>AP Addresse</td><td>"+wifiGetAPIP()+"</td></tr>\n";
  sPage += "<tr><td>Uhrzeit</td><td>"+datetimeGet()+"</td></tr>\n";
  sPage += "<tr><td>Timer</td><td>"+alarmGetTimer()+"</td></tr>\n";
  sPage += "<tr><td>Relais Zustand</td><td>"+sState+"</td></tr>\n";
  sPage += "</table>\n";
  sPage += "</div>\n";
  sPage += "<small>App v" VERSION " &copy; 2024 Franz Lorenz</small>\n";
  sPage += "<script>\n";
  sPage += "function click(sPar,sVal){ let sLink='/click?'+sPar+'='+sVal; var xhttp=new XMLHttpRequest(); xhttp.open('GET\',sLink,true); xhttp.send(); }\n";
  sPage += "function timer(nVal){ click(\"timer\",nVal ); setTimeout(function(){location.reload();},1000); }\n";
  sPage += "function alarm(nVal){ click(\"alarm\",nVal ); setTimeout(function(){location.reload();},1000); }\n";
  sPage += "</script>\n";
  sPage += "</body>\n";
  sPage += "</html> \n";
  websrvServer.send( 200, "text/html", sPage+"\r\n" );
}

void websrvHandleClick( void )
{
  String sDebug = "";
  for( int i=0; i < websrvServer.args(); i++ )
  {
    sDebug += "Arg no" + ( String )i + " –> ";
    sDebug += websrvServer.argName( i ) + ": ";
    sDebug += websrvServer.arg(i) + "\n";
  } 
  Serial.print( "websrvHandleClick() - " );
  Serial.println( sDebug );
  //
  if( websrvServer.argName( 0 ) == "timer" )
  {
    String sVal = websrvServer.arg( 0 );
    sVal.trim();
    Serial.println( "alarmSetTimer '"+sVal+"'" );
    alarmSetTimer( sVal.toInt() );
  }
  if( websrvServer.argName( 0 ) == "alarm" )
  {
    String sVal = websrvServer.arg( 0 );
    sVal.trim();
    Serial.println( "alarmSetActiveAlarmArrayNr '"+sVal+"'" );
    alarmSetActiveAlarmArrayNr( sVal.toInt() );
  }
  websrvHandleRoot();
}

/**
 *  This function returns all relevant information of the
 *  device.
 *  @return String    string with all informations
 */
String websrvSocketGet( void )
{
  String sRet = datetimeGet();
  if( relaisGet() )
    sRet += "|1";
  else
    sRet += "|0";
  if( alarmIsAlarmActive() )
    sRet += "|1";
  else
    sRet += "|0";
  if( alarmIsTimerActive() )
    sRet += "|1";
  else
    sRet += "|0";
  sRet += "|"+alarmGetTimer();
  sRet += "|"+wifiGetIP();
  sRet += "|"+wifiGetAPIP();
  return sRet;
}

/**
 *  This function handles all events form the websocket server.
 */
void websrvSocketEvent( uint8_t u8Num, WStype_t Type, uint8_t *pu8Payload, size_t nLength )
{
  Serial.printf( "webSocketEvent(%d, %d, ...)\r\n", u8Num, Type );
  switch( Type ) 
  {
    case WStype_DISCONNECTED:
      Serial.printf( "[%u] Disconnected!\r\n", u8Num );
      break;
    case WStype_CONNECTED:
      {
        Serial.printf( "[%u] Connected url: %s\r\n", u8Num, pu8Payload );
        // websrvSocket.sendTXT(num, LEDON, strlen(LEDON));
      }
      break;
    case WStype_TEXT:
      Serial.printf( "[%u] get Text: %s\r\n", u8Num, pu8Payload );
      if( 0 == strcmp( ( char* )pu8Payload, "get" ) )
      {
        String sRet = websrvSocketGet();
        websrvSocket.sendTXT( u8Num, sRet );
      }
      break;
    case WStype_BIN:
      Serial.printf( "[%u] get binary length: %u\r\n", u8Num, nLength );
      break;
    default:
      Serial.printf( "Invalid WStype [%d]\r\n", Type );
      break;
  }
}

//----------------------------------------------------------
//  API FUNCTIONS

/**
 *  This function initialize the NTP service.
 */
void websrvInit( void )
{
  websrvState = 0;
  websrvServer.on( "/", websrvHandleRoot );
  websrvServer.on( "/click", websrvHandleClick );
  websrvServer.begin();
  websrvSocket.begin();
  websrvSocket.onEvent( websrvSocketEvent );  
}

/**
 *  This function calls the NTP server.
 */
void websrvHandle( void )
{
  websrvServer.handleClient();
  websrvSocket.loop();
  switch( websrvState )  
  {
    default:
      websrvState = 0;
      break;
  }
}

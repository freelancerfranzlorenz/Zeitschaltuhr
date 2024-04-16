/**
 *  Project:    Zeitschaltuhr
 *  Author:     Franz Lorenz
 *  Copyright:  Franz Lorenz, Kelheim
 */

//----------------------------------------------------------
//  SYSTEM

//----------------------------------------------------------
//  DEFINES

// led.h
#define   LED_GPIO                  2
#define   LED_ON_TIME_10MS          30
#define   LED_OFF_TIME_10MS         5

// ntp.h
#define   NTP_SERVER_URL            "pool.ntp.org"

// relais.h
#define   RELAIS_GPIO               12
#define   RELAIS_STATE_DEFAULT      false

// wifi.h
#define   WIFI_AP_SSID              <YOUR ACCESSPOINT SSID>
#define   WIFI_AP_PASSWORD          <YOUR ACCESSPOINT PASSWORD>
//
#define   WIFI_SSID                 <YOUR LOCAL WIFI NETWORK SSID>
#define   WIFI_PASSWORD             <YOUR LOCAL WIFI NETWORK PASSWORD>
//
#define   WIFI_AP_SWITCHOFF_HOUR    22
#define   WIFI_AP_SWITCHON_HOUR     5

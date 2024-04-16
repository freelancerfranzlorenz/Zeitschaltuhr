/**
 *  Project:    Zeitschaltuhr
 *  Author:     Franz Lorenz
 *  Copyright:  Franz Lorenz, Kelheim
 */

//----------------------------------------------------------
//  SYSTEM

//----------------------------------------------------------
//  DEFINES

//----------------------------------------------------------
//  TYPEDEFs

//----------------------------------------------------------
//  GLOBALS
boolean   relaisOn = false;

//----------------------------------------------------------
//  API FUNCTIONS

/**
 *  This function initialize the relais interface.
 */
void relaisInit( void )
{
  pinMode( RELAIS_GPIO, OUTPUT );
  digitalWrite( RELAIS_GPIO, LOW );
  relaisOn = RELAIS_STATE_DEFAULT;
}

/**
 *  This function sets the relais state.
 *  @param  bOn     on
 */
void relaisSet( boolean bOn )
{
  relaisOn = bOn;
  if( relaisOn )
    digitalWrite( RELAIS_GPIO, HIGH );
  else
    digitalWrite( RELAIS_GPIO, LOW );
}

/**
 *  This function returns the current state.
 *  @return   boolean   state
 */
boolean relaisGet( void )
{
  return relaisOn;
}

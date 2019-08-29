//
//  Copyright 2013 Paradise Arcade Shop, ParadiseArcadeShop.com  
//  All rights reserved.  Use is subject to license terms.
//
//  Code is provided for entertainment purposes and use with the Kaimana controller.
//  Code may be copied, modified, resused with this Copyright notice.
//  No commercial use without written permission from Paradise Arcade Shop.
//
//  Paradise Arcade Shop Kaimana LED Driver Board
//  Initial Release October 15, 2013
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  Kaimana example based on original source released by ParadiseArcadeShop.com October 15, 2013
//
//  Created:  October 24, 2013    zonbipanda // gmail.com  -- Arduino 1.0.5 Support
//  Revised:  October 29, 2013    zonbipanda // gmail.com
//  Revised:  April   11, 2015    zonbipanda // gmail.com  -- Arduino 1.6.3 Support
//

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"

// local function declarations
int  pollSwitches(void);
int tourneypollSwitches(void);
void setLEDRandomColor(int index);
boolean tournamentMode = false;
int holdTimeout = 0;
int aniTimeout= 0;
int selection = 0;

// ParadiseArcadeShop.com Kaimana features initialzied when Kaimana class instantiated
Kaimana kaimana;

// the setup routine runs first and once each time power is applied to the Kaimana board
void setup() 
{                
  // light up all leds at boot to demonstrate everything is functional
	_STARTUP_ANIMATION
}

// the loop routine repeats indefinitely and executes immediately following the setup() function
void loop() 
{
  unsigned long  ulTimeout;
  
  // initialize timeout value to now + some seconds
  ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );

  // infinite loop of read switches, update LEDs and idle animation when necessary
  while(true)
  {
	  if (tournamentMode != true)
		{
			// active -- poll switches and update leds
			if( pollSwitches(ulTimeout) != 0 )
			{
				// some switches were active so reset idle timeout to now + some seconds
				ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
			}
			else
			{
				// no switches active so test for start of idle timeout  
				if( millis() > ulTimeout )
				{
				_IDLE_ANIMATION
				}          
			}
		}
    else
    {
      if( tourneypollSwitches() != 0 )
      {
        // some switches were active so reset idle timeout to now + some seconds
        ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
      }
    }    
    // delay a little to avoid flickering (yea, updates happens really, really fast!)
    //delay( MAIN_LOOP_DELAY );
  } 
}

// ==============================================================
//
//  local functions start here
//
// ==============================================================

int pollSwitches(unsigned long  ulTimeout)
{
  static int  iLED[LED_COUNT];
  static int  iActiveSwitchCount;
  static int  i;  
  static int  j;  
  static int  firsttime;
  static uint16_t  joystickDirection;
  static uint16_t  switchActivity;

  // reset LED status
  if (firsttime == 1)
  {
    for(i=0;i<LED_COUNT;++i)
    {
      iLED[i] = false;
      firsttime = 0;
    }
  }

  joystickDirection = ATTACK_NONE;
  // complex special case for joystick but it's worth the effort
  if(!digitalRead(PIN_RIGHT))
    joystickDirection |= ATTACK_RIGHT;
  if(!digitalRead(PIN_LEFT))
    joystickDirection |= ATTACK_LEFT;
  if(!digitalRead(PIN_DOWN))
    joystickDirection |= ATTACK_DOWN;
  if(!digitalRead(PIN_UP))
    joystickDirection |= ATTACK_UP;
  
  // clear results for switch activity
  switchActivity = ATTACK_NONE;

  // test switch and set LED based on result
  if(!digitalRead(PIN_P1))
  {
    switchActivity |= ATTACK_P1;
    
    // switch is active
    if(iLED[LED_P1] == true)
    {
      //maintain color while switch is active
      iLED[LED_P1] = true;
    }
    else
    {
      // select new color when switch is first activated
      // setLEDRandomColor(LED_P1);
      kaimana.setLED(LED_P1, BLACK);
      iLED[LED_P1] = true;
      kaimana.setLED(LED_2P1, _IDLE_BTN_2P1_COLOR);
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_P1, _IDLE_BTN_P1_COLOR);    
      iLED[LED_P1] = false;
      kaimana.setLED(LED_2P1, _IDLE_BTN_2P1_COLOR);
      iLED[LED_2P1] = false;
  }

  // test switch and set LED based on result
  if(!digitalRead(PIN_P2))
  {
    switchActivity |= ATTACK_P2;
    
    // switch is active
    if(iLED[LED_P2] == true)
    {
      //maintain color while switch is active
      iLED[LED_P2] = true;
    }
    else
    {
      // select new color when switch is first activated
      // setLEDRandomColor(LED_P2);
      kaimana.setLED(LED_P2, BLACK);
      iLED[LED_P2] = true;
      kaimana.setLED(LED_2P2, _IDLE_BTN_2P2_COLOR);
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_P2, BLACK);
      kaimana.setLED(LED_P2, _IDLE_BTN_P2_COLOR);    
      iLED[LED_P2] = false;
      kaimana.setLED(LED_2P2, _IDLE_BTN_2P2_COLOR);
      iLED[LED_2P2] = false;
  }

  // test switch and set LED based on result
  if(!digitalRead(PIN_P3))
  {
    switchActivity |= ATTACK_P3;
    
    // switch is active
    if(iLED[LED_P3] == true)
    {
      //maintain color while switch is active
      iLED[LED_P3] = true;
    }
    else
    {
      // select new color when switch is first activated
      // setLEDRandomColor(LED_P3);
      kaimana.setLED(LED_P3, BLACK);
      iLED[LED_P3] = true;
      kaimana.setLED(LED_2P3, _IDLE_BTN_2P3_COLOR);
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_P3, BLACK);    
      kaimana.setLED(LED_P3, _IDLE_BTN_P3_COLOR);    
      iLED[LED_P3] = false;
      kaimana.setLED(LED_2P3, _IDLE_BTN_2P3_COLOR);
      iLED[LED_2P3] = false;
  }

  // test switch and set LED based on result
  if(!digitalRead(PIN_P4))
  {
    switchActivity |= ATTACK_P4;
    
    // switch is active
    if(iLED[LED_P4] == true)
    {
      //maintain color while switch is active
      iLED[LED_P4] = true;
    }
    else
    {
      // select new color when switch is first activated
      // setLEDRandomColor(LED_P4);
      kaimana.setLED(LED_P4, BLACK);
      iLED[LED_P4] = true;
      kaimana.setLED(LED_2P4, _IDLE_BTN_2P4_COLOR);
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_P4, BLACK);    
      kaimana.setLED(LED_P4, _IDLE_BTN_P4_COLOR);    
      iLED[LED_P4] = false;
      kaimana.setLED(LED_2P4, _IDLE_BTN_2P4_COLOR);
      iLED[LED_2P4] = false;
  }

  // test switch and set LED based on result
  if(!digitalRead(PIN_K1))
  {
    switchActivity |= ATTACK_K1;
    
    // switch is active
    if(iLED[LED_K1] == true)
    {
      //maintain color while switch is active
      iLED[LED_K1] = true;
    }
    else
    {
      // select new color when switch is first activated
      // setLEDRandomColor(LED_K1);
      kaimana.setLED(LED_K1, BLACK);
      iLED[LED_K1] = true;
      kaimana.setLED(LED_2K1, _IDLE_BTN_2K1_COLOR);
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_K1, BLACK);    
      kaimana.setLED(LED_K1, _IDLE_BTN_K1_COLOR);    
      iLED[LED_K1] = false;
      kaimana.setLED(LED_2K1, _IDLE_BTN_2K1_COLOR);
      iLED[LED_2K1] = false;
  }

  // test switch and set LED based on result
  if(!digitalRead(PIN_K2))
  {
    switchActivity |= ATTACK_K2;
    
    // switch is active
    if(iLED[LED_K2] == true)
    {
      //maintain color while switch is active
      iLED[LED_K2] = true;
    }
    else
    {
      // select new color when switch is first activated
      // setLEDRandomColor(LED_K2);
      kaimana.setLED(LED_K2, BLACK);
      iLED[LED_K2] = true;
      kaimana.setLED(LED_2K2, _IDLE_BTN_2K2_COLOR);
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_K2, BLACK);    
      kaimana.setLED(LED_K2, _IDLE_BTN_K2_COLOR);    
      iLED[LED_K2] = false;
      kaimana.setLED(LED_2K2, _IDLE_BTN_2K2_COLOR);
      iLED[LED_2K2] = false;
  }

  // test switch and set LED based on result
  if(!digitalRead(PIN_K3))
  {
    switchActivity |= ATTACK_K3;
    
    // switch is active
    if(iLED[LED_K3] == true)
    {
      //maintain color while switch is active
      iLED[LED_K3] = true;
    }
    else
    {
      // select new color when switch is first activated
      // setLEDRandomColor(LED_K3);
      kaimana.setLED(LED_K3, BLACK);
      iLED[LED_K3] = true;
      kaimana.setLED(LED_2K3, _IDLE_BTN_2K3_COLOR);
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_K3, BLACK);    
      kaimana.setLED(LED_K3, _IDLE_BTN_K3_COLOR);    
      iLED[LED_K3] = false;
      kaimana.setLED(LED_2K3, _IDLE_BTN_2K3_COLOR);
      iLED[LED_2K3] = false;
  }

  // test switch and set LED based on result
  if(!digitalRead(PIN_START))
  {
    // switch is active
    if(iLED[LED_START] == true)
    {
      //maintain color while switch is active
      iLED[LED_START] = true;
	    //Button hold to start tourneymode
      holdTimeout += 1;
      if(holdTimeout == 5000)
      {
        tournamentMode = true;
        tourneyModeActivate();
      }	
    }
    else
    {
      // select new color when switch is first activated
      //setLEDRandomColor(LED_HOME);
      iLED[LED_START] = true;
    }
  }
  else
  {
    // switch is inactive
    //kaimana.setLED(LED_HOME, BLACK);    
    iLED[LED_START] = false;
	  holdTimeout=0;
  }

  // zero active switch counter (note: 4 way joystick counts as 1)
  iActiveSwitchCount = 0;
  
  // set LED color based on switch
  for(i=0;i<LED_COUNT;++i)
  {
    if(iLED[i] == true)
      ++iActiveSwitchCount;
  }  

  // update the leds with new/current colors in the array
  if( millis() < ulTimeout )
    {
      kaimana.updateALL();
    }
  
  // return number of active switches
  return(iActiveSwitchCount);
}  

int tourneypollSwitches(void)
{
  static int  iLED[LED_COUNT];
  static int  iActiveSwitchCount = 0;
  static int  i;
  static int  j;
  static int  firsttime;
  static uint16_t  joystickDirection;
  static uint16_t  switchActivity;

  // test switch and set LED based on result
  if(!digitalRead(PIN_START))
  {
	  //Button hold to change idle animation
		holdTimeout += 1;
		//Button hold to start tourneymode		
		if(holdTimeout == 2000/MAIN_LOOP_DELAY)
		{
      tournamentMode = false;
      tourneyModeDeactivate();
		}
    ++iActiveSwitchCount;
	  delay(MAIN_LOOP_DELAY);	  
  }
  else
  {
    holdTimeout=0;
  }
    return(iActiveSwitchCount);
}
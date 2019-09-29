/* 
  francoisrrr
  => deleted/commented LED_JOY; LED_HOME; LED_SELECT; LED_START; LED_K4
  => deleted showStartup() as it is redundant with defaultStartup() defined in animations.cpp
  => deleted combo test from pollSwitches()
*/


#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"
#include <EEPROM.h> // EEPROM Libray is necessary to save button layout configuration after power off


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

// local function declarations
int pollSwitches(void);
int tourneypollSwitches(void);
void setLEDRandomColor(int index);
boolean tournamentMode = false;
int holdTimeout = 0;

int layoutselect=EEPROM.read(0); // read user selected button layout from EEPROM at adress 0

/*
  Pre-set button layouts
    btnlayout[layout][button][rgb]
    P1->P2->P3->P4->K1->K2->K3->2P1->2P2->2P3->2P4->2K1->2K2->2K3
*/

// Arcade cabinet layouts
const int btnlayout[4][14][3] = {
  // 2 Buttons JAMMA
  {{RED},{RED},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLUE},{BLUE},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK}},
  // 3 Buttons JAMMA
  {{RED},{RED},{RED},{BLACK},{BLACK},{BLACK},{BLACK},{BLUE},{BLUE},{BLUE},{BLACK},{BLACK},{BLACK},{BLACK}},
  // 4 Buttons SNK
  {{RED},{YELLOW},{GREEN},{BLUE},{BLACK},{BLACK},{BLACK},{RED},{YELLOW},{GREEN},{BLUE},{BLACK},{BLACK},{BLACK}},
  // 6 Buttons CAPCOM
  {{RED},{RED},{RED},{BLACK},{RED},{RED},{RED},{BLUE},{BLUE},{BLUE},{BLACK},{BLUE},{BLUE},{BLUE}}
};

// Fighstick layouts
/* const int btnlayout[2][14][3] = {
  // 4 Buttons SNK
  {{RED},{YELLOW},{GREEN},{BLUE},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK}},
  // 6 Buttons CAPCOM
  {{RED},{RED},{RED},{BLACK},{RED},{RED},{RED},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK},{BLACK}}
}; */

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
    delay( MAIN_LOOP_DELAY );
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

  // reset LED status
  if (firsttime == 1)
  {
    for(i=0;i<LED_COUNT;++i)
    {
      iLED[i] = false;
      firsttime = 0;
    }
  }
  
  // zero active switch counter (note: 4 way joystick counts as 1)
  iActiveSwitchCount = 0;

  // complex special case for joystick but it's worth the effort
  if(!digitalRead(PIN_RIGHT)){
    iActiveSwitchCount ++;
  }
  if(!digitalRead(PIN_LEFT)){
    iActiveSwitchCount ++;
  }
  if(!digitalRead(PIN_DOWN)){
    iActiveSwitchCount ++;
  } 
  if(!digitalRead(PIN_UP)){
    iActiveSwitchCount ++;
  }

  // ### LED_P1
  // test switch and set LED based on result
  if(!digitalRead(PIN_P1))
  {    
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
      //kaimana.setLED(LED_P1, _ON_PRESS_BTN_COLOR);
      iLED[LED_P1] = true;
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_P2, BLACK);
      kaimana.setLED(LED_P1, btnlayout[layoutselect][0][0],btnlayout[layoutselect][0][1],btnlayout[layoutselect][0][2]);    
      iLED[LED_P1] = false;
      kaimana.setLED(LED_2P1, btnlayout[layoutselect][7][0],btnlayout[layoutselect][7][1],btnlayout[layoutselect][7][2]);
      iLED[LED_2P1] = false;
  }

  // ### LED_P2
  // test switch and set LED based on result
  if(!digitalRead(PIN_P2))
  {    
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
      //kaimana.setLED(LED_P2, _ON_PRESS_BTN_COLOR);
      iLED[LED_P2] = true;
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_P2, BLACK);
      kaimana.setLED(LED_P2, btnlayout[layoutselect][1][0],btnlayout[layoutselect][1][1],btnlayout[layoutselect][1][2]);    
      iLED[LED_P2] = false;
      kaimana.setLED(LED_2P2, btnlayout[layoutselect][8][0],btnlayout[layoutselect][8][1],btnlayout[layoutselect][8][2]);
      iLED[LED_2P2] = false;
  }

  // ### LED_P3
  // test switch and set LED based on result
  if(!digitalRead(PIN_P3))
  {    
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
      // kaimana.setLED(LED_P3, _ON_PRESS_BTN_COLOR);
      iLED[LED_P3] = true;
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_P3, BLACK);    
      kaimana.setLED(LED_P3, btnlayout[layoutselect][2][0],btnlayout[layoutselect][2][1],btnlayout[layoutselect][2][2]);    
      iLED[LED_P3] = false;
      kaimana.setLED(LED_2P3, btnlayout[layoutselect][9][0],btnlayout[layoutselect][9][1],btnlayout[layoutselect][9][2]);
      iLED[LED_2P3] = false;
  }

  // ### LED_P4
  // test switch and set LED based on result
  if(!digitalRead(PIN_P4))
  {    
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
      // kaimana.setLED(LED_P4, _ON_PRESS_BTN_COLOR);
      iLED[LED_P4] = true;
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_P4, BLACK);    
      kaimana.setLED(LED_P4, btnlayout[layoutselect][3][0],btnlayout[layoutselect][3][1],btnlayout[layoutselect][3][2]);    
      iLED[LED_P4] = false;
      kaimana.setLED(LED_2P4, btnlayout[layoutselect][10][0],btnlayout[layoutselect][10][1],btnlayout[layoutselect][10][2]);
      iLED[LED_2P4] = false;
  }

  // ### LED_K1
  // test switch and set LED based on result
  if(!digitalRead(PIN_K1))
  {    
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
      // kaimana.setLED(LED_K1, _ON_PRESS_BTN_COLOR);
      iLED[LED_K1] = true;
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_K1, BLACK);    
      kaimana.setLED(LED_K1, btnlayout[layoutselect][4][0],btnlayout[layoutselect][4][1],btnlayout[layoutselect][4][2]);    
      iLED[LED_K1] = false;
      kaimana.setLED(LED_2K1, btnlayout[layoutselect][11][0],btnlayout[layoutselect][11][1],btnlayout[layoutselect][11][2]);
      iLED[LED_2K1] = false;
  }

  // ### LED_K2
  // test switch and set LED based on result
  if(!digitalRead(PIN_K2))
  {    
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
      // kaimana.setLED(LED_K2, _ON_PRESS_BTN_COLOR);
      iLED[LED_K2] = true;
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_K2, BLACK);    
      kaimana.setLED(LED_K2, btnlayout[layoutselect][5][0],btnlayout[layoutselect][5][1],btnlayout[layoutselect][5][2]);    
      iLED[LED_K2] = false;
      kaimana.setLED(LED_2K2, btnlayout[layoutselect][12][0],btnlayout[layoutselect][12][1],btnlayout[layoutselect][12][2]);
      iLED[LED_2K2] = false;
  }

  // ### LED_K3
  // test switch and set LED based on result
  if(!digitalRead(PIN_K3))
  {    
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
      // kaimana.setLED(LED_K3, _ON_PRESS_BTN_COLOR);
      iLED[LED_K3] = true;
    }
  }
  else
  {
      // switch is inactive
      // kaimana.setLED(LED_K3, BLACK);    
      kaimana.setLED(LED_K3, btnlayout[layoutselect][6][0],btnlayout[layoutselect][6][1],btnlayout[layoutselect][6][2]);    
      iLED[LED_K3] = false;
      kaimana.setLED(LED_2K3, btnlayout[layoutselect][13][0],btnlayout[layoutselect][13][1],btnlayout[layoutselect][13][2]);
      iLED[LED_2K3] = false;
  }

  // ### PIN_START
  // test switch and set LED based on result
  if(!digitalRead(PIN_START))
  {
    // switch is active
    if(iLED[LED_START] == true)
    {
      //maintain color while switch is active
      iLED[LED_START] = true;
	    //Button hold
      holdTimeout += 1;
      if(holdTimeout == 1000*5/MAIN_LOOP_DELAY)
      // cycle button layout
      {
        layoutselect = (layoutselect<sizeof(btnlayout)/sizeof(*btnlayout)-1)?layoutselect+1:0;
        // write to EEPROM at adress 0
        EEPROM.update(0, layoutselect);
      }
      else if (holdTimeout == 2000*5/MAIN_LOOP_DELAY)
      // activate tourneymode
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

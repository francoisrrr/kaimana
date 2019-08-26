// ----------------------------------------------------------------------------
francoisrrr

** Description
  2 players arcade panel layout with 7 LED per player (4 Punch & 3 Kicks). It works fine with a 1 player layout too ;)

  KAIMANA->LED_K3->LED_K2->LED_K1->LED_P1->LED_P2->LED_P3->LED_P4->LED_2P1->LED_2P2->LED_2P3->LED_2P4->LED_2K3->LED_2K2->LED_2K1

  => LED are set to _IDLE_BTN_COLOR when switch is inactive and turn BLACK when switch is active
  => LED layouts are selected by pressing HOME for 2s
      JAMMA Layout : 3 buttons in line
      SNK Layout : 4 buttons in line
      CAPCOM Layout : 6 buttons
      ARCSYS Layout : 4+2 buttons
  => Tournament Mode is activated by pressing HOME for 5s

  NB) LED_JOY; LED_HOME; LED_SELECT; LED_START; LED_K4 are deactivated
  NB) Combo animations are deactivated


** Modifications
  Code is based on the default from https://paradisearcadeshop.com
  kaimana_custom.h   => LED mapping edit
  kaimana.cpp        => deleted LED_JOY code from setLED()
  kaimana.h          => added PINK color constant
  animations.cpp     => updated trackled[] from 16 to 28 indexes
                     => tourneyModeActivate() and tourneyModeDeactivate() according with LED map
  .ino               => pollSwitches() commented setLED() for LED_JOY; LED_HOME; LED_SELECT; LED_START; LED_K4
                     => pollSwitches() modified setLED() parameters for LED_P and LED_K
                     => deleted showStartup() as it is redundant with defaultStartup() defined in animations.cpp
                     => commented combo test from pollSwitches()

// ----------------------------------------------------------------------------
Paradise Arcade Shop Kaimana LED Driver Board
Initial Release October 15, 2013


Copyright 2013 Paradise Arcade Shop, ParadiseArcadeShop.com  
All rights reserved.  Use is subject to license terms.

Code is provided for entertainment purposes and use with the Kaimana controller.
Code may be copied, modified, resused with this Copyright notice.
No commercial use without written permission from Paradise Arcade Shop.

Paradise Arcade Shop Kaimana LED Driver Board
Initial Release October 15, 2013

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.



The Kaimana class library is based on original source released by ParadiseArcadeShop.com
with feedback from the community.

Created:  October 24, 2013    zonbipanda  -- Arduino 1.0.5 Support

Revised:  October 29, 2013    zonbipanda

Revised:  April   11, 2015    zonbipanda  -- Arduino 1.6.3 Support

_
/*
* Events.h
*
* Part of Arduino Event System.
*
* Author: mromani@ottotecnica.com
* Copyright (c) 2010 OTTOTECNICA Italy
*
* This library is free software; you can redistribute it
* and/or modify it under the terms of the GNU Lesser
* General Public License as published by the Free Software
* Foundation; either version 2.1 of the License, or (at
* your option) any later version.
*
* This library is distributed in the hope that it will
* be useful, but WITHOUT ANY WARRANTY; without even the
* implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*
* You should have received a copy of the GNU Lesser
* General Public License along with this library; if not,
* write to the Free Software Foundation, Inc.,
* 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*
*/

#ifndef Events_h
#define Events_h

// Some common events.
// Codes 0..199 are available for
// user defined events.

class Events {

public:
   enum eventType {
       // no event occurred
       // param: none
       EV_NONE = 200,
       
       // a key was pressed
       // param: key code
       EV_KEY_PRESS,
       
       // a key was released
       // param: key code
       EV_KEY_RELEASE,
       
       // use this to notify a character
       // param: the character to be notified
       EV_CHAR,
       
       // generic time event
       // param: a time value (exact meaning is defined
       // by the code inserting this event into the queue)
       EV_TIME,
       
       // generic timer events
       // param: same as EV_TIME
       EV_TIMER0,
       EV_TIMER1,
       EV_TIMER2,
       EV_TIMER3,
       
       // analog read (last number = analog channel)
       // param: value read
       EV_ANALOG0,
       EV_ANALOG1,
       EV_ANALOG2,
       EV_ANALOG3,
       EV_ANALOG4,
       EV_ANALOG5,
       
       // menu events
       EV_MENU0,
       EV_MENU1,
       EV_MENU2,
       EV_MENU3,
       EV_MENU4,
       EV_MENU5,
       EV_MENU6,
       EV_MENU7,
       EV_MENU8,
       

       // serial event
       // example: a new char is available
       //          param: the return value of Serial.read()
       EV_SERIAL,
       
       // LCD screen needs to be refreshed
       EV_PAINT
   };
};

#endif

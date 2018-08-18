/*

* EventDispatcher.cpp

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


#include "EventDispatcher.h"





EventDispatcher::EventDispatcher(EventQueue* evQueue) {

   q = evQueue;

   numListeners = 0;

}





boolean EventDispatcher::addEventListener(int ev_code, EventListener f, OverwriteOption overwrite) {

   int k;

   

   // argument check

   if (f == 0) {

       return false;

   }

   

   if (overwrite == OVERWRITE_EVENT) {

       k = _searchEventCode(ev_code);

       if (k >= 0) {           // event code found

           callback[k] = f;    // replace function

           enabled[k] = true;  // it's an add(), so enable the listener

           return true;

       }

   }



   // APPEND or (OVERWRITE and NOT FOUND)

   

   // dispatch table is full

   if (numListeners == MAX_LISTENERS) {

       return false;

   }

   

   callback[numListeners] = f;

   eventCode[numListeners] = ev_code;

   enabled[numListeners] = true;

   

   numListeners++;

   

   return true;

}





boolean EventDispatcher::removeEventListener(int ev_code, EventListener f) {

   int i;

   int k;

   

   if (numListeners == 0) {

       return false;

   }

   

   k = _searchEventListener(ev_code, f);

   if (k < 0) {

       return false;

   }

   

   for (i = k; i < numListeners - 1; i++) {

       callback[i] = callback[i + 1];

       eventCode[i] = eventCode[i + 1];

       enabled[i] = enabled[i + 1];

   }

   

   numListeners--;

   

   return true;

}





boolean EventDispatcher::enableEventListener(int ev_code, EventListener f, boolean enable) {

   int k;



   if (numListeners == 0) {

       return false;

   }

   

   k = _searchEventListener(ev_code, f);

   if (k < 0) {

       return false;

   }

   

   enabled[k] = enable;

   

   return true;

}





boolean EventDispatcher::isEventListenerEnabled(int ev_code, EventListener f) {

   int k;



   if (numListeners == 0) {

       return false;

   }

   

   k = _searchEventListener(ev_code, f);

   if (k < 0) {

       return false;

   }

   

   return enabled[k];

}







void EventDispatcher::run() {

   int event;

   int param;

   int i;

   boolean handlerFound;

   

   handlerFound = false;



   if (q->dequeueEvent(&event, &param)) {

       for (i = 0; i < numListeners; i++) {

           if ((callback[i] != 0) && (eventCode[i] == event) && enabled[i]) {

               handlerFound = true;

               (*callback[i])(event, param);

           }

       }

       

       if (!handlerFound) {

           if ((defaultCallback != 0) && (defaultCallbackEnabled)) {

               (*defaultCallback)(event, param);

           }

       }

   }

}





boolean EventDispatcher::setDefaultListener(EventListener f) {

   if (f == 0) {

       return false;

   }

   

   defaultCallback = f;

   defaultCallbackEnabled = true;

   return true;

}





void EventDispatcher::removeDefaultListener() {

   defaultCallback = 0;

   defaultCallbackEnabled = false;

}





void EventDispatcher::enableDefaultListener(boolean enable) {

   defaultCallbackEnabled = enable;

}





int EventDispatcher::_searchEventListener(int ev_code, EventListener f) {

   int i;

   

   for (i = 0; i < numListeners; i++) {

       if ((eventCode[i] == ev_code) && (callback[i] == f)) {

           return i;

       }

   }

   

   return -1;

}





int EventDispatcher::_searchEventCode(int ev_code) {

   int i;

   

   for (i = 0; i < numListeners; i++) {

       if (eventCode[i] == ev_code) {

           return i;

       }

   }

   

   return -1;

}


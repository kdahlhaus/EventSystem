/*

* EventDispatcher.h

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



#ifndef EventDispatcher_h

#define EventDispatcher_h



#include <WProgram.h>

#include <Events.h>

#include <EventQueue.h>





typedef void (*EventListener)(int ev_code, int ev_param);





class EventDispatcher {



public:

   // maximum number of event/callback entries

   // can be changed to save memory or allow more events to be dispatched

   static const int MAX_LISTENERS = 20;

   

   enum OverwriteOption { ALWAYS_APPEND, OVERWRITE_EVENT };

   

   // the function f will be called when event ev_code will be dequeued

   // returns true if the listener is successfully installed,

   // false otherwise (e.g. the dispatch table is full)

   // Overwrite options:

   // ALWAYS_APPEND   = just add ev_code/f to the list

   // OVERWRITE_EVENT = if a listener with the same event is found, replace its function with f

   boolean addEventListener(int ev_code, EventListener f, OverwriteOption overwrite = ALWAYS_APPEND);

   

   // remove event listener

   // other listeners with the same function or ev_code will not be affected

   boolean removeEventListener(int ev_code, EventListener f);

   

   // enable or disable a listener

   // return true if the listener was successfully enabled or disabled,

   // false if the listener was not found

   boolean enableEventListener(int ev_code, EventListener f, boolean enable);

   

   boolean isEventListenerEnabled(int ev_code, EventListener f);

   

   // the default listener is a callback function that is called when

   // an event with no listener is dequeued

   boolean setDefaultListener(EventListener f);

   void removeDefaultListener();

   void enableDefaultListener(boolean enable);

   

   // this must be continuously called (in loop())

   void run();

   

   // get the current number of entries in the dispatch table

   int getNumEntries() { return numListeners; }

   

   // get a referenct to the underlying event queue

   EventQueue* getEventQueue() { return q; }



   // a dispatcher is used to process events of a particular queue

   EventDispatcher(EventQueue* evQueue);



private:

   // event queue to be managed

   EventQueue* q;

   

   // actual number of event listeners

   int numListeners;



   // pointers to callback functions

   EventListener callback[MAX_LISTENERS];

   

   // each listener observes a specific event type

   int eventCode[MAX_LISTENERS];

   

   // each listener can be enabled or disabled

   boolean enabled[MAX_LISTENERS];

   

   // callback function to be called for event types

   // which have no listener

   EventListener defaultCallback;

   

   // once set, the default callback function can be enabled or disabled

   boolean defaultCallbackEnabled;

   

   // returns the array index of the specified listener

   // or -1 if no such event/function couple is found

   int _searchEventListener(int ev_code, EventListener f);

   

   int _searchEventCode(int ev_code);

};



#endif



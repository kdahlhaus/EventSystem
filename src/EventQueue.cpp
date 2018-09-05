/*
* EventQueue.cpp
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

#include "EventQueue.h"


EventQueue::EventQueue() {
   init();
}


void EventQueue::init() {
 int i;
 
 eventQueueHead = 0;
 eventQueueTail = EVQUEUE_SIZE - 1;
 numEvents = 0;
 
 for (i = 0; i < EVQUEUE_SIZE; i++) {
   eventQueue[i] = Events::EV_NONE;
   eventParam[i] = 0;
 }
}


bool EventQueue::isEmpty() {
 return (numEvents == 0);
}


bool EventQueue::isFull() {
 return (eventQueueHead == eventQueueTail);
}


int EventQueue::getNumEvents() {
 return numEvents;
}


bool EventQueue::enqueueEvent(int ev_code, void *ev_param) {

 if (isFull()) {
   return false;
 }

 // store the event
 eventQueue[eventQueueHead] = ev_code;
 eventParam[eventQueueHead] = ev_param;

 // update queue head value
 eventQueueHead = (eventQueueHead + 1) % EVQUEUE_SIZE;;

 // update number of events in queue
 numEvents++;

 return true;
}


bool EventQueue::dequeueEvent(int* ev_code, void **ev_param) {
 int temp;
 bool isEmpty;
 
 if (numEvents == 0) {
   return false;
 }

 eventQueueTail = (eventQueueTail + 1) % EVQUEUE_SIZE;

 // store event code and event parameter
 // into the user-supplied variables
 *ev_code = eventQueue[eventQueueTail];
 *ev_param = eventParam[eventQueueTail];

 // update number of events in queue
 numEvents--;

 return true;
}


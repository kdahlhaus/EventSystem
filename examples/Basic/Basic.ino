#include <Events.h>
#include <EventQueue.h>
#include <EventDispatcher.h>

EventQueue q;
EventDispatcher dispatcher(&q);

// Events 0-199 are open for definition.
// Others are defined in Events.h
#define SAMPLE_EVENT 1 

// event handler
void eventHandler(int event, int param) {
   Serial.print("eventHandler:");
   Serial.println(param);
}


void setup() {
   Serial.begin(9600);
   
   // register a SAMPLE_EVENT handler
   dispatcher.addEventListener(SAMPLE_EVENT, eventHandler);
}
 
void loop() {

    // get events from the queue and call the
    // registered function(s)
    dispatcher.run();

    // generate two SAMPLE_EVENT s every 1.5 seconds
    static unsigned long prevMillis = 0;
    unsigned long currMillis;
    currMillis = millis();
    if (currMillis - prevMillis >= 1500) {
        prevMillis = currMillis;

        // this will default the parameter to -1 
        q.enqueueEvent(SAMPLE_EVENT);

        // this send the event with parameter=123
        q.enqueueEvent(SAMPLE_EVENT, 123);
    }    
}

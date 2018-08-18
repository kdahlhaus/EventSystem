#include <Events.h>
#include <EventQueue.h>
#include <EventDispatcher.h>

// the event queue
EventQueue q;

// the event dispatcher
EventDispatcher disp(&q);

// use this analog channel
#define AN_CHAN 0

// generate an event when the analog
// channel value changes this much
// increase value for noisy sources
#define AN_DELTA 5

// let's suppose we have three pushbuttons
// A, B and C connected to digital pins 5, 3, 4
#define NUM_KEYS 3
#define KEY_A 1
#define KEY_B 2
#define KEY_C 3
int kb_pins[NUM_KEYS] = { 5, 3, 4 };
int kb_keys[NUM_KEYS] = { KEY_A, KEY_B, KEY_C };
int kb_on[NUM_KEYS] = { 0, 0, 0 };
int kb_off[NUM_KEYS] = { 0, 0, 0 };
int kb_curr_state[NUM_KEYS] = { 0, 0, 0 };
int kb_prev_state[NUM_KEYS] = { 0, 0, 0 };
#define KB_SAMPLE_DELAY 20
#define KB_DEBOUNCE_COUNT 3

// led to be turned on or off
#define LED_PIN 13


// time event handler
void timeHandler(int event, int param) {
   Serial.print("Time elapsed: ");
   Serial.println(millis() / 1000);
}


// analog event handler
void analogHandler(int event, int param) {
   Serial.print("Analog value: ");
   Serial.println(param);
}


// keyboard event handler
void kbHandler(int event, int param) {
   switch(event) {
       case Events::EV_KEY_PRESS:
           Serial.print("key pressed: ");
           if (param == KEY_B) {
               digitalWrite(LED_PIN, 1);    // key B turns the led ON
           }
           break;
       
       case Events::EV_KEY_RELEASE:
           Serial.print("key released: ");
           if (param == KEY_C) {
               digitalWrite(LED_PIN, 0);    // key C turns the led OFF
           }
           break;
   }
   
   switch(param) {
       case KEY_A:
           Serial.print("A");
           break;
       
       case KEY_B:
           Serial.print("B");
           break;
       
       case KEY_C:
           Serial.print("C");
           break;
   }
   
   Serial.println();
}


// this function generates an EV_TIME event
// each 1000 ms
void timeManager() {
   static unsigned long prevMillis = 0;
   unsigned long currMillis;

   currMillis = millis();
   if (currMillis - prevMillis >= 1000) {
       prevMillis = currMillis;
       q.enqueueEvent(Events::EV_TIME, 0);    // param is not used here
   }
}


// this function generates an EV_ANALOG event
// whenever the analog channel AN_CHAN changes
void analogManager() {
   static int prevValue = 0;
   int currValue;

   currValue = analogRead(AN_CHAN);

   if (abs(currValue - prevValue) >= AN_DELTA) {
       prevValue = currValue;
       q.enqueueEvent(Events::EV_ANALOG0, currValue);    // use param to pass analog value to event handler
   }
}


// this function debounces the pushbutton pins
// and emits the "keyboard events"
void kbManager() {
   static unsigned long prevMillis = 0;
   unsigned long currMillis;
   int i;
   
   currMillis = millis();
   if (currMillis - prevMillis >= KB_SAMPLE_DELAY) {
       prevMillis = currMillis;
       for (i = 0; i < NUM_KEYS; i++) {
           if (!digitalRead(kb_pins[i])) {    // buttons are active low
               kb_on[i]++;
               kb_off[i] = 0;
           }
           else {
               kb_on[i] = 0;
               kb_off[i]++;
           }
           
           if (kb_on[i] >= KB_DEBOUNCE_COUNT) {
               kb_on[i] = 0;
               kb_off[i] = 0;
               kb_curr_state[i] = 1;
               if (kb_prev_state[i] == 0) {
                   // it was released and now it's pressed
                   kb_prev_state[i] = 1;
                   q.enqueueEvent(Events::EV_KEY_PRESS, kb_keys[i]);
               }
           }
           
           if (kb_off[i] >= KB_DEBOUNCE_COUNT) {
               kb_on[i] = 0;
               kb_off[i] = 0;
               kb_curr_state[i] = 0;
               if (kb_prev_state[i] == 1) {
                   // it was pressed and now it's released
                   kb_prev_state[i] = 0;
                   q.enqueueEvent(Events::EV_KEY_RELEASE, kb_keys[i]);
               }
           }
       }
   }
}


// program setup
void setup() {
   Serial.begin(115200);
   
   // call timeHandler for TIME events
   disp.addEventListener(Events::EV_TIME, timeHandler);
   
   // call analogHandler for ANALOG0 events
   disp.addEventListener(Events::EV_ANALOG0, analogHandler);
   
   // call kbHandler for _both_ KEY_PRESS and KEY_RELEASE events
   disp.addEventListener(Events::EV_KEY_PRESS, kbHandler);
   disp.addEventListener(Events::EV_KEY_RELEASE, kbHandler);
}


// loop
void loop() {
   // call the event generating functions
   timeManager();
   analogManager();
   kbManager();

   // get events from the queue and call the
   // registered function(s)
   disp.run();
}


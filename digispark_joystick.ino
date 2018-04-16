#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "DigiJoystick.h"

#define CHANNELS 8          // we are working with an up to 8-ch-Transmitter (ppm standard is a max of 8 ch)
#define PulseLength  985

#define x1 125
#define x2 125
#define y1 125
#define y2 125
#define sw1 125
#define sw2 125
#define aux1 125
#define aux2 125

volatile int valuesInt[CHANNELS+1] = {0};
volatile int valuesUse[CHANNELS+1] = {0};
volatile byte counter = CHANNELS;
volatile boolean ready = false;
volatile uint8_t timelast, timelastloop, GoodPulses;

void errorDetection(int error){
  for (int i = 0; i < error; i++){ 
        digitalWrite(1, HIGH);
        delay(25);
        digitalWrite(1, LOW);
        delay(25);
  }
  delay(250);
}


void setup(){
  
  #if defined (__AVR_ATtiny85__)
    if (F_CPU ==  16000000) clock_prescale_set(clock_div_1);
  #endif
  
  pinMode(1, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, EvalPPM, RISING);    
}

void EvalPPM(){
  uint8_t timenew = micros();
  uint8_t timediff = timenew - timelast;
  timelast = timenew;
  if (timediff > 3000){ // this must be the gap between set of pulses to synchronize
    counter = 0;
  } 
  else {
    if (900 < timediff && timediff < 2200 && counter < CHANNELS){
      valuesInt[counter] = timediff;
      if(counter<4 && timediff>PulseLength){
        GoodPulses |= (1<<counter); // if signal is valid - mark channel as OK
      }
      if(GoodPulses==0x0F) {                                               // If first four chanels have good pulses, clear FailSafe counter
        GoodPulses = 0;
      }    
    }
    ready = true;
    counter++;
  }
}

void loop(){
 char flysky[CHANNELS] = { x1, y1, x2, y2, sw1, sw2, aux1, aux2 };

  if(ready){
   for (int i = 0; i < CHANNELS; i++){ 
    flysky[i] = (valuesUse[i]/4)-250;
   }
    DigiJoystick.setValues(flysky);
    DigiJoystick.delay(50); // wait 50 milliseconds
    ready = false;
//    errorDetection(CHANNELS);
    digitalWrite(1, HIGH);
  }
  else{
//    errorDetection(1);
    digitalWrite(1, LOW);
  }
} 

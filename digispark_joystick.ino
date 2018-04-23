#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "DigiJoystick.h"

#define CHANNELS 8          // we are working with an up to 8-ch-Transmitter (ppm standard is a max of 8 ch)

#define x1 125
#define x2 125
#define y1 125
#define y2 125
#define sw1 125
#define sw2 125
#define aux1 125
#define aux2 125
#define AVG 3

volatile int valuesUse[CHANNELS+1] = {0};
volatile byte counter = CHANNELS;
volatile boolean ready = false;
volatile long timelast, timelastloop;

void errorDetection(int error){
  for (int i = 0; i < error; i++){ 
        digitalWrite(1, HIGH);
        delay(25);
        digitalWrite(1, LOW);
        delay(25);
  }
  delay(250);
}

void EvalPPM(){
  long timenew = micros();
  long timediff = timenew - timelast;
  timelast = timenew;
  
  if (timediff > 3000){ // this must be the gap between set of pulses to synchronize
   counter= 0;
   ready = true;
  }
  else {
    if (985< timediff && timediff < 2200 && counter < CHANNELS){
      valuesUse[counter] = timediff;
      counter++;
    }
  }
}

void setup(){

  #if defined (__AVR_ATtiny85__)
    if (F_CPU ==  16000000) clock_prescale_set(clock_div_1);
  #endif
   
  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, EvalPPM, RISING);
  timelast = micros();
  timelastloop = timelast;
 }

void loop(){ 
   char flysky[CHANNELS] = { x1, y1, x2, y2, sw1, sw2, aux1, aux2 };
   if(ready){
   for (int i = 0; i < CHANNELS; i++){ 
    flysky[i] = (byte)((valuesUse[i]/4)-250), HEX;
    //flysky[i] = valuesUse[i];
   }
    ready = false;
//   errorDetection(CHANNELS);
   digitalWrite(1, HIGH);
   DigiJoystick.setValues(flysky);
   DigiJoystick.delay(50); // wait 50 milliseconds
   }
   else{
//    errorDetection(1);
      digitalWrite(1, LOW);
      DigiJoystick.update();
   }
} 

#ifdef __AVR__
  #include <avr/power.h>      // added for attiny85 
#endif

#include "DigiJoystick.h"

#define CHANNELS 14          // still not working with more than 8-ch (fs-a8s limitation) 22-ch will be eventually implemented
#define AVG 3          // 3 added values for average values (don't think this is right at all)

#define x1 125
#define x2 125
#define y1 125
#define y2 125
#define var1 125
#define var2 125
#define sw1 125
#define sw2 125
#define sw3 125
#define sw4 125
#define aux0 125
#define aux1 125
#define aux2 125
#define aux3 125

volatile int valuesInt[CHANNELS+1] = {0};
volatile int valuesUse[CHANNELS+1] = {0};
volatile byte counter = CHANNELS;
volatile byte average  = AVG;
volatile boolean ready = false;
volatile long timelast;
long timelastloop;

void EvalPPM()
{
  long timenew = micros();
  long timediff = timenew - timelast;
  timelast = timenew;
  if (timediff > 2500)   // this must be the gap between set of pulses to synchronize, was 2200 but didn't work well
  { valuesInt[CHANNELS] = valuesInt[CHANNELS] + timediff;
    counter = 0; 
    if (average == AVG)
    { for (int i = 0; i < CHANNELS + 1; i++)
      { valuesUse[i] = (valuesInt[i] + 0.5) / average;
      if(valuesUse[i] < 1020) valuesUse[i] = 1000;
      if(valuesUse[i] > 1980) valuesUse[i] = 2000;
      valuesInt[i] = 0;
      }
      average = 0;
      ready = true;
    }  
    average++;
  }
  else
  { if (counter < CHANNELS)
    { valuesInt[counter] = valuesInt[counter] + timediff;
      counter++;
    }
  }
}

void setup(){
  
  #if defined (__AVR_ATtiny85__)
    if (F_CPU ==  16000000) clock_prescale_set(clock_div_1);    // should set clock to 16 MHz tried 8 MHz without luck
  #endif
   
  Serial.begin(115200);
  //Serial.println(F("Start reading PPM-Signal from Remote-Control"));
  pinMode(2, INPUT_PULLUP); 
  attachInterrupt(0, EvalPPM, RISING);  
  timelast = micros();
  timelastloop = timelast;
}

void loop() {
 char flysky[CHANNELS] = { 
  x1, y1, x2, y2, var1, var2, sw1, sw2, sw3, sw4, aux0, aux1, aux2, aux3
 };
  
 if (ready){ 
  for (int i = 0; i < CHANNELS; i++){ 
    Serial.print(valuesUse[i]);
    if (i < CHANNELS ) Serial.print(", ");
    flysky[i] = (valuesUse[i]/4)-250;
  }
  Serial.println();
  DigiJoystick.setValues(flysky);
  DigiJoystick.delay(50); // wait 50 milliseconds
  ready = false;
  }  
}

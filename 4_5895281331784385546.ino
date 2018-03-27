//DigiJoystick test and usage documentation

#include "DigiJoystick.h"

#define NUM_OF_CHL 8          // we are working with an 8-ch-Transmitter
#define NUM_OF_AVG 3         // 3 added values for average

#define x1 125
#define x2 125
#define y1 125
#define y2 125
#define var1 125
#define var2 125
#define sw1 125
#define sw2 125

volatile int valuesInt[9] = {0};
volatile int valuesUse[9] = {0};
volatile byte counter = NUM_OF_CHL;
volatile byte average  = NUM_OF_AVG;
volatile boolean ready = false;
volatile long timelast;
long timelastloop;

void EvalPPM()
{
  long timenew = micros();
  long timediff = timenew - timelast;
  timelast = timenew;
  if (timediff > 2200)   // this must be the gap between set of pulses to synchronize
  { valuesInt[NUM_OF_CHL] = valuesInt[NUM_OF_CHL] + timediff;
    counter = 0; 
    if (average == NUM_OF_AVG)
    { for (int i = 0; i < NUM_OF_CHL + 1; i++)
      { valuesUse[i] = (valuesInt[i] + 0.5) / average;
      if(valuesUse[i] < 1000) valuesUse[i] = 1000;
      if(valuesUse[i] > 2000) valuesUse[i] = 2000;
      valuesInt[i] = 0;
      }
      average = 0;
      ready = true;
    }  
    average++;
  }
  else
  { if (counter < NUM_OF_CHL)
    { valuesInt[counter] = valuesInt[counter] + timediff;
      counter++;
    }
  }
}

void setup(){
  Serial.begin(115200);
  //Serial.println(F("Start reading PPM-Signal from Remote-Control"));
  pinMode(2, INPUT_PULLUP ); 
  attachInterrupt(0, EvalPPM, RISING);  
  timelast = micros();
  timelastloop = timelast;
}

void loop() {
 byte flysky[8] = { 
  x1, y1, x2, y2, var1, var2, sw1, sw2
 };
 
 
 if (ready){ 
  for (int i = 0; i < NUM_OF_CHL; i++){ 
    Serial.print(valuesUse[i]);
    if (i < NUM_OF_CHL ) Serial.print(", ");
    flysky[i] = (valuesUse[i]/4)-250;
  }
  Serial.println();
  DigiJoystick.setValues(flysky);
  DigiJoystick.delay(50); // wait 50 milliseconds
  ready = false;
  }  
}

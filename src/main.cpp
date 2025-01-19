#include <Arduino.h>


//specification notes: 
// two digital outputs (LED's)
// signal A (DATA), signal B (sync)

// two digital inputs (buttons) - Note buttons should not be required to be held down to update state
// OUTPUT_ENABLE @ PB1 & OUTPUT_SELECT @ PB2
// OUTPUT_ENABLE will enable/disable DATA output 
// OUTPUT_SELECT will switch between normal or alternative waveforms on DATA 

// timing variable for PRODUCTION or DEBUG
// Debug mode slows parameters by factor of 1000
// Conditional code compile

//normal output timing parameters
  //DataPeriodOn1 (A) period On seems to be multiplied by number of signals starting at 1 leadgin up to C
  //DataPeriodOff (B)
  //DataPeriodOffFinal (D)
  //SyncPeriodOn 50uS in production mode 
  //SyncPeriod "dependant on other parameter"

  //calculations
    //surname = WILLS => W=4, I=9, L=12, L=12 
    // DataPeriodOn1 (A) = 4*100uS = 0.4mS
    // DataPeriodOff (B) = 9*100uS = 0.9mS
    // NumPulses (C) = 12 + 4 = 16 pulses
    // DataPeriodOffFinal (D) = 12 * 500uS = 6mS

    //DataPeriodOn[n] = DataPeriodOn1 + ((n-1) * 50uS)

//alternative output timing parameters
  //surname = WILLS => S=8
  //  8%4+1 = 1
  //Option 1: removes final three pulses from waveform 

void setup() {
  // put your setup code here, to run once:
 
}

void loop() {
  // put your main code here, to run repeatedly:
}


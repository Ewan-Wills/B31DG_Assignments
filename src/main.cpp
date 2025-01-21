#include <Arduino.h>

#include <iostream>
#include <thread>
using namespace std;
#include <list>
//specification notes: 
// two digital outputs (LED's)
// signal A (DATA), signal B (SYNC)

// two digital inputs (buttons) - Note buttons should not be required to be held down to update state
// OUTPUT_ENABLE @ PB1 & OUTPUT_SELECT @ PB2
// OUTPUT_ENABLE will enable/disable DATA output 
// OUTPUT_SELECT will switch between normal or alternative waveforms on DATA 

// timing variable for PRODUCTION or DEBUG
// Debug mode slows parameters by factor of 1000
// *Conditional code compile*

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

//define if program will compile in production or debug 
#define PRODUCTION false

const int NumPulses = 16;

int DataPeriodOn[NumPulses+1];

int DataPeriodOff;
int DataPeriodOffFinal; 
int DataPeriodOn1;
int SyncPeriodOn;
  
//OUTPUT_ENABLE True if enabled, false if disabled
bool OUTPUT_ENABLE = false; 
//OUTPUT_SELECT true if normal and false if alternative 
bool OUTPUT_SELECT = true;

//push button pin numbers
const int PB1 = 11; //OUTPUT_ENABLE
const int PB2 = 12; //OUTPUT_SELECT
//button states to remember state from previous loop
bool ButtonState1 = false;
bool ButtonState2 = false;

//signal pin numbers
const int SignalA = 21; //DATA
const int SignalB = 22; //SYNC

void syncOutput(){}

void dataOutput(){
  while (true){
    Serial.print("output enable: ");
    Serial.println(OUTPUT_ENABLE);
    
    //pulse SYNC at the start of period
    digitalWrite(SignalB, HIGH);
    delayMicroseconds(SyncPeriodOn);
    digitalWrite(SignalB, LOW);
    
    
    //normal mode
    if (OUTPUT_SELECT){ 
      for (int n=1; n<NumPulses; n++){ 
          //LED on for DataPeriodOn
          if(OUTPUT_ENABLE){digitalWrite(SignalA, HIGH);} //Only make data pin high if OUTPUT_ENABLE high
          delayMicroseconds(DataPeriodOn[n]);
          digitalWrite(SignalA, LOW);
          //delay between pulses
          delayMicroseconds(DataPeriodOff);
        
      } 
      //delay between end of pulses and start of next period
      delayMicroseconds(DataPeriodOffFinal);
    }
    //alternative mode
    else{
          //dont play final three pulses
        for (int n=1; n<NumPulses-3; n++){    
          //LED on for DataPeriodOn
          if(OUTPUT_ENABLE){digitalWrite(SignalA, HIGH);} //Only make data pin high if OUTPUT_ENABLE high
          delayMicroseconds(DataPeriodOn[n]);
          digitalWrite(SignalA, LOW);
          //delay between pulses
          delayMicroseconds(DataPeriodOff);
        } 
        //delay between end of pulses and start of next period
        delayMicroseconds(DataPeriodOffFinal);
    }
    
  }
}
void setup() {
  //all values in microseconds
  if (PRODUCTION){
    
    const int DataPeriodOff = 900; //0.9mS
    
    DataPeriodOffFinal = 500; //500uS
    DataPeriodOn1 = 400; //uS
    SyncPeriodOn = 50;//50uS

    //calculate all DataPeriodOn's for NumPulses for PRODUCTION
    for (int n=1; n<NumPulses; n++){
      DataPeriodOn[n] = DataPeriodOn1 + (n-1)*(50*(10^-6));
    }      
  }else{
    DataPeriodOff = 900*1000; //0.9S
    DataPeriodOffFinal = 500*1000; //500mS
    DataPeriodOn1 = 400*1000;//mS
    SyncPeriodOn = 50*1000;//50mS

    //calculate all DataPeriodOn's for NumPulses *1000 for DEBUG
    for (int n=1; n<NumPulses; n++){
      DataPeriodOn[n] = DataPeriodOn1 + (n-1)*(50*(10^-3));
    }  
  }

  //initiate push buttons
  pinMode(PB1, INPUT);
  pinMode(PB2, INPUT);

  //initiate signal lines
  pinMode(SignalA, OUTPUT);
  pinMode(SignalB, OUTPUT);
}

void loop() {

  //start data output on a different thread (asynchrenously) from main loop 
  thread dataOutputThread(dataOutput);

  //if button 1 pushed 
  if (digitalRead(PB1)){
    //if button wasnt already pressed last time around 
    if (!ButtonState1){
      //flip state of OUTPUT_ENABLE
      OUTPUT_ENABLE = !OUTPUT_ENABLE;
      ButtonState1=true;
      //print message on serial 
      Serial.print("Button 1 pressed"); //serial.print may not be asynchrenous 
      Serial.println(OUTPUT_ENABLE);
    }
  }
  //if button isnt pushed 
  else{  
    //button pressed = false
    ButtonState1=false;
  }

  //if button 2 pushed 
  if (digitalRead(PB2)){
    //if button wasnt already pressed last time around 
    if (!ButtonState2){
      //flip state of OUTPUT_SELECT
      OUTPUT_SELECT = !OUTPUT_SELECT;
      ButtonState2=true;
      //print message on serial 
      Serial.print("Button 2 pressed"); //serial.print may not be asynchrenous 
      Serial.println(OUTPUT_SELECT);
    }
  }
  //if button isnt pushed 
  else{  
    //button pressed = false
    ButtonState2=false;
  }


}
  


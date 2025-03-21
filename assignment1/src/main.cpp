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

// normal output timing parameters
//   DataPeriodOn1 (A) period On seems to be multiplied by number of signals starting at 1 leadgin up to C
//   DataPeriodOff (B)
//   DataPeriodOffFinal (D)
//   SyncPeriodOn 50uS in production mode 
//   SyncPeriod "dependant on other parameter"

//   calculations
//     surname = WILLS => W=4, I=9, L=12, L=12 
//     DataPeriodOn1 (A) = 4*100uS = 0.4mS
//     DataPeriodOff (B) = 9*100uS = 0.9mS
//     NumPulses (C) = 12 + 4 = 16 pulses
//     DataPeriodOffFinal (D) = 12 * 500uS = 6mS

//     DataPeriodOn[n] = DataPeriodOn1 + ((n-1) * 50uS)

// alternative output timing parameters
//   surname = WILLS => S=8
//    8%4+1 = 1
//   Option 1: removes final three pulses from waveform 

//define if program will compile in production or debug 
#define PRODUCTION true

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
const int PB1 = 25; //OUTPUT_ENABLE
const int PB2 = 26; //OUTPUT_SELECT
//button states to remember state from previous loop
bool ButtonState1 = false;
bool ButtonState2 = false;

//signal pin numbers
const int SignalA = 32; //DATA -- red
const int SignalB = 33; //SYNC -- green

void ayncDelayMicroseconds(int64_t numDelay){
  //delay() and delayMicroseconds dont work async
  //vTaskDelay(pdMS_TO_TICKS()); doesnt work with microseconds
  //Serial.println(numDelay);
  //use esp builtin timer to delay for set amount of time
  
  int64_t microseconds = esp_timer_get_time();
  if (numDelay!=0){
      while (((int64_t) esp_timer_get_time() - microseconds) <= numDelay){}
  }

  //ets_delay_us(numDelay);
}

void dataOutput(void *){
  int currentNumPulses;
  while (true){    
    //pulse SYNC at the start of period
    digitalWrite(SignalB, HIGH);
    ayncDelayMicroseconds(SyncPeriodOn);
    digitalWrite(SignalB, LOW);
    
    
    //normal mode
    
      for (int n=1; n<=NumPulses; n++){ 
          
        if ((OUTPUT_SELECT) && (n>NumPulses-3)){//Note >= will stop at 12, > will stop at 13.                  
        }else{
          
          if(OUTPUT_ENABLE){digitalWrite(SignalA, HIGH);} //Only make data pin high if OUTPUT_ENABLE high

          ayncDelayMicroseconds(DataPeriodOn[n]);        
          digitalWrite(SignalA, LOW);
          //delay between pulses
          ayncDelayMicroseconds(DataPeriodOff);
        }
    }   
    //delay between end of pulses and start of next period
    ayncDelayMicroseconds(DataPeriodOffFinal); 
        
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println(""); 
  //all values in microseconds
  if (PRODUCTION){
    
    DataPeriodOff = 900; //900uS    
    DataPeriodOffFinal = 6000; //6mS
    DataPeriodOn1 = 400; //400uS
    SyncPeriodOn = 50;//50uS

    //calculate all DataPeriodOn's for NumPulses for PRODUCTION
    for (int n=1; n<=NumPulses; n++){
      
      DataPeriodOn[n] = DataPeriodOn1 + (n-1)*(50);
      Serial.print("Generated PRODUCTION: DataPeriodOn[");
      Serial.print(n);
      Serial.print("]: ");
      Serial.print(DataPeriodOn[n]);
      Serial.println("uS");
    }  

  }else{
    DataPeriodOff = 900*1000; //900mS - b
    DataPeriodOffFinal = 6000*1000; //6S - d
    DataPeriodOn1 = 400*1000;//400mS - a
    SyncPeriodOn = 50*1000;//50mS

    //calculate all DataPeriodOn's for NumPulses *1000 for DEBUG
    for (int n=1; n<=NumPulses; n++){
      DataPeriodOn[n] = DataPeriodOn1 + (n-1)*(50*(1000));
    
      Serial.print("Generated DEBUG: DataPeriodOn[");
      Serial.print(n);
      Serial.print("]: ");
      Serial.print(DataPeriodOn[n]);
      Serial.println("uS");
    }  
  }
  Serial.println(""); 
  Serial.print("SyncPeriodOn: ");
  Serial.print(SyncPeriodOn);
  Serial.println("uS");
  Serial.print("DataPeriodOff: ");
  Serial.print(DataPeriodOff);
  Serial.println("uS");
  Serial.print("DataPeriodOffFinal: ");
  Serial.print(DataPeriodOffFinal);
  Serial.println("uS");

  


  //initiate push buttons
  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB2, INPUT_PULLUP);

  //initiate signal lines
  pinMode(SignalA, OUTPUT);
  pinMode(SignalB, OUTPUT);


  digitalWrite(SignalA, LOW);
  digitalWrite(SignalB, LOW);

  //start data output on a different thread (asynchrenously) from main loop 
  //thread dataOutputThread(dataOutput);
  xTaskCreatePinnedToCore(dataOutput, "dataOutput", 4096, NULL, 1, NULL,0);

}

void loop() {

  //start data output on a different thread (asynchrenously) from main loop 
  //thread dataOutputThread(dataOutput);
  xTaskCreatePinnedToCore(dataOutput, "dataOutput", 4096, NULL, 1, NULL,0);

  //if button 1 pushed 
  if (digitalRead(PB1)){
    //if button wasnt already pressed last time around 
    if (!ButtonState1){
      //flip state of OUTPUT_ENABLE
      OUTPUT_ENABLE = !OUTPUT_ENABLE;
      ButtonState1=true;
      //print message on serial 
      Serial.print("PB1 pressed:OUTPUT_ENABLE = "); //serial.print may not be asynchrenous 
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
      Serial.print("PB2 pressed: OUTPUT_SELECT = "); //serial.print may not be asynchrenous 
      Serial.println(OUTPUT_SELECT);
    }
  }
  //if button isnt pushed 
  else{  
    //button pressed = false
    ButtonState2=false;
  }


}
  


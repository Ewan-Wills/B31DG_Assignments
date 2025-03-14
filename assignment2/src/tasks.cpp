#ifndef main_h
#include "main.h"
#endif
//create a boolean to hold whether the button was already pushed in the main loop
bool buttonAlreadyPushed;
  
tasks::tasks(B31DGCyclicExecutiveMonitor *monitor)
{
    _monitor = monitor;

    // initialise F1 and F2 so task 6 can recall values from taks 3 and 4
    this->F1 = 0.0;
    this->F2 = 0.0;
    this->task7LedState = false;


    //set modes for pins used
    pinMode(outputPinTask1, OUTPUT); // output for task 1
    pinMode(outputPinTask2, OUTPUT); // output for task 2
    pinMode(outputPinTask6, OUTPUT); // output for sum of frequencies
    pinMode(outputPinTask7, OUTPUT); // output for task 7 toggle led

    pinMode(inputPinTask3, INPUT);        // frequency input for task 3
    pinMode(inputPinTask4, INPUT);        // frequency input for task 4
    pinMode(inputPinTask7, INPUT_PULLUP); // button for task 7
}

void tasks::tasksDelayuS(int numDelay){
    #if program1 || edfScheduler
        delayMicroseconds(numDelay);
    #endif
    #if program2
        vTaskDelay(pdMS_TO_TICKS(numDelay/1000));
    #endif
  }

void tasks::doTask(int taskNum)
{
    //start the job on the monitor, run the task, then end the job on the monitor 
    if(taskNum<=5){ _monitor->jobStarted(taskNum);}
    
    switch (taskNum)
    {
    case 1:
        (task1()); break;
    case 2:
        (task2()); break;
    case 3:
        (task3()); break;
    case 4:
        (task4()); break;
    case 5:
        (task5()); break;
    case 6:
        (task6()); break;
    case 7:
        (task7()); break;

    default: break;
    }
    if(taskNum<=5){_monitor->jobEnded(taskNum);}
}
// Output a digital signal. This should be HIGH for 250μs, then LOW for 50μs, then HIGH again for 300μs, then LOW again.
//Takes 600uS
void tasks::task1()
{
    //tasksDelayuS(600);
    digitalWrite(outputPinTask1, HIGH);
    tasksDelayuS(250);
    digitalWrite(outputPinTask1, LOW);
    tasksDelayuS(50);
    digitalWrite(outputPinTask1, HIGH);
    tasksDelayuS(300);
    digitalWrite(outputPinTask1, LOW);
}
// Output a second digital signal. This should be HIGH for 100μs, then LOW for 50μs, then HIGH again for 200μs, then LOW again
//Takes 350uS
void tasks::task2()
{
    digitalWrite(outputPinTask2, HIGH);
    tasksDelayuS(100);
    digitalWrite(outputPinTask2, LOW);
    tasksDelayuS(50);
    digitalWrite(outputPinTask2, HIGH);
    tasksDelayuS(200);
    digitalWrite(outputPinTask2, LOW);
}
// Measure the frequency of a 3.3v square wave signal
//Takes 1000uS to 1500uS 
void tasks::task3()
{
    tasksDelayuS(1000);
    //F1 = measureFreq(inputPinTask3);
}
// Measure the frequency of a 3.3v square wave signal
//Takes 667uS to 1200uS
void tasks::task4()
{
    tasksDelayuS(667);
    //F2 = measureFreq(inputPinTask4);
}
// Call the monitor’s method doWork().
//Takes 500uS
void tasks::task5()
{
    //tasksDelayuS(500);
    _monitor->doWork();
}

// Use an LED to indicate whether the sum of the two frequencies F1 and F2 is greater than 1500
void tasks::task6()
{
    //tasksDelayuS(10);
    // write pin high if sum of frequencies is greater than 1500. If equal or lower, write pin low.
    digitalWrite(outputPinTask6, (F1 + F2 > 1500.0));
    // Serial.print("Sum=");
    // Serial.print(F1+F2);
    // Serial.println();
}
// Monitor a pushbutton. Toggle the state of a second LED and call the monitor’s method doWork() whenever the pushbutton is pressed.
void tasks::task7()
{
    //If button pressed (and wasnt already pressed since last loop), toggle led and doWork()
    //tasksDelayuS(10);
    if (digitalRead(inputPinTask7))
    {
        buttonAlreadyPushed = false;
    }
    else {
        if(!buttonAlreadyPushed){
            buttonAlreadyPushed = true;
            task7LedState = !task7LedState;
            digitalWrite(outputPinTask7, task7LedState);
            _monitor->doWork();    
        }
    }

}

double tasks::measureFreq(int pin)
{
    int startTime;
    int endTime;
    double period;
    //if pin high, wait for pin to be low, start timer and end timer when high again. This is half the period
    if (digitalRead(pin)) { 
        while (digitalRead(pin)){}; 
        startTime = micros();
        while (!digitalRead(pin)){};
    }
    //if pin low, wait for pin to be high, start timer and end timer when low again. This is half the period
    else{
        while (!digitalRead(pin)){}; 
        startTime = micros();
        while (digitalRead(pin)){};
    }

    //Start time =998804 End time=999518
    endTime = micros();
    double perioduS =(double) (2* (endTime - startTime) );
    double freq = (1/ (((perioduS)) * (10e-7)) );

    // Find difference between now and start time. Since we know wave is 50% duty cycle, this (the difference) will be half of the period. Multiply by two to get the period

    // return the inverse of the period (i.e. frequency). Round the frequency to integer
    // Serial.print("measurefreq:");
    // Serial.println(freq);
    return (freq);

};
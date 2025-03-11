#include "tasks.h"
//create a boolean to hold whether the button was already pushed in the main loop
bool buttonAlreadyPushed;

tasks::tasks(B31DGCyclicExecutiveMonitor *monitor)
{
    _monitor = monitor;

    // initialise F1 and F2 so task 6 can recall values from taks 3 and 4
    this->F1 = 0;
    this->F2 = 0;
    //
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

void tasks::doTask(int taskNum)
{
    //start the job on the monitor, run the task, then end the job on the monitor 
    // _monitor->jobStarted(taskNum);
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
    // _monitor->jobEnded(taskNum);
}
// Output a digital signal. This should be HIGH for 250μs, then LOW for 50μs, then HIGH again for 300μs, then LOW again.
//Takes 600uS
void tasks::task1()
{
    // Serial.println("TASK1");
    delayMicroseconds(600);
    // digitalWrite(outputPinTask1, HIGH);
    // delayMicroseconds(250);
    // digitalWrite(outputPinTask1, LOW);
    // delayMicroseconds(50);
    // digitalWrite(outputPinTask1, HIGH);
    // delayMicroseconds(300);
    // digitalWrite(outputPinTask1, LOW);
}
// Output a second digital signal. This should be HIGH for 100μs, then LOW for 50μs, then HIGH again for 200μs, then LOW again
//Takes 350uS
void tasks::task2()
{
    //Serial.println("TASK2");
    delayMicroseconds(350);
    // digitalWrite(outputPinTask2, HIGH);
    // delayMicroseconds(100);
    // digitalWrite(outputPinTask2, LOW);
    // delayMicroseconds(50);
    // digitalWrite(outputPinTask2, HIGH);
    // delayMicroseconds(200);
    // digitalWrite(outputPinTask2, LOW);
}
// Measure the frequency of a 3.3v square wave signal
//Takes 1000uS to 1500uS 
void tasks::task3()
{
    //Serial.println("TASK3");
    delayMicroseconds(1000);
    //F1 = measureFreq(inputPinTask3);
}
// Measure the frequency of a 3.3v square wave signal
//Takes 667uS to 1200uS
void tasks::task4()
{
    // Serial.println("TASK4");
    delayMicroseconds(667);
    //F2 = measureFreq(inputPinTask4);
}
// Call the monitor’s method doWork().
//Takes 500uS
void tasks::task5()
{
    // Serial.println("TASK5");
    delayMicroseconds(500);
    //_monitor->doWork();
}

// Use an LED to indicate whether the sum of the two frequencies F1 and F2 is greater than 1500
void tasks::task6()
{
    // write pin high if sum of frequencies is greater than 1500. If equal or lower, write pin low.
    digitalWrite(outputPinTask6, (F1 + F2 > 1500));
}
// Monitor a pushbutton. Toggle the state of a second LED and call the monitor’s method doWork() whenever the pushbutton is pressed.
void tasks::task7()
{
    //If button pressed (and wasnt already pressed since last loop), toggle led and doWork()
    if (!digitalRead(inputPinTask7))
    {
        buttonAlreadyPushed = false;
    }
    if (digitalRead(inputPinTask7) && !buttonAlreadyPushed)
    {
        buttonAlreadyPushed = true;
        task7LedState = !task7LedState;
        digitalWrite(outputPinTask7, task7LedState);
        _monitor->doWork();    
    }

}

int tasks::measureFreq(int pin)
{
    long currentTime;
    long differenceTime;
    // if pin is high, wait for pin to be low
    if (digitalRead(pin))
    {
        while (digitalRead(pin))
            ;
    }
    // if pin was low, wait for pin to be high. If pin was high it would have waited and now is low.

    while (!digitalRead(pin))
        ;
    // This ensures we are getting the time at the rising edge of pulse.
    currentTime = micros();
    // wait while the signal is high
    while (digitalRead(pin))
        ;
    // wait while the signal is low
    while (!digitalRead(pin))
        ;

    // calculate time since rising edge of signal. this is the period of the signal
    differenceTime = (micros() - currentTime);

    // return the inverse of the period (i.e. frequency). Round the frequency to integer and cast back to long
    return ((int)round(1 / differenceTime));
};
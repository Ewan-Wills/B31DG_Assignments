#ifndef main_h
#include "main.h"
#endif
// create a boolean to hold whether the button was already pushed in the main loop
bool buttonAlreadyPushed;

tasks::tasks(B31DGCyclicExecutiveMonitor *monitor)
{
    _monitor = monitor;

    // initialise F1 and F2 so task 6 can recall values from taks 3 and 4
    this->F1 = 0.0;
    this->F2 = 0.0;
    this->task7LedState = false;

    // set modes for pins used
    pinMode(outputPinTask1, OUTPUT); // output for task 1
    pinMode(outputPinTask2, OUTPUT); // output for task 2
    pinMode(outputPinTask6, OUTPUT); // output for sum of frequencies
    pinMode(outputPinTask7, OUTPUT); // output for task 7 toggle led

    pinMode(inputPinTask3, INPUT);        // frequency input for task 3
    pinMode(inputPinTask4, INPUT);        // frequency input for task 4
    pinMode(inputPinTask7, INPUT_PULLUP); // button for task 7
}
/* Delay in microseconds*/
void tasks::tasksDelayuS(int numDelay)
{
//If we are running on without freertos we can use delayMicroseconds function. If we are using freertos we should use vTaskDelay 
#if program1 || edfScheduler
    delayMicroseconds(numDelay);
#endif
#if program2
    vTaskDelay(pdMS_TO_TICKS(numDelay / 1000));
#endif
}
/*Run task with number 1 - 7*/
void tasks::doTask(int taskNum)
{
    // start the job on the monitor, run the task, then end the job on the monitor

    //Only start monitor if task is between 1 and 5
    if (taskNum <= 5)
    {
        _monitor->jobStarted(taskNum);
    }

    switch (taskNum)
    {
    case 1:
        (task1());
        break;
    case 2:
        (task2());
        break;
    case 3:
        (task3());
        break;
    case 4:
        (task4());
        break;
    case 5:
        (task5());
        break;
    case 6:
        (task6());
        break;
    case 7:
        (task7());
        break;

    default:
        break;
    }
    if (taskNum <= 5)
    {
        _monitor->jobEnded(taskNum);
    }
}
/*
Output a digital signal. This should be HIGH for 250μs, then LOW for 50μs, then HIGH again for 300μs, then LOW again.
Takes 600uS
*/
void tasks::task1()
{
    // tasksDelayuS(600);
    digitalWrite(outputPinTask1, HIGH);
    tasksDelayuS(250);
    digitalWrite(outputPinTask1, LOW);
    tasksDelayuS(50);
    digitalWrite(outputPinTask1, HIGH);
    tasksDelayuS(300);
    digitalWrite(outputPinTask1, LOW);
}
/*
Output a second digital signal. This should be HIGH for 100μs, then LOW for 50μs, then HIGH again for 200μs, then LOW again
Takes 350uS 
*/
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
/*
Measure the frequency of a 3.3v square wave signal
Takes 1000uS to 1500uS
*/
void tasks::task3()
{
    //tasksDelayuS(1500);
    F1 = measureFreq(inputPinTask3);
}
/*
Measure the frequency of a 3.3v square wave signal
Takes 667uS to 1200uS
*/
void tasks::task4()
{
    //tasksDelayuS(1200);
    F2 = measureFreq(inputPinTask4);
}
/*
Call the monitor’s method doWork().
Takes 500uS
*/
void tasks::task5()
{
    _monitor->doWork(); //Uses delayMicroseconds but doesnt seem to cause issue with freertos timers so all good
}

/*
Use an LED to indicate whether the sum of the two frequencies F1 and F2 is greater than 1500
*/
void tasks::task6()
{
    //  write pin high if sum of frequencies is greater than 1500. If equal or lower, write pin low.
    digitalWrite(outputPinTask6, (F1 + F2 > 1500.0));
}
/*
Monitor a pushbutton. Toggle the state of a second LED and call the monitor’s method doWork() whenever the pushbutton is pressed.
*/
void tasks::task7()
{
    // If button pressed (and wasnt already pressed since last loop), toggle led and doWork()
    if (digitalRead(inputPinTask7))
    {
        buttonAlreadyPushed = false;
    }
    else
    {
        if (!buttonAlreadyPushed)
        {
            buttonAlreadyPushed = true;
            task7LedState = !task7LedState;
            digitalWrite(outputPinTask7, task7LedState);
            _monitor->doWork();
        }
    }
}
/*
Returns frequency of given pin as a double
*/
double tasks::measureFreq(int pin)
{
    int startTime;
    int endTime;
    // If pin high, wait for pin to be low, start timer and end timer when high again. This is half the period
    if (digitalRead(pin))
    {
        while (digitalRead(pin))
        {//Do nothing
        };
        startTime = micros();
        while (!digitalRead(pin))
        {//Do nothing
        };
    }
    // If pin low, wait for pin to be high, start timer and end timer when low again. This is half the period
    else
    {
        while (!digitalRead(pin))
        {//Do nothing
        };
        startTime = micros();
        while (digitalRead(pin))
        {//Do nothing
        };
    }
    endTime = micros();
    //The peruid in microseconds is the difference between start and end time multiplied by 2. Since we know 50% duty cycle the "on time" or "off time" is always half the period
    double perioduS = (double)(2 * (endTime - startTime));
    //Frequency is the inverse of period. Period is period in us *10^-6 (= 1.0e-6 ). 
    double freq = (1 / (((perioduS)) * (1e-6)));

    return (freq);
};
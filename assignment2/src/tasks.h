
#include <Arduino.h>
#include <B31DGMonitor.h>

//define pins 
#define outputPinTask1 12
#define outputPinTask2 14
#define outputPinTask6 27
#define outputPinTask7 26

#define inputPinTask3 25
#define inputPinTask4 33
#define inputPinTask7 32


//create 
class tasks
{
private:
    int F1;
    int F2;
    bool task7LedState;
    void task1();
    void task2();
    void task3();
    void task4();
    void task5();
    void task6();
    void task7();
    int measureFreq(int pin);    
    B31DGCyclicExecutiveMonitor *_monitor;
public:   
    tasks(B31DGCyclicExecutiveMonitor *monitor);
    void doTask(int taskNumber);
    void ayncDelayMicroseconds(int delay_us); 
};


#include <Arduino.h>
#include <B31DGMonitor.h>

//define pins 
#define outputPinTask1 16
#define outputPinTask2 17
#define inputPinTask3 18
#define inputPinTask4 19
#define outputPinTask6 20
#define inputPinTask7 21
#define outputPinTask7 22

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
    
};

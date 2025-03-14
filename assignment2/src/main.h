#define main_h
#include <Arduino.h>
#include <B31DGMonitor.h>
#include <tasks.h>

//only one true at a time please
#define edfScheduler false
#define program1 true
#define program2 false



// number of tasks including 6 and 7. Note that NUMBER_TASKS from B31DGMonitor.h is 5
#define NUMTASKS 7


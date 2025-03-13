// // First Program. Implement Cyclic Excecutive
// #include <tasks.h> //Note: tasks.h includes B31DGMonitor and Arduino headers
// #include "freertos/FreeRTOS.h"
// #include "freertos/timers.h"

// int deadlineLengths[7] = {4, 3, 10, 10, 5, 1, 1}; //timer only has conversion for milliseconds

// // Timer handles
// TimerHandle_t timer[7];

// // create pointer monitor and task objects
// B31DGCyclicExecutiveMonitor *monitor;
// tasks *task;

// // Timer callback functions
// void Timer1Callback(TimerHandle_t xTimer)
// {
//     int taskToRun = (int) pvTimerGetTimerID(xTimer);
//     Serial.printf("Timer %d Executed at %lu ms\n", taskToRun, millis(), '\n');

//     monitor->jobStarted(taskToRun);
//     task->doTask(taskToRun);
//     monitor->jobEnded(taskToRun);
// }
// void setup()
// {
//     // Start serial monitor so that the monitor can print its results
//     Serial.begin(115200);

//     // Create monitor object. No offset used currently
//     monitor = new B31DGCyclicExecutiveMonitor();

//     // create object for handling tasks
//     task = new tasks(monitor);

//     // activate the monitor
//     monitor->startMonitoring();

//     // Create FreeRTOS software timers
//     int i = 1;
//     timer[i-1] = xTimerCreate(("Timer"+i), pdMS_TO_TICKS(deadlineLengths[i-1]), pdTRUE, (void*) i ,Timer1Callback );

//     if (timer[i] != NULL)
//     {
//         Serial.printf("Timer %d created successfully, starting...",i, '\n');
//         xTimerStart(timer[i-1], 0);
//     }
//     else
//     {
//         Serial.printf("Failed to create timer: %d", i , '\n');
//     }
// }

// void loop()
// {
// }

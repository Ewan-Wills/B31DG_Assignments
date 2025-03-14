// // First Program. Implement Cyclic Excecutive
// #include <tasks.h> //Note: tasks.h includes B31DGMonitor and Arduino headers

// int deadlineLengths[7] = {4, 3, 10, 10, 5, 1, 1}; //timer only has conversion for milliseconds

// // Timer handles
// TimerHandle_t timer[7];
// #define NUMTASKS 7
// // create pointer monitor and task objects
// B31DGCyclicExecutiveMonitor *monitor;
// tasks *task;

// // Timer callback functions
// void taskCallback(TimerHandle_t xTimer)
// {
//     int taskToRun = (int) pvTimerGetTimerID(xTimer);
    
//     while (1){
//         int startTime = micros();
//         monitor->jobStarted(taskToRun);
//         task->doTask(taskToRun);
//         monitor->jobEnded(taskToRun);
//         int endTime = micros();
//         int difference = endTime-startTime;
//         vTaskDelay(deadlineLengths[taskToRun-1] -difference);
//     }

    
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
//     BaseType_t xTask[NUMTASKS];
//     xTask[i-1] = xTaskCreate(&taskCallback, ("task"+i),configMINIMAL_STACK_SIZE, (void*) i, 1, NULL);

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

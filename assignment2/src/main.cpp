// First Program. Implement Cyclic Excecutive
#include <tasks.h> //Note: tasks.h includes B31DGMonitor and Arduino headers
#include <Ticker.h>

//number of tasks including 6 and 7. Note that NUMBER_TASKS from B31DGMonitor.h is 5
#define NUMTASKS 7

// create a list to hold task deadline lengths (uS). Note: there is no deadline required for task 6 and 7 they are set to 1000 so they can be run every frame (they ar both usualy pretty quick but 7 can take 500uS if button is pressed)
int deadlineLengths[NUMTASKS] = {4000, 3000, 10000, 10000, 5000, 1000, 1000};

// create a object to hold task number and time-to-deadline
struct queueTask
{
    bool runTask;
    unsigned long timeOfDeadline;
};
//counter integer
int counter;
// create a list to hold all of the tasks and their time of deadlines
queueTask queue[NUMTASKS];

// integer for schedule to hold the task with the earlieset deadline (if -1, no tasks)
int taskToRun;


// create tickerFunc and getPriority functions 
void tickerFunc();
int getPriority();

// create ticker object
Ticker ticker;

//create pointer monitor and task objects 
B31DGCyclicExecutiveMonitor *monitor;
tasks *task;

//function to add a task number to the queue list
void addToQueue(int taskNumToAdd)
{
    // time of deadline is the current time + time to deadline. Time to deadline is deadline length- task length
    queue[taskNumToAdd - 1] = {runTask : true, timeOfDeadline : (micros() + deadlineLengths[taskNumToAdd - 1])};
}
//checks if counter is on a deadline (factors of 3,4,5 or 10) and adds respective tasks to queue
void updateQueue(int count)
{
    //run task 6 and 7 every frame. This does mean if button is pushed alot it will mess up the schedule.
    //Another solution was to have the check for button pressed in the main loop, 
    //then add the rest of task 7 (toggle led and doWork()) to the schedule. This would be safer for the schedule but may be considered an interupt which isnt allowed as far as im aware.
    if (count % 1 == 0)
    {
        addToQueue(6);
        addToQueue(7);
    }
    
    if (count % 4 == 0)
    {
        addToQueue(1);
    }
    if (count % 3 == 0)
    {
        addToQueue(2);
    }
    if (count % 10 == 0)
    {
        addToQueue(3);
        addToQueue(4);
    }
    if (count % 5 == 0)
    {
        addToQueue(5);

    }
}
//callback function for ticker object
void tickerFunc()
{
    //Add one to counter and update queue
    counter++;
    updateQueue(counter);
}
//get the task in the queue with the soonest deadline 
int getPriority()
{
    //returns -1 if no tasks in queue 
    taskToRun = -1;
    //iterate through queue
    for (int i = 1; i <= NUMTASKS; i++)
    {
        // .runTask is a boolean. If true we are supposed to run the task if its false we ignore it
        //Note: i is the task number, the index of queue for that task is i-1 
        if (queue[i - 1].runTask == true)
        {
            // if there has not yet been a task found, set it to the current task in queue
            if (taskToRun == -1)
            {
                //save this index as the current soonest task
                taskToRun = i;
            }

            // compare the current soonest task's timeOfDeadline with the current item in queue's timeOfDeadline
            if (queue[i - 1].timeOfDeadline < queue[taskToRun - 1].timeOfDeadline)
            {
                //save this index as the current soonest task
                taskToRun = i;
            }
        }
    }
    //return the task with the earliest deadline
    return taskToRun;


}
void setup()
{
    //Start serial monitor so that the monitor can print its results
    Serial.begin(115200);

    //Create monitor object. No offset used currently 
    monitor = new B31DGCyclicExecutiveMonitor();

    // create object for handling tasks
    task = new tasks(monitor);

    // activate the monitor
    monitor->startMonitoring();

    //Initially, Add all the tasks to the queue. (This is just to make the monitor show even numbers (2500,3334,1000 etc...)) 
    for (int i = 1; i <= NUMTASKS; i++)
    {
        addToQueue(i);
    }

    // Create ticker with 1ms tick period
    ticker.attach_ms(1, &tickerFunc);

    Serial.println("Started");
}

void loop()
{
    //Get the task with the earliest deadline
    int taskToRun = getPriority();
    //If not -1 i.e. there is a task inthe queue
    if (taskToRun != -1)
    {
        // set the taskNum of the item in the queue to -1. If it is -1 the schedular will ignore it until it is added again through the ticker.
        queue[taskToRun - 1].runTask = false;

        // start job in monitor and
        task->doTask(taskToRun);
    }
}

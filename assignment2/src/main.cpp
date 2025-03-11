// First Program. Implement Cyclic Excecutive
#include <tasks.h> //Note: tasks.h includes B31DGMonitor and Arduino headers
#include <Ticker.h>

// create a list to hold task lengths (microseconds). Note these are estimated not calculated
int taskLengths[7] = {600, 350, 1500, 1200, 500,0,0};

// create a list to hold task deadline lengths (microseconds). Note: there is no deadline required for task 6 and 7. It is set to 100001 so that they wont interfere with
int deadlineLengths[7] = {4000, 3000, 10000, 10000, 5000, 10001, 10001};

// create a object to hold task number and time-to-deadline
struct queueTask
{
  bool runTask;
  unsigned long timeOfDeadline;
};
int counter;
// create a list to hold all of the
queueTask queue[7];

// integer for schedule to hold the task with the earliest deadline (if -1, no tasks)
int taskToRun;
// create tickerFunc function

void tickerFunc();
int scheduler();
// create ticker object
Ticker ticker(&tickerFunc, 1 ,0 ,MICROS);

B31DGCyclicExecutiveMonitor *monitor;
tasks *task;

void addToQueue(int taskNumToAdd, unsigned long timeRef = micros())
{
  //time of deadline is the current time + time to deadline. Time to deadline is deadline length- task length
  queue[taskNumToAdd - 1] = {runTask : true, timeOfDeadline : (timeRef + deadlineLengths[taskNumToAdd - 1])};
}
void updateQueue(int count, unsigned long timeRef = micros()){
  if (count % 4 == 0){
    addToQueue(1, timeRef);
  }
  if (count % 3 == 0){
    addToQueue(2, timeRef);
  }
  if (count % 10 == 0){
    addToQueue(3, timeRef);
    addToQueue(4, timeRef);
  }
  if (count % 5 == 0){
    addToQueue(5, timeRef);
  }
}
void tickerFunc()
{
  // use counter to add queueTask objects to queue (switch case)
  // counter determines which deadline has been reached/restarted and to add a new task to queue.
  counter++;

  updateQueue(counter);  

  int taskToRun = scheduler();

  if (taskToRun != -1){
    // if (taskLengths[taskToRun] > 1000){
    //   counter++;
    //   updateQueue(counter, micros()+1000); 
    // }

    queue[taskToRun - 1].runTask = false;
    monitor->jobStarted(taskToRun);
    task->doTask(taskToRun);
    monitor->jobEnded(taskToRun);
  }
}

int scheduler(){
  // if there are items in queue run the one with the lowest timeOfDeadline

  // for (int i = 1; i <= 7; i++)
  // {
  //   Serial.print("Num: ");
  //   Serial.print(queue[i-1].runTask);
  //   Serial.print(", time: ");
  //   Serial.print(queue[i-1].timeOfDeadline);
  //   Serial.print("  ");
  // }
  // Serial.println("  ");

  taskToRun = -1;
  for (int i = 1; i <= 7; i++)
  {
    // if tasknum is -1, ignore this item in the queue
    if (queue[i-1].runTask == true)
    {
      // if there has not yet been a task found, set it to the current task in queue
      if (taskToRun == -1)
      {
        taskToRun = i;
      }
      
      
      // if there is a task found, compare its timeOfDeadline with the current item in queue's timeOfDeadline
      if (queue[i-1].timeOfDeadline < queue[taskToRun-1].timeOfDeadline)
        {
          taskToRun = i;
        }
      
    }
  }

  // if task to run does not equal -1, ie there is tasks in the queue
  if (taskToRun != -1)
  {
    // set the taskNum of the item in the queue to -1. If it is -1 the schedular will ignore it.
    // run that task
    return taskToRun;
  }else{
    return -1;
  }

  // Serial.print("Task to run: ");
  // Serial.println(taskToRun);

  //delayMicroseconds(10);
}
void setup()
{
  Serial.begin(115200);
  // ticker.interval(1000);
  //39976 - 39659
  monitor = new B31DGCyclicExecutiveMonitor();
  // create object for handling tasks
  task = new tasks(monitor);

  // activate the monitor
  monitor->startMonitoring();

  // add first 5 tasks and the deadline times to the queue
  for (int i = 1; i <= 5; i++)
  {
    addToQueue(i);
  }

  // addToQueue(6);
  
  // addToQueue(7);
  // queue[5].runTask = false;
  // queue[6].runTask = false;

  //Serial.println(micros());
  // start the ticker
  ticker.start();

  Serial.println("Started");
}

void loop()
{
  // check ticker and run callback if ready. The tickers callback function will update the queue when deadline has started
  ticker.update();

  // If button pressed (and wasnt already pressed since last loop), add task 7 to queue
  //  if (digitalRead(inputPinTask7))
  //  {
  //    addToQueue(7);
  //  }
  
}

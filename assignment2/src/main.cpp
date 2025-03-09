//First Program. Implement Cyclic Excecutive
#include <tasks.h> //Note: tasks.h includes B31DGMonitor and Arduino headers
#include <Ticker.h>

//create a list to hold task lengths (microseconds). Note these are estimated not calculated
int taskLengths[5] = {600,350,1500,1200,500};

//create a list to hold task deadline lengths (microseconds)
int deadlineLengths[5] = {4000,3000,10000,10000,5000};

//create a object to hold task number and time-to-deadline
struct queueTask{
  int taskNum;
  unsigned long timeOfDeadline;
};

//create a list to hold all of the 
queueTask queue[5];

//create counter for ticker
int counter=0;
//create tickerFunc function
void tickerFunc();

//create ticker object
Ticker ticker(&tickerFunc, 1000);

void tickerFunc(){
  //use counter to add queueTask objects to queue (switch case)
}
void setup() {
  Serial.begin(115200);

  //create object for handling tasks
  tasks task;  

  //activate the monitor 
  task.monitor.startMonitoring();
  
  //add all tasks and the deadline times to the queue
  for (int i=0; i<5;i++){
    queue[i] = {taskNum:i+1, timeOfDeadline: (micros()+deadlineLengths[i+1]) };
  }

  //start the ticker
  ticker.start();
}

void loop() {
  //check ticker and run callback if ready. Callback will update 
  ticker.update();

  //if there are items in queue run the one with the lowest timeOfDeadline

}

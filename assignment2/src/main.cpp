//First Program. Implement Cyclic Excecutive


#include <tasks.h> //Note: tasks.h includes B31DGMonitor and Arduino headers
#include <Ticker.h>

void setup() {


  //create object for handling tasks
  tasks task;  
  
  //activate the monitor 
  task.monitor.startMonitoring();
}

void loop() {

}

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
 // idea from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  long long Idle         = LinuxParser::IdleJiffies();
  long long Non_Idle     = LinuxParser::ActiveJiffies();
 
  long Total = Non_Idle + Idle; 
  
  float CPU_Percentage = (Total - Idle)/(float)Total;
  
  return CPU_Percentage; 
}
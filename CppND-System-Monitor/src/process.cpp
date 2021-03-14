#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;

Process::Process(int ID) : PassID(ID) {
}

// TODO: Return this process's ID
int Process::Pid(){ 
  	return PassID;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  // Idea from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  long seconds =  Process::UpTime(); 
  long total_time = LinuxParser::ActiveJiffies(Process::Pid())/sysconf(_SC_CLK_TCK);
  cpu_usage = total_time/(float)seconds;
  return cpu_usage; 
}

// TODO: Return the command that generated this process
string Process::Command() {	return LinuxParser::Command(Process::Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
   	return cpu_usage < a.cpu_usage;
}
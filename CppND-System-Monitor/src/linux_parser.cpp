#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str()); 
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string value;
  string key;
  int MemFree, Cached, MemTotal, Buffers;
  std::ifstream filesem(kProcDirectory + kMeminfoFilename);
    if(filesem.is_open()){
       while (std::getline(filesem, line))
       {
          std::istringstream linestream(line);
           while(linestream >> key >> value){
                if(key == "MemTotal:")MemTotal = std::stoi(value);
                if(key == "MemFree:")MemFree = std::stoi(value); 
                if(key == "Cached:")Cached = std::stoi(value);
                if(key == "Buffers:")Buffers = std::stoi(value);
           }
       }
    }
  float Mem_util = (MemTotal - MemFree - Buffers - Cached)/(float)MemTotal;
  return Mem_util; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    string line;
  	string value;
  	string uptime;
 
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> uptime >> value;
     }
  	return std::stol(uptime); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::PrevNonIdleJiffies() { 
  string line;
  string value;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  char count{1};
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
         if(count == 1){
           linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
           long total_nonIdle = stol(user) + stol(nice) + stol(system) + stol(irq) + stol(softirq) + stol(steal);
           return total_nonIdle;
          }
        count++;          
      }    
    }
  }
  return 0; 
}

long LinuxParser::PrevIdleJiffies() { 
  string line;
  string value;
  string idle, iowait;
  char count{1};
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
         if(count == 4){
           linestream >> idle >> iowait;
           long total_Idle = stol(idle) + stol(iowait);
           return total_Idle;
          }
        count++;          
      }    
    }
  }
  return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    string line;
  	string value;
    string utime, stime, cutime, cstime;
  	long total_time;
 
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)){
          std::istringstream linestream(line);
          for(int i = 1; i < 14; i++) {
             linestream >> value; 
          }
          linestream >> utime >> stime >> cutime >> cstime;
          total_time = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
          return total_time;
          } 
        }
   return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line;
  string value;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  char count{1};
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
         if(count == 1){
           linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
           long total_nonIdle = stol(user) + stol(nice) + stol(system) + stol(irq) + stol(softirq) + stol(steal);
           return total_nonIdle;
          }
        count++;          
      }    
    }
  }
  return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  string value;
  string idle, iowait;
  char count{1};
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
         if(count == 4){
           linestream >> idle >> iowait;
           long total_Idle = stol(idle) + stol(iowait);
           return total_Idle;
          }
        count++;          
      }    
    }
  }
  return 0; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  	string line;
  	string value;
  	string key;
 
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)){
        std::istringstream linestream(line);
              while(linestream >> key >> value){
                if(key == "processes")
                  return std::stoi(value);
              }
        }
    }
  	return std::stoi(value); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	string line;
  	string value;
  	string key;
 
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)){
        std::istringstream linestream(line);
              while(linestream >> key >> value){
                if(key == "procs_running")
                  return std::stoi(value);
              }
        }
    }
  	return std::stoi(value);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid){
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::getline(stream, line);
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
    string line;
  	string value;
  	string key;
 
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)){
        std::istringstream linestream(line);
              while(linestream >> key >> value){
                if(key == "VmSize:") {
                  int tempVal = stoi(value);
                  tempVal = tempVal/1000;
                  return std::to_string(tempVal);
                }
              }
        }
    } 
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string line;
  	string value;
  	string key;
 
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)){
        std::istringstream linestream(line);
              while(linestream >> key >> value){
                if(key == "Uid:")
                  return "value";
              }
        }
    } 
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    std::string line;
    std::string value;
    std::string key, id, X1, val1, val2, Idcap;
    std::ifstream stream(kPasswordPath);
    if(stream.is_open()){
       while (std::getline(stream, line))
       {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            while(linestream >> id >> X1 >> val1 >> val2 >> Idcap){
                std::string key = val1;
                if(key == LinuxParser::Uid(pid))
                	return id;
            }
        }
    }
    return id;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    string line;
  	string uptime;
    long up_time, result;
  	int count{1};
 
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)){
          std::istringstream linestream(line);
          while(linestream >> uptime){
             if(count == 22){
                up_time = std::stol(uptime)/sysconf(_SC_CLK_TCK);
               	result = LinuxParser::UpTime() - up_time;
                return result;
              }
             count++;
          }
        }
    }
  return 0;
}
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

#define Hertz sysconf(_SC_CLK_TCK)

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid) : pid_(pid) {}

// Return this process's ID
int Process::Pid() { 
  return this->pid_; 
}

// Returns this process's CPU utilization
float Process::CpuUtilization() const { 
  return this->cpuUtilization_; 
}

void Process::CpuUtilization(vector<long int>& times) {
  // calculate processor utilization
  long uptime = LinuxParser::UpTime();
  long utime = times[0];
  long stime = times[1];
  long cutime = times[2];
  long cstime = times[3];
  long starttime = times[4];
  
  long totalTime = utime + stime + cutime + cstime;
  long seconds = (long)(uptime - (starttime / (float)Hertz));
  
  this->cpuUtilization_ = ((totalTime / (float)Hertz) / seconds);
}

// Returns the command that generated this process
string Process::Command() { 
  return this->command_; 
}

void Process::Command(string command){
  this->command_ = command;
}

// Returns this process's memory utilization
string Process::Ram() const { 
  return this->ram_; 
}

void Process::Ram(string ram){
  this->ram_ = ram;
}

// Returns the user (name) that generated this process
string Process::User() { 
  return this->user_; 
}

void Process::User(string user){
  this->user_ = user;
}

// Returns the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime() - (this->upTime_ / Hertz); 
}

void Process::UpTime(long int upTime){
  this->upTime_ = upTime;
}

// Overloads the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if(cpuUtilization_ < a.CpuUtilization()){
    return true;
  }else if(cpuUtilization_ == a.CpuUtilization()){
    long ram1 = 0, ram2 = 0;
    try{
      ram1 = stol(ram_);
    }catch(...) {}
    
    try{
      ram2 = stol(a.Ram());
    }catch(...) {}
    
  	if(ram1 <= ram2){
      return true;
    }
  }
  return false; 
}
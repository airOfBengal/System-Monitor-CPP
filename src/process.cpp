#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

// Return this process's ID
int Process::Pid() { 
  return this->pid_; 
}

// Returns this process's CPU utilization
float Process::CpuUtilization() const { 
  return this->cpuUtilization_; 
}

// Returns the command that generated this process
string Process::Command() { 
  return this->command_; 
}

// Returns this process's memory utilization
string Process::Ram() const { 
  return this->ram_; 
}

// Returns the user (name) that generated this process
string Process::User() { 
  return this->user_; 
}

// Returns the age of this process (in seconds)
long int Process::UpTime() { 
  return this->upTime_; 
}

// Overloads the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  if(cpuUtilization_ < a.CpuUtilization()){
    return true;
  }else if(cpuUtilization_ == a.CpuUtilization()){
  	if(stol(ram_) <= stol(a.Ram())){
      return true;
    }
  }
  return false; 
}